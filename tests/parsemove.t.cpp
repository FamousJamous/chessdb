#include "parsemove.t.h"
#include "parsemove.h"
#include <cassert>
#include <iostream>
#include <string>
#include "expmove.h"
#include "testbytestream.h"
namespace jl {

namespace {

void test(std::string const & str, char piece, char file, int rank,
          bool capture = false, char fromFile = 0, int fromRank = 0,
          bool enPassant = false, char promoteTo = 0) {
  std::cout << str << std::endl;
  TestByteStream tbs(str);
  Move move = parseMove(tbs);
  Move expMove(piece, file, rank, capture, fromFile, fromRank, enPassant,
               promoteTo);
  checkExpMove(move, expMove);
}

} // namespace

void testParseMove() {
  test("Be5", 'B', 'e', 5);
  test("Nf3", 'N', 'f', 3);
  test("c5", 'P', 'c', 5);
  test("Bxe5", 'B', 'e', 5, true);
  test("B:e5", 'B', 'e', 5, true);
  test("Be5:", 'B', 'e', 5, true);
  test("exd5:", 'P', 'd', 5, true, 'e');
  test("exd6e.p.", 'P', 'd', 6, true, 'e', 0, true);
  test("e8=Q.p.", 'P', 'e', 8, false, 0, 0, false, 'Q');
}

}
