#include "if_byte_stream.h"
namespace Chessdb {

IFByteStream::IFByteStream(std::string const & fileName):
  ifs(fileName.c_str()) {
}

char IFByteStream::get() {
  return ifs.get();
}

void IFByteStream::putBack(char c) {
  ifs.putback(c);
}

bool IFByteStream::good() const {
  return ifs.good();
}

} // namespace Chessdb
