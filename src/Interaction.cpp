#include "Interaction.hpp"

Interaction::Interaction() : i(0), j(0), bond(), solidbond(),  fn(0.0), fnb(0.0), ft{0.0}, ftb(0.0), damp(0.0), Gs{0.0}, dn0{0.0} {}

Interaction::Interaction(size_t I, size_t J) : i(I), j(J), bond(false), solidbond(false), 
    fn(0.0), fnb{0.0}, ft(0.0), ftb{0.0}, damp(0.0), Gs{0.0}, dn0{0.0}  {}
