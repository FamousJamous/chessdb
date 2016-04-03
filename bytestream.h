#ifndef BYTE_STREAM_H
#define BYTE_STREAM_H
namespace jl {
class ByteStream {
 public:
  virtual char get() = 0;
  virtual void putBack(char c) = 0;
  virtual bool good() const = 0;
};
} // namespace jl
#endif
