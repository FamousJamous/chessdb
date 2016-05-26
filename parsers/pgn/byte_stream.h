#ifndef CHESSDB_BYTE_STREAM_H
#define CHESSDB_BYTE_STREAM_H
namespace Chessdb {

class ByteStream {
 public:
  virtual char get() = 0;
  virtual void putBack(char c) = 0;
  virtual bool good() const = 0;
};

} // namespace Chessdb
#endif
