#include "parseround.h"
#include <cassert>
#include <cctype>
#include <iostream>
#include "bytestream.h"
namespace jl {

namespace {

struct RoundState {
  enum Value {
    NUM_FIND, NUM_PARSE, DOT, SAN, /*SAN_FROM, SAN_MIDDLE, SAN_TO, SAN_DONE, */COMMENT
  };
};

std::ostream & operator<<(std::ostream & os, RoundState::Value const & state) {
  switch (state) {
    case RoundState::NUM_FIND:
      return os << "NUM_FIND";
    case RoundState::NUM_PARSE:
      return os << "NUM_PARSE";
    case RoundState::DOT:
      return os << "DOT";
    case RoundState::SAN:
      return os << "SAN";
      /*
    case RoundState::SAN_FROM:
      return os << "SAN_FROM";
    case RoundState::SAN_MIDDLE:
      return os << "SAN_MIDDLE";
    case RoundState::SAN_TO:
      return os << "SAN_TO";
    case RoundState::SAN_DONE:
      return os << "SAN_DONE";
      */
    case RoundState::COMMENT:
      return os << "COMMENT";
    default:
      assert(0);
  }
  return os;
}

std::string strip(std::string const & str) {
  int start = 0;
  while (std::isspace(str[start])) ++start;
  int stop = str.size() - 1;
  while (std::isspace(str[stop])) --stop;
  // 0 1 2 3 4
  //   a b c
  // start = 1
  // stop = 3
  return str.substr(start, stop - start + 1);
}

void putBackNum(ByteStream & stream, std::string const & san) {
  int i = san.size() - 1;
  while (0 <= i && std::isdigit(san[i])) {
    stream.putBack(san[i]);
    --i;
  }
}

void removeNum(std::string * san) {
  int i = san->size() - 1;
  while (0 <= i && std::isdigit((*san)[i])) {
    --i;
  }
  san->erase(i);
}

} // namespace

Round parseRound(ByteStream & stream) {
  RoundState::Value state = RoundState::NUM_FIND;
  int num = 0;
  std::string san;
  std::string comment;
  while (stream.good()) {
    char c = stream.get();
    switch (state) {
      case RoundState::NUM_FIND:
        if (std::isdigit(c)) {
          stream.putBack(c);
          state = RoundState::NUM_PARSE;
        }
        break;
      case RoundState::NUM_PARSE:
        if (std::isdigit(c)) {
          num = c - '0' + num * 10;
        } else {
          state = RoundState::DOT;
        }
        break;
      case RoundState::DOT:
        if (!std::isspace(c)) {
          stream.putBack(c);
          //state = RoundState::SAN_FROM;
          state = RoundState::SAN;
        }
        break;
      case RoundState::SAN:
        if ('[' == c) {
          stream.putBack(c);
          return Round(num, strip(san), comment);
        } else if ('{' == c) {
          state = RoundState::COMMENT;
        } else if ('.' == c) {
          stream.putBack(c);
          putBackNum(stream, san);
          removeNum(&san);
          return Round(num, strip(san), comment);
        } else {
          san.push_back(c);
        }
        break;
        /*
      case RoundState::SAN_FROM:
        san.push_back(c);
        if (std::isspace(c)) {
          state = RoundState::SAN_MIDDLE;
        }
        break;
      case RoundState::SAN_MIDDLE:
        san.push_back(c);
        if (!std::isspace(c)) {
          state = RoundState::SAN_TO;
        }
        break;
      case RoundState::SAN_TO:
        if ('{' == c) {
          state = RoundState::COMMENT;
        } else if (std::isspace(c)) {
          state = RoundState::SAN_DONE;
        } else {
          san.push_back(c);
        }
        break;
      case RoundState::SAN_DONE:
        if ('[' == c || std::isdigit(c)) {
          stream.putBack(c);
          return Round(num, strip(san), comment);
        } else if ('{' == c) {
          state = RoundState::COMMENT;
        }
        break;
        */
      case RoundState::COMMENT:
        if ('}' != c) {
          comment.push_back(c);
        } else {
          return Round(num, strip(san), comment);
        }
        break;
      default:
        assert(0);
    }
  }
  assert(0);
  return Round(num, san, comment);
}

} // namespace jl
