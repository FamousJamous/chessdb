#include "exptagpair.h"
#include <cassert>
#include <iostream>
#include "tagpair.h"
namespace jl {

void checkExpTagPair(TagPair const & tagPair, ExpTagPair const & expTagPair) {
  if (tagPair.key != expTagPair.key || tagPair.value != expTagPair.value) {
    std::cerr << "tagPair.key " << tagPair.key
              << " tagPair.value " << tagPair.value
              << " expTagPair.key " << expTagPair.key
              << " expTagPair.value " << expTagPair.value
              << std::endl;
    assert(0);
  }
}

} // namespace jl
