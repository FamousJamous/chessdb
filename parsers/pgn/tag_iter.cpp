#include "tag_iter.h"

#include <cctype>
#include <sstream>
#include <stdexcept>
#include "byte_stream.h"
namespace Chessdb {

namespace {

struct State {
  enum Value {
    NONE, KEY, BETWEEN, VALUE, END
  };
};

} // namespace

TagIter::TagIter(ByteStream & stream): stream(stream) {
  ++(*this);
}

TagIter::operator bool () const {
  return !key.empty();
}

void TagIter::operator++() {
  State::Value state = State::NONE;
  key.clear();
  value.clear();
  while (stream.good()) {
    char c = stream.get();
    switch (state) {
      case State::NONE:
        if (std::isspace(c)) {
          continue;
        }
        if ('[' != c) {
          stream.putBack(c);
          return;
        }
        state = State::KEY;
        break;
      case State::KEY:
        if (std::isspace(c)) {
          state = State::BETWEEN;
        } else {
          key.push_back(c);
        }
        break;
      case State::BETWEEN:
        if (std::isspace(c)) {
          continue;
        }
        if ('"' != c) {
          std::ostringstream err;
          err << "unexpected char '" << c << "' in tag between key and value";
          throw std::runtime_error(err.str());
        }
        state = State::VALUE;
        break;
      case State::VALUE:
        if ('"' == c) {
          state = State::END;
          continue;
        }
        value.push_back(c);
        break;
      case State::END:
        if (std::isspace(c)) {
          continue;
        }
        if (']' != c) {
          std::ostringstream err;
          err << "unexpected char '" << c << "'in tag after value";
          throw std::runtime_error(err.str());
        }
        return;
    }
  }
}

std::string const & TagIter::getKey() const {
  return key;
}

std::string const & TagIter::getValue() const {
  return value;
}

} // namespace Chessdb
