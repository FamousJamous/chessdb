#include "tag_iter.t.h"
#include "tag_iter.h"

#include <cassert>
#include <iostream>
#include <string>
#include "if_byte_stream.h"
namespace Chessdb {

void testTagIter() {
  std::string const exp[][2] = {
    { "Event", "7. Ciutat de Manacor" },
    { "Site", "Manacor ESP" },
    { "Date", "2016.02.28" },
    { "Round", "9.2" },
    { "White", "Pardo Simon, David" },
    { "Black", "Stremavicius, T" },
    { "Result", "1/2-1/2" },
    { "WhiteElo", "2512" },
    { "BlackElo", "2468" },
    { "ECO", "A89" },
    { "EventDate", "2016.02.21" }
  };
  IFByteStream ifb("data/tag_iter.t.txt");
  int i = 0;
  for (TagIter it(ifb); it; ++it, ++i) {
    std::string const & key = exp[i][0];
    std::string const & value = exp[i][1];
    if (it.getKey() != key || it.getValue() != value) {
      std::cerr << "it[" << it.getKey() << "] " << it.getValue()
                << " exp[" << key << "] " << value << std::endl;
      assert(0);
    }
  }
  int const n = sizeof(exp)/sizeof(exp[0]);
  if (i != n) {
    std::cerr << "i " << i << " exp " << n << std::endl;
    assert(0);
  }
}

} // namespace Chessdb
