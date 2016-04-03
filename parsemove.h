#ifndef PARSE_MOVE_H
#define PARSE_MOVE_H
#include "move.h"
namespace jl {

class ByteStream;
Move parseMove(ByteStream & byteStream);

} // namespace jl
#endif
