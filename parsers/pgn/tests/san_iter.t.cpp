#include "san_iter.t.h"
#include "san_iter.h"

#include <cassert>
#include <iostream>
#include <string>
#include "byte_stream.h"
#include "if_byte_stream.h"
#include "test_byte_stream.h"
namespace Chessdb {

namespace {

template <int n>
void test(std::string const (&exp)[n], ByteStream * byteStream) {
  int i = 0;
  for (SanIter it(*byteStream); it; ++it, ++i) {
    if (*it != exp[i]) {
      std::cerr << "*it " << *it << " exp[" << i << "] " << exp[i] << std::endl;
      assert(0);
    }
  }
  if (i != n) {
    std::cerr << "i " << i << " exp " << n << std::endl;
    assert(0);
  }
}

void bugNxb5a6() {
  std::string const exp[] = {
    "Nxb5", "a6"
  };
  TestByteStream tbs("1.Nxb5\na6");
  test(exp, &tbs);
}

void testGame0() {
  std::string const exp[] = {
    "d4", "f5", "Nf3", "Nf6", "c4", "g6", "g3", "Bg7", "Bg2", "O-O", "O-O",
    "d6", "Nc3", "Nc6", "d5", "Na5", "b3", "c5", "Bd2", "a6", "Qc2", "b5",
    "cxb5", "axb5", "Nxb5", "Ne4", "Rac1", "Nxd2", "Qxd2", "Qb6", "Nc3", "Qb4",
    "Na4", "Rb8", "Qxb4", "Rxb4", "Nd2", "Bd7", "Nxc5", "1/2-1/2"
  };
  IFByteStream ifb("data/san_iter.t.txt");
  test(exp, &ifb);
}

} // namespace

void testSanIter() {
  testGame0();
  bugNxb5a6();
}

} // namespace Chessdb
