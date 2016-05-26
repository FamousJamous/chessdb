#ifndef CHESSDB_COORD_H
#define CHESSDB_COORD_H
namespace Chessdb {

struct Coord {
  Coord(char file = 0, int rank = 0): file(file), rank(rank) {
  }
  char file;
  int rank;
};

} // Chessdb
#endif
