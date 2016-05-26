#include "infer_move.t.h"
#include "infer_move.h"

#include <cassert>
#include <iostream>
#include "coord.h"
#include "move.h"
#include "parse_san.h"
#include "print_state.h"
#include "san_iter.h"
#include "state.h"
#include "test_byte_stream.h"
namespace Chessdb {

namespace {

using namespace Chessdb;

bool operator!=(Coord const & lhs, Coord const & rhs) {
  return lhs.file != rhs.file || lhs.rank != rhs.rank;
}

bool operator!=(FromTo const & lhs, FromTo const & rhs) {
  return lhs.from != rhs.from || lhs.to != rhs.to;
}

bool operator!=(Move const & lhs, Move const & rhs) {
  if (lhs.sans != rhs.sans) {
    return true;
  }
  if (lhs.fromTo != rhs.fromTo) {
    return true;
  }
  if (lhs.castle != rhs.castle) {
    return true;
  }
  if (lhs.check != rhs.check) {
    return true;
  }
  if (lhs.enPassCap != rhs.enPassCap) {
    return true;
  }
  if (lhs.promPiece != rhs.promPiece) {
    return true;
  }
  return false;
}

std::ostream & operator<<(std::ostream & os, Coord const & coord) {
  return os << "[ " << coord.file << " " << coord.rank << " ]";
}

std::ostream & operator<<(std::ostream & os, FromTo const & fromTo) {
  return os << "[ from " << fromTo.from << " to " << fromTo.to << " ]";
}

std::ostream & operator<<(std::ostream & os, Move const & move) {
  return os << "[ sans " << move.sans
            << " fromTo " << move.fromTo
            << " castle " << move.castle
            << " check " << move.check
            << " enPassCap " << move.enPassCap
            << " promPiece " << move.promPiece
            << " ]";
}

void test(std::string const & san, Move const & exp, State * state) {
  SanProps sanProps = parseSan(san);
  Move move;
  move.sans = san;
  try {
    inferMove(sanProps, *state, &move);
  } catch (std::runtime_error const & err) {
    std::cerr << "caught err for san " << san << " in testInferMove "
              << err.what() << std::endl;
    assert(0);
  }

  if (exp != move) {
    std::cerr << "exp  " << exp << std::endl
              << "move " << move << std::endl;
    printState(*state);
    assert(0);
  }
  state->makeMove(move);
}

FromTo strToFromTo(std::string const & str, int i) {
  return FromTo(Coord(str[i], str[i + 1] - '0'),
                Coord(str[i + 2], str[i + 3] - '0'));
}

Move makeMove(std::string const & san, std::string const & fromTos) {
  Move move;
  move.sans = san;
  move.fromTo = strToFromTo(fromTos, 0);
  if (4 < fromTos.size()) {
    move.castle = strToFromTo(fromTos, 4);
  }
  return move;
}

void testMove(std::string const & san, std::string const & fromTos,
              State * state) {
  test(san, makeMove(san, fromTos), state);
}

void testEnPass(std::string const & san, std::string const & fromTos,
                std::string const & enPassCap, State * state) {
  Move exp = makeMove(san, fromTos);
  exp.enPassCap.file = enPassCap[0];
  exp.enPassCap.rank = enPassCap[1] - '0';
  test(san, exp, state);
}

void testProm(std::string const & san, std::string const & fromTos,
              char promPiece, State * state) {
  Move exp = makeMove(san, fromTos);
  exp.promPiece = promPiece;
  test(san, exp, state);
}

void testPromote() {
  State::Color const turn = State::BLACK;
  State::Board const board = {
  // abcdefgh
    "        ", // 1
    "   P    ", // 2
    "        ", // 3
    "        ", // 4
    "        ", // 5
    "        ", // 6
    "        ", // 7
    "        ", // 8
  };
  testProm("d1Q", "d2d1", 'Q', &State(turn, board));
}

void testPawn() {
  State state0;
  testMove("d4", "d2d4", &state0);
  State state1;
  testMove("d3", "d2d3", &state1);

  State::Board const enPassant0 = {
  // abcdefgh
    "        ", // 1
    "        ", // 2
    "        ", // 3
    "      pP", // 4
    "        ", // 5
    "        ", // 6
    "        ", // 7
    "        ", // 8
  };
  testEnPass("hxg3e.p.", "h4g3", "g4", &State(State::BLACK, enPassant0));
  testEnPass("hxg3", "h4g3", "g4", &State(State::BLACK, enPassant0));
}

void testGame0() {
  State state;
  testMove("d4", "d2d4", &state);
  testMove("f5", "f7f5", &state);
  testMove("Nf3", "g1f3", &state);
  testMove("Nf6", "g8f6", &state);
  testMove("c4", "c2c4", &state);
  testMove("g6", "g7g6", &state);
  testMove("g3", "g2g3", &state);
  testMove("Bg7", "f8g7", &state);
  testMove("Bg2", "f1g2", &state);
  testMove("O-O", "e8g8h8f8", &state);
  testMove("O-O", "e1g1h1f1", &state);
  testMove("d6", "d7d6", &state);
  testMove("Nc3", "b1c3", &state);
  testMove("Nc6", "b8c6", &state);
  testMove("d5", "d4d5", &state);
  testMove("Na5", "c6a5", &state);
  testMove("b3", "b2b3", &state);
  testMove("c5", "c7c5", &state);
  testMove("Bd2", "c1d2", &state);
  testMove("a6", "a7a6", &state);
  testMove("Qc2", "d1c2", &state);
  testMove("b5", "b7b5", &state);
  testMove("cxb5", "c4b5", &state);
  testMove("axb5", "a6b5", &state);
  testMove("Nxb5", "c3b5", &state);
  testMove("Ne4", "f6e4", &state);
  testMove("Rac1", "a1c1", &state);
  testMove("Nxd2", "e4d2", &state);
  testMove("Qxd2", "c2d2", &state);
  testMove("Qb6", "d8b6", &state);
  testMove("Nc3", "b5c3", &state);
  testMove("Qb4", "b6b4", &state);
  testMove("Na4", "c3a4", &state);
  testMove("Rb8", "a8b8", &state);
  testMove("Qxb4", "d2b4", &state);
  testMove("Rxb4", "b8b4", &state);
  testMove("Nd2", "f3d2", &state);
  testMove("Bd7", "c8d7", &state);
  testMove("Nxc5", "a4c5", &state);
  //testMove("1/2-1/2")
}

void testGame1() {
  State state;
  testMove("c4", "c2c4", &state);
  testMove("f5", "f7f5", &state);
  testMove("d4", "d2d4", &state);
  testMove("Nf6", "g8f6", &state);
  testMove("Nc3", "b1c3", &state);
  testMove("e6", "e7e6", &state);
  testMove("Nf3", "g1f3", &state);
  testMove("d5", "d7d5", &state);
  testMove("Bf4", "c1f4", &state);
}

bool operator!=(State const & lhs, State const & rhs) {
  if (lhs.getTurn() != rhs.getTurn()) {
    return true;
  }
  for (char file = 'a'; file <= 'h'; ++file) {
    for (int rank = 1; rank <= 8; ++rank) {
      Coord coord(file, rank);
      if (lhs.getPiece(coord) != rhs.getPiece(coord)) {
        return true;
      }
    }
  }
  return false;
}

bool operator==(State const & lhs, State const & rhs) {
  return !(lhs != rhs);
}

void pause() {
  std::string dum;
  std::getline(std::cin, dum);
}

void runGame(std::string const & sans, State * state) {
  TestByteStream tbs(sans);

  //int num = 1;

  for (SanIter it(tbs); it; ++it) {
    SanProps sanProps = parseSan(*it);
    Move move;
    try {
      inferMove(sanProps, *state, &move);
    } catch (std::runtime_error const & err) {
      printState(*state);
      std::cerr << "san: " << *it << " error " << err.what() << std::endl;
      assert(0);
    }
    state->makeMove(move);


    /*
    std::cout << ((1 + num) >> 1) << ". " << *it << std::endl;
    printState(*state, BoardSide::WHITE);
    pause();
    ++num;
    */
  }
}

void assertStates(State const & state, State const & exp) {
  if (state != exp) {
    std::cerr << "exp:" << std::endl;
    printState(exp);
    std::cerr << "state:" << std::endl;
    printState(state);
    assert(0);
  }
}

void testFinalState(std::string const & sans, State::Color const expTurn,
                    State::Board const & expBoard) {
  State state;
  runGame(sans, &state);
  State exp(expTurn, expBoard);
  assertStates(state, exp);
}

void testFinalState0() {
  std::string const sans = "1.d4 f5 2.Nf3 Nf6 3.c4 g6 4.g3 Bg7 5.Bg2 O-O 6.O-O d6 7.Nc3 Nc6 8.d5 Na5 \
9.b3 c5 10.Bd2 a6 11.Qc2 b5 12.cxb5 axb5 13.Nxb5 Ne4 14.Rac1 Nxd2 15.Qxd2 \
Qb6 16.Nc3 Qb4 17.Na4 Rb8 18.Qxb4 Rxb4 19.Nd2 Bd7 20.Nxc5";// 1/2-1/2
  State::Color const expTurn = State::BLACK;
  State::Board const expBoard = {
    "  r  rk ",
    "p  nppbp",
    " p    p ",
    " R      ",
    "N np P  ",
    "   P  P ",
    "   BP BP",
    "     RK "
  };
  testFinalState(sans, expTurn, expBoard);
}

void testFinalState1() {
  std::string const sans = "1.c4 f5 2.d4 Nf6 3.Nc3 e6 4.Nf3 d5 5.Bf4 c6 6.e3 Bd6 7.Bd3 O-O 8.Qc2 Ne4 \
9.h4 Bxf4 10.exf4 Qa5 11.O-O Nd7 12.Rab1 Ndf6 13.b4 Qd8 14.b5 cxb5 15.Nxb5 \
a6 16.Nc3 Nxc3 17.Qxc3 dxc4 18.Bxc4 b5 19.Bb3 Nd5 20.Qc6 Bd7 21.Qd6 Bc8 \
22.Qe5 Re8 23.Rfe1 Kf8 24.Rbc1 Qf6 25.g3 Qxe5 26.Nxe5 Bb7 27.Nd7+ Kf7 28.\
Nc5 Ra7 29.Re5 Ba8 30.Rce1 Rae7 31.Nxa6 g6 32.Rc1 Ra7 33.Nc5 Nc7 34.Nd3 \
Bd5 35.Nb4 Bxb3 36.axb3 Na6 37.Ra1 Rea8 38.Rxa6 Rxa6 39.Nxa6 Rxa6 40.Rxb5 \
Rd6 41.Rb7+ Kg8 42.Rb4 h5 43.Kf1 Ra6 44.Ra4 Rc6 45.Rc4 Ra6 46.Rc1 Rd6 47.\
Rd1 Rb6 48.Rb1 Rb4 49.Ke2 Rxd4 50.b4 Kf7 51.b5 Ke7 52.b6 Rd8 53.b7 Rb8 54.\
Kd3 Kd6";// 1-0
  State::Color const expTurn = State::WHITE;
  State::Board const expBoard = {
    " r      ",
    "     p  ",
    "   k  p ",
    "     p p",
    "     P P",
    "   KP P ",
    " p      ",
    " R      ",
  };
  testFinalState(sans, expTurn, expBoard);
}

void testFinalState2() {
  std::string const sans = "1.Nf3 d6 2.d4 g6 3.c4 f5 4.Nc3 Nf6 5.g3 Bg7 6.Bg2 O-O 7.O-O c6 8.Rb1 a5 9.\
Qb3 h6 10.Rd1 Kh7 11.c5 Na6 12.cxd6 exd6 13.d5 c5 14.Nd2 Nc7 15.Nc4 a4 16.\
Qc2 b5 17.Nb6 Ra6 18.Nxc8 Qxc8 19.e4 b4 20.exf5 Qxf5 21.Qxf5 gxf5 22.Ne2 \
Re8 23.Nf4 Ne4 24.Bh3 Rf8 25.Bf1 Ra7 26.f3 Nf6 27.Kg2 Nd7 28.Ne6 Rf6 29.\
Nf4 Nb6 30.Be3 Rf7 31.Bf2 Be5 32.Nd3 Bf6 33.Nxb4 Rb7 34.Nc2 Ncxd5 35.Na3 \
Rfd7 36.Bd3 Ne7 37.Re1 Nbd5 38.Nc4 Nb4 39.Rxe7+ Rxe7 40.Bxf5+ Kg8 41.a3 \
Nd5 42.Be4 Rxe4 43.fxe4";// 1-0
  State::Color const expTurn = State::BLACK;
  State::Board const expBoard = {
    " r      ",
    " p   bkp",
    "p     p ",
    "P n p   ",
    "  PN    ",
    "   P B P",
    " R      ",
    "      K "
  };
  testFinalState(sans, expTurn, expBoard);
}

void testFinalState3() {
  State inter;

  std::string const sans0 = "1.d4 f5 2.Nf3 Nf6 3.Bf4 g6 4.e3 Bg7 5.Nbd2 O-O 6.h3 d6 7.Be2 Nc6 8.Bh2 e6";
  runGame(sans0, &inter);
  State::Color const turn0 = State::WHITE;
  State::Board const board0 = {
    "r  qk  r",
    "pppnbppb",
    "    pn p",
    "   p    ",
    "     P  ",
    "  NPPNP ",
    "PPP   BP",
    "R BQ RK ",
  };
  assertStates(inter, State(turn0, board0));

  std::string const sans1 = "9.O-O Qe7 10.c3 a5 11.a3 b6 12.c4 e5 13.c5 bxc5 14.dxc5 Kh8 15.Rc1 Nd8";
  runGame(sans1, &inter);
  State::Color const turn1 = State::WHITE;
  State::Board const board1 = {
    "  rq rk ",
    " p nbppb",
    "p   pn p",
    "        ",
    "P p PP  ",
    "   P NP ",
    "  P Q BP",
    "R BN R K",
  };
  assertStates(inter, State(turn1, board1));

  std::string const sans2 = " 16.\
cxd6 cxd6 17.Nc4 Ba6 18.Re1 Ne4 19.Nb6 Bxe2 20.Rxe2 Rb8 21.Nd5 Qb7 22.b4 \
Ne6 23.Rec2 axb4 24.axb4 Qb5 25.Rc6 Rfd8 26.R6c4 Rb7 27.Rc8 Rb8 ";
  runGame(sans2, &inter);
  State::Color const turn2 = State::WHITE;
  State::Board const board2 = {
    "  rq  k ",
    "     ppb",
    "    pn p",
    " p  N   ",
    " Q nPP  ",
    "   PN P ",
    "      BP",
    " RrR   K",
  };
  assertStates(inter, State(turn2, board2));

  std::string const sans3 = "28.Rxd8+ \
Rxd8 29.Ne7 Bf6 30.Qd5 Qxd5 31.Nxd5 Rb8 32.Rc6 Nd8 33.Ra6 Nf7 34.g4 Kg7 \
35.Kg2 Bd8 36.Ra3 Rb5 37.Rd3 Nf6 38.Nxf6 Kxf6 39.Rb3 Ba5 40.gxf5 gxf5";
  runGame(sans3, &inter);
  State::Color const turn3 = State::WHITE;
  State::Board const board3 = {
    "        ",
    "     pkb",
    " r  pn p",
    " p      ",
    "BR  PP  ",
    "   P K  ",
    "     N P",
    "        ",
  };
  assertStates(inter, State(turn3, board3));

  std::string const sans4 = " 41.\
Bg3 Rxb4 42.Ra3 Rb5 43.Ra4 h6 44.Ra2 Ke6 45.Rc2 Rb8 46.Ra2 Bb6 47.Nd2 Bc5 \
48.Nc4 Rg8 49.Kh2 Ng5 50.Nd2 Kd5 51.f4 Ne4 52.Nxe4 Kxe4 53.Ra4+ Kf3 54.\
fxe5 Bxe3";
  runGame(sans4, &inter);
  State::Color const turn4 = State::WHITE;
  State::Board const board4 = {
    "        ",
    "       k",
    "    BKbp",
    "r       ",
    "    pP  ",
    "   P   P",
    "        ",
    "      R ",
  };
  assertStates(inter, State(turn4, board4));

  std::string const sans = "1.d4 f5 2.Nf3 Nf6 3.Bf4 g6 4.e3 Bg7 5.Nbd2 O-O 6.h3 d6 7.Be2 Nc6 8.Bh2 e6 \
9.O-O Qe7 10.c3 a5 11.a3 b6 12.c4 e5 13.c5 bxc5 14.dxc5 Kh8 15.Rc1 Nd8 16.\
cxd6 cxd6 17.Nc4 Ba6 18.Re1 Ne4 19.Nb6 Bxe2 20.Rxe2 Rb8 21.Nd5 Qb7 22.b4 \
Ne6 23.Rec2 axb4 24.axb4 Qb5 25.Rc6 Rfd8 26.R6c4 Rb7 27.Rc8 Rb8 28.Rxd8+ \
Rxd8 29.Ne7 Bf6 30.Qd5 Qxd5 31.Nxd5 Rb8 32.Rc6 Nd8 33.Ra6 Nf7 34.g4 Kg7 \
35.Kg2 Bd8 36.Ra3 Rb5 37.Rd3 Nf6 38.Nxf6 Kxf6 39.Rb3 Ba5 40.gxf5 gxf5 41.\
Bg3 Rxb4 42.Ra3 Rb5 43.Ra4 h6 44.Ra2 Ke6 45.Rc2 Rb8 46.Ra2 Bb6 47.Nd2 Bc5 \
48.Nc4 Rg8 49.Kh2 Ng5 50.Nd2 Kd5 51.f4 Ne4 52.Nxe4 Kxe4 53.Ra4+ Kf3 54.\
fxe5 Bxe3";// 0-1
  State state;
  testFinalState(sans, turn4, board4);
}

void testFinalStates() {
  testFinalState0();
  testFinalState1();
  testFinalState2();
  testFinalState3();
}

void testRunThrough0() {
  std::string const sans = "1.Nf3 f5 2.g3 Nf6 3.Bg2 g6 4.O-O Bg7 5.c4 O-O 6.Nc3 d6 7.d4 a5 8.d5 Na6 9. \
Nd4 Nc5 10.b3 Qe8 11.Rb1 Bd7 12.Bb2 g5 13.Ncb5 Rc8 14.Qd2 Qh5 15.Qxa5 f4 \
16.f3 fxg3 17.hxg3 c6 18.Nc7 Qg6 19.Nce6 Bxe6 20.dxe6 Nh5 21.g4 Nf4 22.e3 \
Nfxe6 23.Nf5 Bf6 24.Qd2 Rf7 25.b4 Nd7 26.c5 Bxb2 27.Rxb2 Ne5 28.Rc2 d5 29. \
Qc3 Qf6 30.a4 Kf8 31.Qe1 Nc4 32.Qg3 Qe5 33.Qh3 Nxe3 34.Re2 d4 35.Qh6+ Ke8 \
36.Rfe1 Rxf5 37.gxf5 Nf4 38.Qxg5 Nxe2+ 39.Rxe2 Kd7 40.Bh3 Qf6 41.Qf4 Rg8+ \
42.Kh2 Kc8 43.Rd2 Rd8 44.b5 Nc4 45.Rd1 Ne3 46.Rb1 Nd5 47.Qg3 Nc7 48.b6 Na6 \
49.Re1 d3 50.Re6 d2 51.Rxf6 exf6 52.Qg7 d1=Q 53.Qxf6 Qd2+ 54.Kg3 Qd7 55. \
Qe5 Rg8+ 56.Kh4 Re8 57.f6";// 1-0
  State state;
  runGame(sans, &state);
}

void testRunThrough1() {
  std::string const sans = "1.Nf3 e6 2.c4 f5 3.g3 Nf6 4.Bg2 Be7 5.O-O O-O 6.d4 d6 7.Nc3 Qe8 8.Qd3 Nc6  \
9.Nb5 Bd8 10.d5 Ne5 11.Qb3 Nxf3+ 12.exf3 e5 13.f4 a6 14.Nc3 exf4 15.Bxf4  \
Nh5 16.Rfe1 Qf7 17.Be3 f4 18.Bd4 Bf6 19.Qd1 Bg4 20.Qd2 fxg3 21.hxg3 Bxd4  \
22.Qxd4 Nf6 23.Ne4 Nxe4 24.Rxe4 Bf5 25.Re2 Rfe8 26.Rae1 b6 27.b4 Rxe2 28. \
Rxe2 Re8 29.Re3 h6 30.c5 bxc5 31.bxc5 Rxe3 32.Qxe3 Qf6 33.a3 dxc5 34.Qxc5  \
Qb6 35.Qc3 Kh7 36.Qe5 Qb1+ 37.Kh2 Qc2 38.Qf4 a5 39.g4 Bg6 40.Qd4 Qc1 41.f4 \
Qxa3 42.f5 Qd6+ 43.Kh1 Bf7 44.Qe4 Kg8 45.Qc4 Kf8 46.Qb5 a4 47.Qb8+ Be8 48. \
Qa7 Ke7 49.Qd4 Kf7 50.Qe3 Bd7 51.Qc3 a3 52.Qb3 Kf8 53.Qb8+ Ke7 54.Qb3 Be8  \
55.Bf3 Qc5 56.Kg2 Bb5 57.d6+ cxd6 58.Qe6+ Kd8 59.Qg8+ Be8 60.Qxg7 a2 61. \
Qf6+ Kc7 62.Qb2 Qa5 63.Qb7+ Kd8 64.f6 a1=Q 65.Qe7+ Kc8 66.Qb7+ Kd8 67.Qe7+ \
Kc8 68.Qxe8+ Qd8 69.Qc6+ Qc7 70.Qe8+ Qd8 71.Bb7+ Kc7 72.Qc6+ Kb8 73.Ba6  \
Qb2+ 74.Kh3 Qbb6";// 0-1
  State state;
  runGame(sans, &state);
}

void testRunThrough2() {
  std::string const sans = "1.Nf3 e6 2.d4 f5 3.c4 Nf6 4.e3 d5 5.Be2 c6 6.Nc3 Bd6 7.O-O O-O 8.b3 Nbd7  \
9.Bb2 Ne4 10.Nxe4 fxe4 11.Ne5 Qg5 12.Nxd7 Bxd7 13.Qd2 Rf6 14.f4 exf3 15. \
Rxf3 Rh6 16.g3 Be8 17.Rf2 Bg6 18.Raf1 Bxg3 19.hxg3 Qxg3+ 20.Rg2 Qh3 21.Bf3 \
Qh1+ 22.Kf2 Qh4+ 23.Ke2 Rf8 24.Qe1 Qh3 25.Rgf2 Rxf3 26.Rxf3 Bh5 27.Rf2 Rf6 \
28.Qf1 Rxf3 29.Rxf3 Qxf1+ 30.Kxf1 Bxf3 31.Kf2 Be4 32.cxd5 exd5 33.b4 Kf7  \
34.a4 Kf6 35.Bc3 h5 36.Be1 h4 37.Kf1 g5 38.Bf2 Kg6 39.Bg1 g4 40.Bf2 g3 41. \
Be1 Kg5"; // 0-1
  State state;
  runGame(sans, &state);
}

void testRunThroughs() {
  testRunThrough0();
  testRunThrough1();
  testRunThrough2();
}

void testGames() {
  testGame0();
  testGame1();
}

} // namespace

void testInferMove() {
  testPromote();
  testPawn();
  testGames();
  testFinalStates();
  testRunThroughs();
}

} // namespace Chessdb
