#include "pgn_iter.h"
#include "byte_stream.h"
namespace Chessdb {

PgnIter::PgnIter(ByteStream & stream): stream(stream) {
  ++(*this);
}

PgnIter::operator bool () const {
  return stream.good();
}

void PgnIter::operator++() {
  while (stream.good()) {
    char c = stream.get();
    if (!std::isspace(c)) {
      stream.putBack(c);
      break;
    }
  }
}

TagIter PgnIter::getTagIt() {
  return TagIter(stream);
}

SanIter PgnIter::getSanIt() {
  return SanIter(stream);
}

} // namespace Chessdb
