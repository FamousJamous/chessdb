#include "parse_san.h"
#include <sstream>
#include <stdexcept>
namespace Chessdb {

namespace {

bool matchBeginning(std::string const & str, std::string const & begins,
                    size_t * i) {
  if (begins == str.substr(0, begins.size())) {
    *i += begins.size();
    return true;
  }
  return false;
}

SanProps::Type parseType(std::string const & str, size_t * i) {
  if (matchBeginning(str, "O-O-O", i)) {
    return SanProps::QUEEN_CASTLE;
  }
  if (matchBeginning(str, "O-O", i)) {
    return SanProps::KING_CASTLE;
  }
  if (matchBeginning(str, "1/2-1/2", i)) {
    return SanProps::DRAW;
  }
  if (matchBeginning(str, "1-0", i)) {
    return SanProps::WHITE_WINS;
  }
  if (matchBeginning(str, "0-1", i)) {
    return SanProps::BLACK_WINS;
  }
  return SanProps::MOVE_PIECE;
}

void checkLen(std::string const & str, size_t i) {
  if (str.size() <= i) {
    std::ostringstream err;
    err << "while parsing " << str << " unexpected end of string at char " << i;
    throw std::runtime_error(err.str());
  }
}

SanProps::Piece parsePiece(std::string const & str, size_t * i) {
  checkLen(str, *i);
  switch (str[*i]) {
    case 'R':
      ++(*i);
      return SanProps::ROOK;
    case 'N':
      ++(*i);
      return SanProps::KNIGHT;
    case 'B':
      ++(*i);
      return SanProps::BISHOP;
    case 'Q':
      ++(*i);
      return SanProps::QUEEN;
    case 'K':
      ++(*i);
      return SanProps::KING;
    default:
      return SanProps::PAWN;
  }
}

bool parseMidCapture(std::string const & str, size_t * i) {
  checkLen(str, *i);
  if ('x' == str[*i] || ':' == str[*i]) {
    ++(*i);
    return true;
  }
  return false;
}

char parseFile(std::string const & str, size_t * i) {
  checkLen(str, *i);
  char file = str[*i];
  if ('a' <= file && file <= 'h') {
    ++(*i);
    return file;
  }
  return 0;
}

char parseMidFile(std::string const & str, size_t * i) {
  checkLen(str, *i);
  char file = str[*i];
  if ('a' <= file && file <= 'h') {
    ++(*i);
    return file;
  }
  return 0;
}

int parseRank(std::string const & str, size_t * i) {
  checkLen(str, *i);
  int rank = str[(*i)++] - '0';
  if (rank < 1 || 8 < rank) {
    std::ostringstream err;
    err << "while parsing " << str << " unexpected rank " << rank;
    throw std::runtime_error(err.str());
  }
  return rank;
}

bool parseEndCapture(std::string const & str, size_t * i) {
  if (str.size() == *i) {
    return false;
  }
  if (':' == str[*i]) {
    ++(*i);
    return true;
  }
  return false;
}

bool parseEnPassant(std::string const & str, size_t * i) {
  if (str.size() == *i) {
    return false;
  }
  if ("e.p." == str.substr(*i)) {
    (*i) += 4;
    return true;
  }
  return false;
}

bool parsePromotion(std::string const & str, SanProps::Piece * promoteTo,
                    size_t * i) {
  if (str.size() <= *i) {
    return false;
  }
  if ('=' == str[*i]) {
    ++(*i);
    *promoteTo = parsePiece(str, i);
    return true;
  }
  *promoteTo = parsePiece(str, i);
  return SanProps::PAWN != *promoteTo;
}

bool parseCheck(std::string const & str, size_t * i) {
  if (str.size() <= *i) {
    return false;
  }
  if ('+' == str[*i]) {
    ++(*i);
    return true;
  }
  return false;
}

bool parseCheckmate(std::string const & str, size_t * i) {
  if (str.size() <= *i) {
    return false;
  }
  if ('#' == str[*i]) {
    ++(*i);
    return true;
  }
  return false;
}

} // namespace

SanProps parseSan(std::string const & str) {
  SanProps san;
  size_t i = 0;
  san.type = parseType(str, &i);
  if (SanProps::MOVE_PIECE != san.type) {
    san.isCheck = parseCheck(str, &i);
    san.isCheckmate = parseCheckmate(str, &i);
    return san;
  }
  san.piece = parsePiece(str, &i);
  san.isCapture = parseMidCapture(str, &i);
  char file = parseFile(str, &i);
  if (file) {
    san.file = file;
  } else {
    san.fromRank = parseRank(str, &i);
  }
  san.isCapture = san.isCapture || parseMidCapture(str, &i);
  file = parseMidFile(str, &i);
  if (file) {
    san.fromFile = san.file;
    san.file = file;
  }
  san.rank = parseRank(str, &i);
  san.isCapture = san.isCapture || parseEndCapture(str, &i);
  san.isEnPassant = parseEnPassant(str, &i);
  SanProps::Piece promoteTo;
  bool isPromotion = parsePromotion(str, &promoteTo, &i);
  if (isPromotion) {
    san.isPromotion = true;
    san.promoteTo = promoteTo;
  }
  san.isCheck = parseCheck(str, &i);
  san.isCheckmate = parseCheckmate(str, &i);
  return san;
}

} // namespace Chessdb
