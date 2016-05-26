#include "game_iter.t.h"
#include "game_iter.h"

#include <iostream>
#include <stdexcept>
#include "if_byte_stream.h"

namespace Chessdb {

void testGameIter() {
  //IFByteStream ifb("../../data/KingBase2016-03-A80-A99.pgn");
  IFByteStream ifb("data/game_iter.t.txt");
  try {
    for (GameIter it(ifb); it; ++it) {
    }
  } catch (std::runtime_error const & err) {
    std::cerr << err.what() << std::endl;
  }
}

} // namespace Chessdb
