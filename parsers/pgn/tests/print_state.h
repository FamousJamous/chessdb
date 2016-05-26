#ifndef CHESSDB_PRINT_STATE_H
#define CHESSDB_PRINT_STATE_H
namespace Chessdb {

class State;

struct BoardSide {
  enum Value { TURN, WHITE, BLACK };
};

void printState(State const & state,
                BoardSide::Value const boardSide = BoardSide::TURN);

void printBoard(State const & state,
                BoardSide::Value const boardSide = BoardSide::TURN);

void printTurn(State const & state);

} // namespace Chessdb
#endif
