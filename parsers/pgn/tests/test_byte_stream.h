#ifndef CHESSDB_TEST_BYTE_STREAM_H
#define CHESSDB_TEST_BYTE_STREAM_H
#include "byte_stream.h"
#include <string>
namespace Chessdb {

class TestByteStream: public ByteStream {
 public:
  TestByteStream(std::string const & str);
  char get();
  void putBack(char c);
  bool good() const;
 private:
  std::string str;
  size_t i;
};

} // namespace Chessdb
#endif
