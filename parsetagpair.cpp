#include "parsetagpair.h"
#include <cassert>
#include <iostream>
#include "bytestream.h"
namespace jl {

namespace {

struct TagPairState {
  enum Value {
    BRACE_OPEN, KEY, KEY_ESCAPED, QUOTE_OPEN, VALUE, VALUE_ESCAPED, BRACE_CLOSE
  };
};

std::ostream & operator<<(std::ostream & os, TagPairState::Value const & state) {
  switch (state) {
    case TagPairState::BRACE_OPEN:
      return os << "BRACE_OPEN";
    case TagPairState::KEY:
      return os << "KEY";
    case TagPairState::KEY_ESCAPED:
      return os << "KEY_ESCAPED";
    case TagPairState::QUOTE_OPEN:
      return os << "QUOTE_OPEN";
    case TagPairState::VALUE:
      return os << "VALUE";
    case TagPairState::VALUE_ESCAPED:
      return os << "VALUE_ESCAPED";
    case TagPairState::BRACE_CLOSE:
      return os << "BRACE_CLOSE";
    default:
      assert(0);
  }
  return os;
}

} // namespace

TagPair parseTagPair(ByteStream & stream) {
  TagPairState::Value state = TagPairState::BRACE_OPEN;
  std::string key;
  std::string value;
  while (stream.good()) {
    char c = stream.get();
    switch (state) {
      case TagPairState::BRACE_OPEN:
        if ('[' == c) {
          state = TagPairState::KEY;
        }
        break;
      case TagPairState::KEY:
        if (!std::isspace(c)) {
          key.push_back(c);
        } else if ('\\' == c) {
          state = TagPairState::KEY_ESCAPED;
        } else {
          state = TagPairState::QUOTE_OPEN;
        }
        break;
      case TagPairState::KEY_ESCAPED:
        key.push_back(c);
        state = TagPairState::KEY;
        break;
      case TagPairState::QUOTE_OPEN:
        if ('"' == c) {
          state = TagPairState::VALUE;
        }
        break;
      case TagPairState::VALUE:
        if ('"' != c) {
          value.push_back(c);
        } else if ('\\' == c) {
          state = TagPairState::VALUE_ESCAPED;
        } else {
          state = TagPairState::BRACE_CLOSE;
        }
        break;
      case TagPairState::VALUE_ESCAPED:
        value.push_back(c);
        state = TagPairState::VALUE;
        break;
      case TagPairState::BRACE_CLOSE:
        if (']' == c) {
          return TagPair(key, value);
        }
        break;
      default:
        assert(0);
    }
  }
  assert(0);
  return TagPair(key, value);
}

} // namespace jl
