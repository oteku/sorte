open Lwt.Infix;

module Nation = {
  type t =
    | Castille
    | Montaigne
    | Vodacce;
};

module Trait = {
  type t = {
    brawn: int,
    finesse: int,
    resolve: int,
    wits: int,
    panache: int,
  };
};

module Hero = {
  type t = {
    id: string,
    name: string,
    nation: Nation.t,
    trait: Trait.t,
  };
};

let inigo_montoya =
  Hero.{
    id: "567de924-cc04-41a0-acc8-431dd332ce79",
    name: "Inigo Montoya",
    nation: Castille,
    trait: {
      brawn: 2,
      finesse: 5,
      resolve: 3,
      wits: 2,
      panache: 4,
    },
  };
let vizzini =
  Hero.{
    id: "9779a6fa-a7fe-4475-a546-350b5e3c3b56",
    name: "Vizzini",
    nation: Vodacce,
    trait: {
      brawn: 2,
      finesse: 2,
      resolve: 4,
      wits: 4,
      panache: 3,
    },
  };

/* Can require less boilerplate with ppx https://github.com/mirage/irmin/blob/master/README_PPX.md */
module IrminHero = {
  type t = Hero.t;
  let nation =
    Irmin.Type.(
      Nation.(
        variant("nation", (castille, montaigne, vodacce) =>
          fun
          | Castille => castille
          | Montaigne => montaigne
          | Vodacce => vodacce
        )
        |~ case0("Castille", Castille)
        |~ case0("Montaigne", Montaigne)
        |~ case0("Vodacce", Vodacce)
        |> sealv
      )
    );
  let trait =
    Irmin.Type.(
      Trait.(
        record("trait", (brawn, finesse, resolve, wits, panache) =>
          {brawn, finesse, resolve, wits, panache}
        )
        |+ field("brawn", int, t => t.brawn)
        |+ field("finesse", int, t => t.finesse)
        |+ field("resolve", int, t => t.resolve)
        |+ field("wits", int, t => t.wits)
        |+ field("panache", int, t => t.panache)
        |> sealr
      )
    );
  let t =
    Irmin.Type.(
      Hero.(
        record("hero", (id, name, nation, trait) =>
          {id, name, nation, trait}
        )
        |+ field("id", string, t => t.id)
        |+ field("name", string, t => t.name)
        |+ field("nation", nation, t => t.nation)
        |+ field("trait", trait, t => t.trait)
        |> sealr
      )
    );
  let merge = Irmin.Merge.(option(idempotent(t)));
};

/* Define a backend */
/* module Git_store = Irmin_unix.Git.FS.KV(Irmin.Contents.String); */
module HeroStore = Irmin_unix.Git.FS.KV(IrminHero);
module Sync = Irmin.Sync(HeroStore);

/* Define a repo */
let git_config = Irmin_git.config(~bare=true, "/tmp/irmin/demo");

/* Define a repo */
let git_repo = HeroStore.Repo.v(git_config);
let git_remote =
  HeroStore.remote(
    "https://oteku:3298touma13@github.com/oteku/demo-repo-irmin.git",
  );

/* Access master brach */
let master = config => HeroStore.Repo.v(config) >>= HeroStore.master;
/* Access a named brach */
let branch = (config, name) =>
  HeroStore.Repo.v(config) >>= (repo => HeroStore.of_branch(repo, name));

let info = (author, message) => Irmin_unix.info(~author, "%s", message);

Sync.push;
let main =
  /* HeroStore.Repo.v(git_config)
     >>= HeroStore.master
     >>= (t => Sync.pull_exn(t, git_remote, `Set))
     >>= (
       _ => */
  Lwt.join([
    HeroStore.Repo.v(git_config)
    >>= HeroStore.master
    >>= (
      t =>
        HeroStore.set_exn(
          t,
          [inigo_montoya.id],
          inigo_montoya,
          ~info=info("thaessle", "create Montoya"),
        )
        >>= (
          () =>
            HeroStore.get(t, [inigo_montoya.id])
            >|= (s => assert(s == inigo_montoya))
        )
    ),
    HeroStore.Repo.v(git_config)
    >>= HeroStore.master
    >>= (
      t =>
        HeroStore.set_exn(
          t,
          [vizzini.id],
          vizzini,
          ~info=info("thaessle", "create Vizzini"),
        )
        >>= (
          () =>
            HeroStore.get(t, [vizzini.id]) >|= (s => assert(s == vizzini))
        )
    ),
  ])
  /* ) */
  >>= (
    () =>
      HeroStore.Repo.v(git_config)
      >>= HeroStore.master
      >>= (t => Sync.push(t, git_remote))
  )
  >>= (_ => print_endline("finished") |> Lwt.return);

let () = Lwt_main.run(main);