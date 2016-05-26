#ifndef CHESSDB_STATE_H
#define CHESSDB_STATE_H
namespace Chessdb {

class Coord;
class Move;

class State {
 public:
  enum Color {
    WHITE, BLACK
  };
  typedef char Board[8][8];
  State();
  State(Color const turn, Board const & board);
  State(State const & other);
  Color getTurn() const {
    return turn;
  }
  char getPiece(Coord const & coord) const;
  void makeMove(Move const & move);
 private:
  Color turn;
  Board board;
};

} // namespace Chessdb
#endif
