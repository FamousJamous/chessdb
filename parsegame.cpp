#include "parsegame.h"
#include <cassert>
#include <cctype>
#include <iostream>
#include "bytestream.h"
#include "round.h"
#include "parseround.h"
#include "parsetagpair.h"
#include "tagpair.h"
namespace jl {

namespace {

struct GameState {
  enum Value {
    TAG_PAIRS, ROUNDS
  };
};

std::ostream & operator<<(std::ostream & os, GameState::Value const & state) {
  switch (state) {
    case GameState::TAG_PAIRS:
      return os << "TAG_PAIRS";
    case GameState::ROUNDS:
      return os << "ROUNDS";
    default:
      assert(0);
  }
  return os;
}

} // namespace

Game parseGame(ByteStream & stream) {
  Game game;
  std::map<std::string, TagPair> & tagPairs = game.tagPairs;
  std::vector<Round> & rounds = game.rounds;
  GameState::Value state = GameState::TAG_PAIRS;
  while (stream.good()) {
    char c = stream.get();
    switch (state) {
      case GameState::TAG_PAIRS:
        if ('[' == c) {
          stream.putBack(c);
          TagPair tagPair = parseTagPair(stream);
          tagPairs.insert(make_pair(tagPair.key, tagPair));
        } else if (std::isdigit(c)) {
          stream.putBack(c);
          state = GameState::ROUNDS;
        }
        break;
      case GameState::ROUNDS:
        if (std::isdigit(c)) {
          stream.putBack(c);
          rounds.push_back(parseRound(stream));
        } else if ('[' == c) {
          stream.putBack(c);
          return game;
        }
        break;
      default:
        assert(0);
    }
  }
  return game;
}

} // namespace jl
