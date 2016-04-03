#include "parsegame.t.h"
#include "parsegame.h"
#include <cassert>
#include <iostream>
#include <string>
#include "expround.h"
#include "exptagpair.h"
#include "ifbytestream.h"
namespace jl {

void testParseGame() {
  std::string const fileName = "KingBase2016-03-A80-A99.pgn";

  ExpTagPair const expTagPairs[] = {
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
    { "EventDate", "2016.02.21" },
  };
  int const numExpTagPairs = sizeof(expTagPairs)/sizeof(expTagPairs[0]);

  ExpRound const expRounds[] = {
    { 1, "d4 f5", "" },
    { 2, "Nf3 Nf6", "" },
    { 3, "c4 g6", "" },
    { 4, "g3 Bg7", "" },
    { 5, "Bg2 O-O", "" },
    { 6, "O-O d6", "" },
    { 7, "Nc3 Nc6", "" },
    { 8, "d5 Na5", "" },
    { 9, "b3 c5", "" },
    { 10, "Bd2 a6", "" },
    { 11, "Qc2 b5", "" },
    { 12, "cxb5 axb5", "" },
    { 13, "Nxb5 Ne4", "" },
    { 14, "Rac1 Nxd2", "" },
    { 15, "Qxd2 \nQb6", "" },
    { 16, "Nc3 Qb4", "" },
    { 17, "Na4 Rb8", "" },
    { 18, "Qxb4 Rxb4", "" },
    { 19, "Nd2 Bd7", "" },
    { 20, "Nxc5 1/2-1/2", "" },
  };
  int const numExpRounds = sizeof(expRounds)/sizeof(expRounds[0]);

  IFByteStream stream(fileName);
  Game game = parseGame(stream);

  std::map<std::string, TagPair> const & tagPairs = game.tagPairs;
  if (numExpTagPairs != tagPairs.size()) {
    std::cout << "numExpTagPairs " << numExpTagPairs
              << " tagPairs.size() " << tagPairs.size()
              << std::endl;
    assert(0);
  }
  for (int i = 0; i < numExpTagPairs; ++i) {
    ExpTagPair const & expTagPair = expTagPairs[i];
    std::string const key = expTagPair.key;
    std::map<std::string, TagPair>::const_iterator find = tagPairs.find(key);
    if (tagPairs.end() == find) {
      std::cerr << key << " not found in tagPairs" << std::endl;
      assert(0);
    }
    checkExpTagPair(find->second, expTagPair);
  }

  std::vector<Round> const & rounds = game.rounds;
  if (numExpRounds != rounds.size()) {
    std::cout << "numExpRounds " << numExpRounds 
              << " rounds.size() " << rounds.size()
              << std::endl;
    assert(0);
  }
  for (int i = 0; i < numExpRounds; ++i) {
    const ExpRound & expRound = expRounds[i];
    const Round & round = rounds[i];
    checkExpRound(round, expRound);
  }
}

} // namespace jl
