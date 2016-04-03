#ifndef PARSE_ROUND_H
#define PARSE_ROUND_H
#include "round.h"
namespace jl {

class ByteStream;
Round parseRound(ByteStream & stream);

} // namespace jl
#endif
