module type NationT = {
  type t;

  let make: t => result(t, exn);
};

module Avalon: NationT;
module Inismore: NationT;
module TheHighlandMarches: NationT;
module Castille: NationT;
module Eisen: NationT;
module Sarmatia: NationT;
module Montaigne: NationT;
module Ussura: NationT;
module Vesten: NationT;
module Vodacce: NationT;

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