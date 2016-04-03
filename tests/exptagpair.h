#ifndef EXP_TAG_PAIR_H
#define EXP_TAG_PAIR_H
namespace jl {

struct ExpTagPair {
  char const * key;
  char const * value;
};

class TagPair;
void checkExpTagPair(TagPair const & tagPair, ExpTagPair const & expTagPair);

} // namespace jl
#endif
