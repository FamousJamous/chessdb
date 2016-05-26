#ifndef CHESSDB_GAME_ITER_H
#define CHESSDB_GAME_ITER_H
#include "game.h";
#include "pgn_iter.h"
namespace Chessdb {

class ByteStream;

class GameIter {
 public:
  GameIter(ByteStream & stream);
  operator bool () const;
  void operator++();
  Game const & operator*() const;
 private:
  PgnIter pgnIt;
  Game game;
};

} // namespace Chessdb
#endif
