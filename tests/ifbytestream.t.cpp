#include "ifbytestream.t.h"
#include "ifbytestream.h"
#include <cassert>
#include <string>
namespace jl {

void testIFByteStream() {
  std::string const exp = "abcd\nefgh\nijklmno\npqrstuvwxy\n\n\n\nz\n";
  IFByteStream stream("ifbytestream.txt");
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
