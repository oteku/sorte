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
} = {
  type t = int;
  let toInt: t => int = ts => ts;
  let fromInt = score =>
    if (score >= 2 && score <= 5) {
      Some(score);
    } else {
      None;
    };
  let (+) = (score1, score2) => toInt(score1) + toInt(score2) |> fromInt;
  let (-) = (score1, score2) => toInt(score1) - toInt(score2) |> fromInt;
  let print = score => toInt(score) |> string_of_int;
  let unwrap = s =>
    switch (s) {
    | Some(s) => s
    | None => 0
    };
};

type t = {
  brawn: Score.t,
  finesse: Score.t,
  resolve: Score.t,
  wits: Score.t,
  panache: Score.t,
};

let init = () =>
  Score.{
    brawn: fromInt(2) |> unwrap,
    finesse: fromInt(2) |> unwrap,
    resolve: fromInt(2) |> unwrap,
    wits: fromInt(2) |> unwrap,
    panache: fromInt(2) |> unwrap,
  };