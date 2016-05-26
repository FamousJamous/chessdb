#ifndef CHESSDB_SAN_PROPS_H
#define CHESSDB_SAN_PROPS_H
namespace Chessdb {

struct SanProps {
  SanProps(): 
    type(MOVE_PIECE),
    piece(PAWN),
    file(),
    rank(),
    isCapture(),
    isEnPassant(),
    fromFile(),
    fromRank(),
    isPromotion(),
    promoteTo(PAWN),
    isCheck(),
    isCheckmate() {
  }
  enum Type {
    MOVE_PIECE, KING_CASTLE, QUEEN_CASTLE, DRAW, WHITE_WINS, BLACK_WINS
  };
  enum Piece {
    PAWN, ROOK, KNIGHT, BISHOP, QUEEN, KING
  };
  Type type;
  Piece piece;
  char file;
  int rank;
  bool isCapture;
  bool isEnPassant;
  char fromFile;
  int fromRank;
  bool isPromotion;
  Piece promoteTo;
  bool isCheck;
  bool isCheckmate;
};

} // namespace Chessdb
#endif
