#ifndef CHESSDB_MOVE_H
#define CHESSDB_MOVE_H
#include <string>
#include <vector>
#include "coord.h"
namespace Chessdb {

struct FromTo {
  FromTo(Coord const & from = Coord(), Coord const & to = Coord()):
    from(from), to(to) {
  }
  Coord from;
  Coord to;
};

struct Move {
  Move(): check(false), promPiece(0) {
  }
  std::string sans;
  FromTo fromTo;
  // rook movement for castle (invalid coordinates if not castle)
  FromTo castle;
  bool check;
  // coordinate of piece that's captured by en passant (invalid coordinate if not en passant)
  Coord enPassCap;
  // 'R', 'N', 'B', 'Q'
  char promPiece;
};

} // namespace Chessdb
#endif
