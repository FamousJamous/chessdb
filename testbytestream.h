#ifndef TEST_BYTE_STREAM_H
#define TEST_BYTE_STREAM_H
#include "bytestream.h"
#include <string>
namespace jl {

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

} // namespace jl
#endif
