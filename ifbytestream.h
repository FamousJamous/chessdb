#ifndef I_F_BYTE_STREAM_H
#define I_F_BYTE_STREAM_H
#include <fstream>
#include "bytestream.h"
namespace jl {

class IFByteStream: public ByteStream {
 public:
  IFByteStream(std::string const & fileName);
  char get();
  void putBack(char c);
  bool good() const;
 private:
  std::ifstream ifs;
};

} // namespace jl
#endif
