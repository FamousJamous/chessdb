#include "testbytestream.t.h"
#include "testbytestream.h"
#include <cassert>
#include <iostream>
#include <string>
namespace jl {

void testTestByteStream() {
  std::string const exp = "abcd\nefg\nab";
  TestByteStream stream(exp);
  for (int i = 0, n = exp.size(); i < n; ++i) {
    if (!stream.good()) {
      std::cerr << "stream went bad early at index " << i
                << ". expected " << n << std::endl;
      assert(0);
    }
    char c = stream.get();
    if (c != exp[i]) {
      std::cout << "c " << c << " exp[" << i << "] " << exp[i] << std::endl;
      assert(0);
    }
  }
  stream.get();
  if (stream.good()) {
    std::cerr << "expected stream to be bad" << std::endl;
    assert(0);
  }
}

} // namespace jl
