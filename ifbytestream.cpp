#include "ifbytestream.h"
namespace jl {

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

} // namespace jl
