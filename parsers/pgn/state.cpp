#include "state.h"
#include "coord.h"
#include "move.h"
namespace Chessdb {

namespace {

char makePiece(char p, State::Color color) {
  //return State::WHITE == color ? p : p - ('a' - 'A');
  return State::WHITE == color ? p + ('a' - 'A'): p;
}

void setPawns(int row, State::Color color, State::Board * board) {
  for (int col = 0; col < 8; ++col) {
    (*board)[row][col] = makePiece('P', color);
  }
}

void setBackRow(int row, State::Color color, State::Board * board) {
  (*board)[row][0] = makePiece('R', color);
  (*board)[row][1] = makePiece('N', color);
  (*board)[row][2] = makePiece('B', color);
  (*board)[row][3] = makePiece('Q', color);
  (*board)[row][4] = makePiece('K', color);
  (*board)[row][5] = makePiece('B', color);
  (*board)[row][6] = makePiece('N', color);
  (*board)[row][7] = makePiece('R', color);
}

void setEmpty(int row, State::Board * board) {
  for (int col = 0; col < 8; ++col) {
    (*board)[row][col] = ' ';
  }
}

char & getPieceRef(Coord const & coord, State::Board * board) {
  return (*board)[coord.rank - 1][coord.file - 'a'];
}

void movePiece(FromTo const & fromTo, State::Board * board) {
  Coord const & from = fromTo.from;
  Coord const & to = fromTo.to;
  getPieceRef(to, board) = getPieceRef(from, board);
  getPieceRef(from, board) = ' ';
}

void copyBoard(State::Board const & board, State::Board * copy) {
  for (int row = 0; row < 8; ++row) {
    for (int col = 0; col < 8; ++col) {
      (*copy)[row][col] = board[row][col];
    }
  }
}

} // namespace

State::State(): turn(WHITE) {
  setBackRow(0, WHITE, &board);
  setPawns(1, WHITE, &board);
  for (int row = 2; row <= 5; ++row) {
    setEmpty(row, &board);
  }
  setPawns(6, BLACK, &board);
  setBackRow(7, BLACK, &board);
}

State::State(Color const turnIn, Board const & boardIn): turn(turnIn) {
  copyBoard(boardIn, &board);
}

State::State(State const & other): turn(other.turn) {
  copyBoard(other.board, &board);
}

char State::getPiece(Coord const & coord) const {
  return board[coord.rank - 1][coord.file - 'a'];
}

void State::makeMove(Move const & move) {
  FromTo const & fromTo = move.fromTo;
  movePiece(fromTo, &board);
  if (move.castle.from.file) {
    movePiece(move.castle, &board);
  }
  if (move.enPassCap.file) {
    getPieceRef(move.enPassCap, &board) = ' ';
  }
  if (move.promPiece) {
    getPieceRef(fromTo.to, &board) = makePiece(move.promPiece, turn);
  }
  turn = (State::WHITE == turn) ? State::BLACK : State::WHITE;
}

} // namespace Chessdb
