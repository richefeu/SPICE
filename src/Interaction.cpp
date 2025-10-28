#include "Interaction.hpp"

Interaction::Interaction() : i(0), j(0) {}

Interaction::Interaction(size_t I, size_t J) : i(I), j(J) {}

void Interaction::copy(Interaction &I) {
  isBonded           = I.isBonded;
  isSameMaterialBond = I.isSameMaterialBond;

  fn  = I.fn;
  fnb = I.fnb;
  ft  = I.ft;
  ftb = I.ftb;

  meff = I.meff;
  kn   = I.kn;
  kt   = I.kt;
  mu   = I.mu;
  muR  = I.muR;
  fadh = I.fadh;

  damp = I.damp;
  Gs   = I.Gs;
  dn0  = I.dn0;
}