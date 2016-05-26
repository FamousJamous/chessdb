#ifndef CHESSDB_TAG_ITER_H
#define CHESSDB_TAG_ITER_H
#include <string>
namespace Chessdb {

class ByteStream;

class TagIter {
 public:
  TagIter(ByteStream & stream);
  operator bool () const;
  void operator++();
  std::string const & getKey() const;
  std::string const & getValue() const;
 private:
  ByteStream & stream;
  std::string key;
  std::string value;
};

} // namespace Chessdb
#endif
