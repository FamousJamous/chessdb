#ifndef CHESSDB_PGN_ITER_H
#define CHESSDB_PGN_ITER_H
#include "san_iter.h"
#include "tag_iter.h"
namespace Chessdb {

class ByteStream;

class PgnIter {
 public:
  PgnIter(ByteStream & stream);
  operator bool () const;
  void operator++();
  TagIter getTagIt();
  SanIter getSanIt();
 private:
  ByteStream & stream;
};

} // namespace Chessdb
#endif
