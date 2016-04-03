#ifndef TAG_PAIR_H
#define TAG_PAIR_H
#include <string>
namespace jl {

struct TagPair {
  TagPair(std::string const & key, std::string const & value):
    key(key), value(value) {
  }
  std::string key;
  std::string value;
};

} // namespace jl
#endif
