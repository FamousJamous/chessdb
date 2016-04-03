#include "move.h"
#include <cassert>
namespace jl {

namespace {

void checkFile(char file) {
  assert('a' <= file && file <= 'h');
}

void checkRank(int rank) {
  assert(1 <= rank && rank <= 8);
}


} // namespace

Move::Move(char piece, char file, int rank, bool capture,
           char fromFile, int fromRank, bool enPassant, char promoteTo):
  piece(piece), file(file), rank(rank), capture(capture), fromFile(fromFile),
  fromRank(fromRank), enPassant(enPassant), promoteTo(promoteTo)
{
  /*
  assert('K' == piece || 'Q' == piece || 'R' == piece || 'B' == piece ||
         'N' == piece || 'P' == piece);
  checkFile(file);
  checkRank(rank);
  if (0 != fromFile) {
    checkFile(fromFile);
  }
  if (0 != fromRank) {
    checkRank(fromRank);
  }
  */
}

} // namespace jl
