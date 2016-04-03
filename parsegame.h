#ifndef PARSE_GAME_H
#define PARSE_GAME_H
#include "game.h"
namespace jl {

class ByteStream;
Game parseGame(ByteStream & byteStream);

} // namespace jl
#endif
