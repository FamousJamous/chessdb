#include "parsemove.h"
#include <iostream>
#include "bytestream.h"
namespace jl {

namespace {

bool isPiece(char c) {
  return 'R' == c || 'N' == c || 'B' == c || 'Q' == c || 'K' == c;
}

bool isEndCapture(char c) {
  return ':' == c;
}

void putBackStr(int i, std::string const & str, ByteStream * stream) {
  for (; 0 <= i; --i) {
    stream->putBack(str[i]);
  }
}

// If the beginning of stream matches str, consumes that portion of the stream
// and returns true. Leave stream untouched and return false otherwise.
bool matchStr(std::string const & str, ByteStream * stream) {
  for (int i = 0, n = str.size(); i < n; ++i) {
    if (!stream->good()) {
      putBackStr(i - 1, str, stream);
      return false;
    }
    char c = stream->get();
    if (str[i] != c) {
      stream->putBack(c);
      putBackStr(i - 1, str, stream);
      return false;
    }
  }
  return true;
}

char parsePiece(ByteStream & stream) {
  char c = stream.get();
  if ('S' == c) { // German knight
    return 'N';
  } else if ('F' == c) { // French bishop
    return 'B';
  } else if (isPiece(c)) {
    return c;
  } else {
    stream.putBack(c);
    return 'P';
  }
}

bool parseMidCapture(ByteStream & stream) {
  return matchStr("x", &stream) ||
         matchStr(":", &stream);
}

void parseFiles(ByteStream & stream, char * file, char * fromFile, bool * capture) {
  *file = stream.get();
  if (parseMidCapture(stream)) {
    *fromFile = *file;
    *file = stream.get();
    *capture = true;
  }
}

int parseRank(ByteStream & stream) {
  return stream.get() - '0';
}

char parsePromoteTo(ByteStream & stream) {
  if (matchStr("=", &stream)) {
    return parsePiece(stream);
  }
  return 0;
}

void parseEndCapture(ByteStream & stream, bool * capture) {
  if (matchStr(":", &stream)) {
    *capture = true;
  }
}

bool parseEnPassant(ByteStream & stream) {
  return matchStr("e.p.", &stream);
}

} // namespace

Move parseMove(ByteStream & stream) {
  char piece = parsePiece(stream);
  bool capture = parseMidCapture(stream);
  char fromFile = 0;
  char file = 0;
  parseFiles(stream, &file, &fromFile, &capture);
  int rank = parseRank(stream);
  char promoteTo = parsePromoteTo(stream);
  parseEndCapture(stream, &capture);
  int fromRank = 0;
  bool enPassant = parseEnPassant(stream);
  return Move(piece, file, rank, capture, fromFile, fromRank, enPassant,
              promoteTo);
}

} // namespace jl
