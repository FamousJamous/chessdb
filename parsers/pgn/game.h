#ifndef CHESSDB_GAME_H
#define CHESSDB_GAME_H
#include <map>
#include <vector>
#include "move.h"
namespace Chessdb {

struct Game {
  Game(): outcome(DRAW) {
  }
  typedef std::map<std::string, std::string> Tags;
  Tags tags;
  typedef std::vector<Move> Moves;
  Moves moves;
  enum Outcome { DRAW, WHITE_WINS, BLACK_WINS };
  Outcome outcome;
};

} // namespace Chessdb
#endif
