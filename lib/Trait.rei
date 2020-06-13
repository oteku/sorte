module Score: {
  type t;
  /* Quelques soit le choix on se doute qu'on devra intéragir avec des int */
  let toInt: t => int;
  let fromInt: int => option(t);
  /* on souhaite avoir une arithmétique basique sans convertir en int à l'extéieur */
  let (+): (t, t) => option(t);
  let (-): (t, t) => option(t);
  /* on souhaite avoir une arithmétique basique sans convertir en int à l'extéieur */
  let print: t => string;
  let unwrap: option(t) => t;
};

type t = {
  brawn: Score.t,
  finesse: Score.t,
  resolve: Score.t,
  wits: Score.t,
  panache: Score.t,
};

let init: unit => t;