#include "pgn_iter.t.h"
#include "pgn_iter.h"

#include <cassert>
#include <iostream>
#include "if_byte_stream.h"

namespace Chessdb {

namespace {

template <int nTags>
void testTags(std::string const (&tags)[nTags][2], int const pgnNum,
              PgnIter & it) {
  int tagI = 0;
  for (TagIter tagIt = it.getTagIt(); tagIt; ++tagIt, ++tagI) {
    if (nTags == tagI) {
      std::cerr << "tagI at limit " << nTags
                << " tagIt[ " << tagIt.getKey() << "] " << tagIt.getValue()
                << std::endl;
      assert(0);
    }
    std::string const & key = tags[tagI][0];
    std::string const & value = tags[tagI][1];
    if (tagIt.getKey() != key || tagIt.getValue() != value) {
      std::cerr << "pgnNum " << pgnNum
                << " tagIt[" << tagIt.getKey() << "] " << tagIt.getValue()
                << " tags[" << key << "] " << value << std::endl;
      assert(0);
    }
  }
  if (tagI != nTags) {
    std::cerr << "pgnNum " << pgnNum
              << " tagI " << tagI << " nTags " << nTags << std::endl;
    assert(0);
  }
}

template <int nSans>
void testSans(std::string const (&sans)[nSans],
              int const pgnNum,
              PgnIter & it) {
  int sanI = 0;
  for (SanIter sanIt = it.getSanIt(); sanIt; ++sanIt, ++sanI) {
    if (nSans == sanI) {
      std::cerr << "sanI at limit " << nSans
                << " *sanIt " << *sanIt << std::endl;
      assert(0);
    }
    if (*sanIt != sans[sanI]) {
      std::cerr << "pgnNum " << pgnNum
                << " *sanIt " << *sanIt << " sans[" << sanI << "] " << sans[sanI] << std::endl;
      assert(0);
    }
  }
  if (sanI != nSans) {
    std::cerr << "pgnNum " << pgnNum
              << " sanI " << sanI << " nSans " << nSans << std::endl;
    assert(0);
  }
}

template <int nTags, int nSans>
void testPgn(std::string const (&tags)[nTags][2],
              std::string const (&sans)[nSans],
              int const pgnNum,
              PgnIter & it) {
  testTags(tags, pgnNum, it);
  testSans(sans, pgnNum, it);
  ++it;
}

void testPgn0(PgnIter & it) {
  std::string const tags[][2] = {
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
  std::string const sans[] = {
    "d4", "f5", "Nf3", "Nf6", "c4", "g6", "g3", "Bg7", "Bg2", "O-O", "O-O",
    "d6", "Nc3", "Nc6", "d5", "Na5", "b3", "c5", "Bd2", "a6", "Qc2", "b5",
    "cxb5", "axb5", "Nxb5", "Ne4", "Rac1", "Nxd2", "Qxd2", "Qb6", "Nc3", "Qb4",
    "Na4", "Rb8", "Qxb4", "Rxb4", "Nd2", "Bd7", "Nxc5", "1/2-1/2"
  };
  testPgn(tags, sans, 0, it);
}

void testPgn1(PgnIter & it) {
  std::string const tags[][2] = {
    { "Event", "TCh-SVK 2015-16"},
    { "Site", "Slovakia SVK"},
    { "Date", "2016.02.28"},
    { "Round", "8.6"},
    { "White", "Jurcik, Marian"},
    { "Black", "Kantorik, Marian"},
    { "Result", "1-0"},
    { "WhiteElo", "2494"},
    { "BlackElo", "2252"},
    { "ECO", "A85"},
    { "EventDate", "2015.10.10" }
  };
  std::string const sans[] = {
    "c4", "f5", "d4", "Nf6", "Nc3", "e6", "Nf3", "d5", "Bf4", "c6", "e3", "Bd6",
    "Bd3", "O-O", "Qc2", "Ne4", "h4", "Bxf4", "exf4", "Qa5", "O-O", "Nd7",
    "Rab1", "Ndf6", "b4", "Qd8", "b5", "cxb5", "Nxb5", "a6", "Nc3", "Nxc3",
    "Qxc3", "dxc4", "Bxc4", "b5", "Bb3", "Nd5", "Qc6", "Bd7", "Qd6", "Bc8",
    "Qe5", "Re8", "Rfe1", "Kf8", "Rbc1", "Qf6", "g3", "Qxe5", "Nxe5", "Bb7",
    "Nd7+", "Kf7", "Nc5", "Ra7", "Re5", "Ba8", "Rce1", "Rae7", "Nxa6", "g6",
    "Rc1", "Ra7", "Nc5", "Nc7", "Nd3", "Bd5", "Nb4", "Bxb3", "axb3", "Na6",
    "Ra1", "Rea8", "Rxa6", "Rxa6", "Nxa6", "Rxa6", "Rxb5", "Rd6", "Rb7+", "Kg8",
    "Rb4", "h5", "Kf1", "Ra6", "Ra4", "Rc6", "Rc4", "Ra6", "Rc1", "Rd6", "Rd1",
    "Rb6", "Rb1", "Rb4", "Ke2", "Rxd4", "b4", "Kf7", "b5", "Ke7", "b6", "Rd8",
    "b7", "Rb8", "Kd3", "Kd6", "1-0"
  };
  testPgn(tags, sans, 1, it);
}

void testPgn2(PgnIter & it) {
  std::string const tags[][2] = {
    { "Event", "IMSA Blitz 2016" },
    { "Site", "Huai'an CHN" },
    { "Date", "2016.02.28" },
    { "Round", "3.8" },
    { "White", "Wang Yue" },
    { "Black", "Ivanchuk, Vassily" },
    { "Result", "1-0" },
    { "WhiteElo", "2718" },
    { "BlackElo", "2710" },
    { "ECO", "A88" },
    { "EventDate", "2016.02.28" },
  };
  std::string const sans[] = {
    "Nf3", "d6", "d4", "g6", "c4", "f5", "Nc3", "Nf6", "g3", "Bg7", "Bg2",
    "O-O", "O-O", "c6", "Rb1", "a5", "Qb3", "h6", "Rd1", "Kh7", "c5", "Na6",
    "cxd6", "exd6", "d5", "c5", "Nd2", "Nc7", "Nc4", "a4", "Qc2", "b5", "Nb6",
    "Ra6", "Nxc8", "Qxc8", "e4", "b4", "exf5", "Qxf5", "Qxf5", "gxf5", "Ne2",
    "Re8", "Nf4", "Ne4", "Bh3", "Rf8", "Bf1", "Ra7", "f3", "Nf6", "Kg2", "Nd7",
    "Ne6", "Rf6", "Nf4", "Nb6", "Be3", "Rf7", "Bf2", "Be5", "Nd3", "Bf6",
    "Nxb4", "Rb7", "Nc2", "Ncxd5", "Na3", "Rfd7", "Bd3", "Ne7", "Re1", "Nbd5",
    "Nc4", "Nb4", "Rxe7+", "Rxe7", "Bxf5+", "Kg8", "a3", "Nd5", "Be4", "Rxe4",
    "fxe4", "1-0"
  };
  testPgn(tags, sans, 2, it);


}

void testPgn3(PgnIter & it) {
  std::string const tags[][2] = {
    { "Event", "TCh-AUT 2015-16" },
    { "Site", "Austria AUT" },
    { "Date", "2016.02.28" },
    { "Round", "8.4" },
    { "White", "Bromberger, Stefan" },
    { "Black", "Georgiadis, Nico" },
    { "Result", "0-1" },
    { "WhiteElo", "2532" },
    { "BlackElo", "2473" },
    { "ECO", "A80" },
    { "EventDate", "2015.11.05" }
  };
  std::string const sans[] = {
    "d4", "f5", "Nf3", "Nf6", "Bf4", "g6", "e3", "Bg7", "Nbd2", "O-O", "h3",
    "d6", "Be2", "Nc6", "Bh2", "e6", "O-O", "Qe7", "c3", "a5", "a3", "b6", "c4",
    "e5", "c5", "bxc5", "dxc5", "Kh8", "Rc1", "Nd8", "cxd6", "cxd6", "Nc4",
    "Ba6", "Re1", "Ne4", "Nb6", "Bxe2", "Rxe2", "Rb8", "Nd5", "Qb7", "b4",
    "Ne6", "Rec2", "axb4", "axb4", "Qb5", "Rc6", "Rfd8", "R6c4", "Rb7", "Rc8",
    "Rb8", "Rxd8+", "Rxd8", "Ne7", "Bf6", "Qd5", "Qxd5", "Nxd5", "Rb8", "Rc6",
    "Nd8", "Ra6", "Nf7", "g4", "Kg7", "Kg2", "Bd8", "Ra3", "Rb5", "Rd3", "Nf6",
    "Nxf6", "Kxf6", "Rb3", "Ba5", "gxf5", "gxf5", "Bg3", "Rxb4", "Ra3", "Rb5",
    "Ra4", "h6", "Ra2", "Ke6", "Rc2", "Rb8", "Ra2", "Bb6", "Nd2", "Bc5", "Nc4",
    "Rg8", "Kh2", "Ng5", "Nd2", "Kd5", "f4", "Ne4", "Nxe4", "Kxe4", "Ra4+",
    "Kf3", "fxe5", "Bxe3", "0-1"
  };
  testPgn(tags, sans, 3, it);
}

} // namespace

void testPgnIter() {
  IFByteStream ifb("data/pgn_iter.t.txt");
  PgnIter it(ifb);
  testPgn0(it);
  testPgn1(it);
  testPgn2(it);
  testPgn3(it);
  assert(!it);
}

} // namespace Chessdb
