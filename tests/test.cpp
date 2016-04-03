#include <cassert>
#include <fstream>
#include <iostream>

#include "ifbytestream.h"
#include "ifbytestream.t.h"
#include "parsemove.t.h"
#include "parsegame.h"
#include "parsegame.t.h"
#include "parseround.t.h"
#include "parsetagpair.t.h"
#include "testbytestream.t.h"
#include "testutils.h"

using namespace jl;

void dumpFront(std::string const & fileName, int const numChars, std::string const & outFileName = "") {
  std::ifstream ifs(fileName.c_str());
  std::ofstream * ofs = 0;
  if (!outFileName.empty()) {
    ofs = new std::ofstream(outFileName.c_str());
  }
  for (int i = 0; ifs.good() && i < numChars; ++i) {
    char c = ifs.get();
    std::cout << c;
    if (ofs) {
      ofs->put(c);
    }
  }
  std::cout << std::endl;
  if (ofs) {
    delete ofs;
  }
}

void testPrint() {
  std::string const fileName = "KingBase2016-03-A80-A99.pgn";
  std::string outFileName = "test_out.txt";
  dumpFront(fileName, 2000, outFileName);
  std::cout << std::endl;
}

void parseSomeGames() {
  std::string const fileName = "KingBase2016-03-A80-A99.pgn";
  IFByteStream stream(fileName);
  for (int i = 0; i < 3; ++i) {
    Game game = parseGame(stream);
    std::cout << game.tagPairs << std::endl;
    std::cout << game.rounds << std::endl;
  }
}

int main() {
  //testPrint();
  testIFByteStream();
  testParseMove();
  testParseGame();
  testParseRound();
  testParseTagPair();
  testTestByteStream();
  parseSomeGames();
  return 0;
}
