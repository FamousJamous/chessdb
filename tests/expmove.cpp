#include "expmove.h"
#include <cassert>
#include <iostream>
#include "move.h"
#include "testutils.h"
namespace jl {

void checkExpMove(Move const & move, Move const & expMove) {
  if (move.piece != expMove.piece) {
    std::cerr << "piece mismatch!" << std::endl
              << "move " << move << std::endl
              << "exp " << expMove << std::endl;
    assert(0);
  }
  if (move.file != expMove.file) {
    std::cerr << "file mismatch!" << std::endl
              << "move " << move << std::endl
              << "exp " << expMove << std::endl;
    assert(0);
  }
  if (move.rank != expMove.rank) {
    std::cerr << "rank mismatch!" << std::endl
              << "move " << move << std::endl
              << "exp " << expMove << std::endl;
    assert(0);
  }
  if (move.capture != expMove.capture) {
    std::cerr << "capture mismatch!" << std::endl
              << "move " << move << std::endl
              << "exp " << expMove << std::endl;
    assert(0);
  }
  if (move.fromFile != expMove.fromFile) {
    std::cerr << "fromFile mismatch!" << std::endl
              << "move " << move << std::endl
              << "exp " << expMove << std::endl;
    assert(0);
  }
  if (move.fromRank != expMove.fromRank) {
    std::cerr << "fromRank mismatch!" << std::endl
              << "move " << move << std::endl
              << "exp " << expMove << std::endl;
    assert(0);
  }
  if (move.enPassant != expMove.enPassant) {
    std::cerr << "enPassant mismatch!" << std::endl
              << "move " << move << std::endl
              << "exp " << expMove << std::endl;
    assert(0);
  }
  if (move.promoteTo != expMove.promoteTo) {
    std::cerr << "promoteTo mismatch!" << std::endl
              << "move " << move << std::endl
              << "exp " << expMove << std::endl;
    assert(0);
  }
}

} // namespace jl
