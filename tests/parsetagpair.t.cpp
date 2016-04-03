#include "parsetagpair.t.h"
#include "parsetagpair.h"
#include <cassert>
#include "exptagpair.h"
#include "testbytestream.h"
namespace jl {

namespace {

void test0() {
  TestByteStream tbs("[Event \"7. Ciutat de Manacor\"]");
  TagPair tagPair = parseTagPair(tbs);
  assert(tagPair.key == "Event");
  assert(tagPair.value == "7. Ciutat de Manacor");
}

void test1() {
  TestByteStream ts("[abcd \"efgh\"]");
  TagPair tagPair = parseTagPair(ts);
  ExpTagPair expTagPair = {
    "abcd", "efgh",
  };
  checkExpTagPair(tagPair, expTagPair);
}

} // namespace

void testParseTagPair() {
  test0();
  test1();
}

} // namespace jl
