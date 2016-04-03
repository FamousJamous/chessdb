#ifndef GAME_H
#define GAME_H
#include <map>
#include <vector>
#include "tagpair.h"
#include "round.h"
namespace jl {

struct Game {
  std::map<std::string, TagPair> tagPairs;
  std::vector<Round> rounds;
};

} // namespace jl
#endif
