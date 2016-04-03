#include "testbytestream.h"
namespace jl {

TestByteStream::TestByteStream(std::string const & str): str(str), i(0) {
}
char TestByteStream::get() {
  if (i < str.size()) {
    return str[i++];
  } else if (i == str.size()) {
    ++i;
  }
  return -1;
}
void TestByteStream::putBack(char c) {
  str[--i] = c;
}
bool TestByteStream::good() const {
  return str.size() >= i;
}

} // namespace jl
