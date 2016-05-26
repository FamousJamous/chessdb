#ifndef CHESSDB_IF_BYTE_STREAM_H
#define CHESSDB_IF_BYTE_STREAM_H
#include <fstream>
#include "byte_stream.h"
namespace Chessdb {

class IFByteStream: public ByteStream {
 public:
  IFByteStream(std::string const & fileName);
  char get();
  void putBack(char c);
  bool good() const;
 private:
  std::ifstream ifs;
};

} // namespace Chessdb
#endif
