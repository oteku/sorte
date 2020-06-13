module type NationT = {
  type t;

  let make: t => result(t, exn);
};

type trait =
  | Brawn
  | Finesse
  | Resolve
  | Wits
  | Panache;

let initNation: list(trait) => (module NationT with type t = trait) =
  possibleTraits =>
    (module
     {
       type t = trait;

       let make: t => result(t, exn) =
         choosen =>
           List.find_opt(a => choosen == a, possibleTraits)
           |> Option.to_result(~none=Arg.Bad("Bad trait value"));
     });

module Avalon = (val initNation([Panache, Resolve]));
module Inismore = (val initNation([Panache, Wits]));
module TheHighlandMarches = (val initNation([Brawn, Finesse]));
module Castille = (val initNation([Finesse, Wits]));
module Eisen = (val initNation([Panache, Resolve]));
module Sarmatia = (val initNation([Brawn, Resolve]));
module Montaigne = (val initNation([Panache, Brawn]));
module Ussura = (val initNation([Resolve, Wits]));
module Vesten = (val initNation([Brawn, Wits]));
module Vodacce = (val initNation([Finesse, Resolve]));

module NationPlus = (M: NationT with type t = trait) => {
  include M;
  let make: (trait, int) => (result(M.t, exn), int) =
    (trait, bonus) => (M.make(trait), bonus);
};

module AvalonPlus = NationPlus(Avalon);
let avalonPanacheBonus = AvalonPlus.make(Panache, 1);

type t =
  | Avalon(Avalon.t)
  | Inismore(Inismore.t)
  | TheHighlandMarches(TheHighlandMarches.t)
  | Castille(Castille.t)
  | Eisen(Eisen.t)
  | Montaigne(Montaigne.t)
  | Vodacce(Vodacce.t)
  | Sarmatia(Sarmatia.t)
  | Vesten(Vesten.t)
  | Ussura(Ussura.t);