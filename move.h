#ifndef MOVE_H
#define MOVE_H
namespace jl {

struct Move {
  Move(char piece, char file, int rank, bool capture, char fromFile = 0,
       int fromRank = 0, bool enPassant = false, char promoteTo = 0);
  // P, R, N, B, Q, K
  char piece;
  // a -> h
  char file;
  // 1 -> 8
  int rank;
  bool capture;
  char fromFile;
  int fromRank;
  bool enPassant;
  char promoteTo;
  bool kingCastle;
  bool queenCastle;
  bool check;
  bool checkMate;
};

} // namespace jl
#endif
