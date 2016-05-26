#include "san_iter.h"
#include <cctype>
#include <stdexcept>
#include "byte_stream.h"
namespace Chessdb {

namespace {

struct State {
  enum Value {
    NONE, NUM, BETWEEN, SAN
  };
};

} // namespace

SanIter::SanIter(ByteStream & stream): stream(stream) {
  ++(*this);
}

SanIter::operator bool () const {
  return !san.empty();
}

void SanIter::operator++() {
  State::Value state = State::NONE;
  san.clear();
  while (stream.good()) {
    char c = stream.get();
    if ('[' == c) {
      stream.putBack(c);
      return;
    }
    if (std::isalnum(c) || '/' == c || '-' == c || '+' == c || '.' == c) {
      san.push_back(c);
    }
    switch (state) {
      case State::NONE:
        if (std::isdigit(c)) {
          state = State::NUM;
        } else if (std::isalpha(c)) {
          state = State::SAN;
        }
        break;
      case State::NUM:
        if ('.' == c) {
          san.clear();
          state = State::BETWEEN;
        } else if ('/' == c || '-' == c) {
          state = State::SAN;
        }
        break;
      case State::BETWEEN:
        if (!std::isspace(c)) {
          state = State::SAN;
        }
        break;
      case State::SAN:
        if (std::isspace(c)) {
          return;
        }
        break;
      default:
        throw std::runtime_error("unexpected SanIter state");
    }
  }
}

std::string const & SanIter::operator*() const {
  return san;
}

} // namespace Chessdb
