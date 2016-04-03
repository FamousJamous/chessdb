#ifndef EXP_ROUND_H
#define EXP_ROUND_H
#include "round.h"
namespace jl {

struct ExpRound {
  int num;
  char const * san;
  char const * comment;
};

class Round;
void checkExpRound(Round const & round, ExpRound const & expRound);

} // namespace jl
#endif
