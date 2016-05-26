#include "game_iter.h"
#include <sstream>
#include <stdexcept>
#include "byte_stream.h"
#include "parse_san.h"
#include "infer_move.h"
#include "move.h"
#include "san_iter.h"
#include "state.h"
#include "tag_iter.h"
namespace Chessdb {

namespace {

void iterateTags(PgnIter * pgnIt, Game::Tags * tags) {
  tags->clear();
  for (TagIter it = pgnIt->getTagIt(); it; ++it) {
    tags->insert(std::make_pair(it.getKey(), it.getValue()));
  }
}

bool getOutcome(SanProps::Type const type, Game::Outcome * outcome) {
  if (SanProps::DRAW == type) {
    *outcome = Game::DRAW;
    return true;
  } else if (SanProps::WHITE_WINS == type) {
    *outcome = Game::WHITE_WINS;
    return true;
  } else if (SanProps::BLACK_WINS == type) {
    *outcome = Game::BLACK_WINS;
    return true;
  }
  return false;
}

void iterateSans(PgnIter * pgnIt, Game::Moves * moves, Game::Outcome * outcome) {
  State state;
  moves->clear();
  for (SanIter it = pgnIt->getSanIt(); it; ++it) {
    SanProps sanProps = Chessdb::parseSan(*it);
    if (getOutcome(sanProps.type, outcome)) {
      (*moves)[moves->size() - 1].sans += " " + *it;
      break;
    }
    Move move;
    move.sans = *it;
    inferMove(sanProps, state, &move);
    state.makeMove(move);
    moves->push_back(move);
  }
}

std::string getTag(Game const & game, std::string const & name) {
  return game.tags.find(name)->second;
}

} // namespace

GameIter::GameIter(ByteStream & stream): pgnIt(stream) {
  ++(*this);
}

GameIter::operator bool () const {
  return pgnIt;
}

void GameIter::operator++() {
  iterateTags(&pgnIt, &game.tags);
  try {
    iterateSans(&pgnIt, &game.moves, &game.outcome);
  } catch (std::runtime_error const & err) {
    std::ostringstream errWithId;
    errWithId << err.what() << " for event " << getTag(game, "Event")
              << " round " << getTag(game, "Round");
    throw std::runtime_error(errWithId.str());
  }
  ++pgnIt;
}

Game const & GameIter::operator*() const {
  return game;
}

} // namespace Chessdb
