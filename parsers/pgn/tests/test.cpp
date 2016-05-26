#include "if_byte_stream.t.h"
#include "infer_move.t.h"
#include "game_iter.t.h"
#include "parse_san.t.h"
#include "pgn_iter.t.h"
#include "san_iter.t.h"
#include "tag_iter.t.h"
#include "test_byte_stream.t.h"
int main() {
  using namespace Chessdb;
  testIFByteStream();
  testInferMove();
  testParseSan();
  testPgnIter();
  testSanIter();
  testTagIter();
  testTestByteStream();
  testGameIter();
  return 0;
}
