#ifndef PARSE_TAG_PAIR_H
#define PARSE_TAG_PAIR_H
#include "tagpair.h"
namespace jl {

class ByteStream;
TagPair parseTagPair(ByteStream & stream);

} // namespace jl
#endif
