#include "parseround.t.h"
#include "expround.h"
#include "parseround.h"
#include "testbytestream.h"
namespace jl {

void testParseRound() {
  TestByteStream ts("  1.d4 f5 2.Nf3");
  Round round = parseRound(ts);
  ExpRound expRound = { 1, "d4 f5", "" };
  checkExpRound(round, expRound);
}

} // namespace jl
