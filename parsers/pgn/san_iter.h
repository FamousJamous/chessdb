#ifndef CHESSDB_SAN_ITER_H
#define CHESSDB_SAN_ITER_H
#include <string>
namespace Chessdb {

class ByteStream;

class SanIter {
 public:
  SanIter(ByteStream & stream);
  operator bool () const;
  void operator++();
  std::string const & operator*() const;
 private:
  ByteStream & stream;
  std::string san;
};

} // namespace Chessdb
#endif
