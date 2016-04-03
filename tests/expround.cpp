#include "expround.h"
#include <cassert>
#include <iostream>
#include "round.h"
namespace jl {

void checkExpRound(Round const & round, ExpRound const & expRound) {
  if (expRound.num != round.num || expRound.san != round.san ||
      expRound.comment != round.comment) {
    std::cerr << "round.num " << round.num
              << " round.san " << round.san
              << " round.comment " << round.comment
              << " expRound.num " << expRound.num
              << " expRound.san " << expRound.san
              << " expRound.comment " << expRound.comment
              << std::endl;
    assert(0);
  }
}

} // namespace jl
