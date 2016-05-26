#include "parse_san.t.h"
#include "parse_san.h"

#include <cassert>
#include <iostream>
#include <string>
#include <stdexcept>
#include "san_props.h"
namespace Chessdb {

void test(std::string const & str, SanProps const & exp);

namespace {

std::ostream & operator<<(std::ostream & os, SanProps::Type const type) {
  switch (type) {
    case SanProps::MOVE_PIECE:
      return os << "move";
    case SanProps::KING_CASTLE:
      return os << "king castle";
    case SanProps::QUEEN_CASTLE:
      return os << "queen castle";
    case SanProps::DRAW:
      return os << "draw";
    case SanProps::WHITE_WINS:
      return os << "white wins";
    case SanProps::BLACK_WINS:
      return os << "black wins";
    default:
      return os;
  }
}

std::ostream & operator<<(std::ostream & os, SanProps::Piece const piece) {
  switch (piece) {
    case SanProps::PAWN:
      return os << "pawn";
    case SanProps::ROOK:
      return os << "rook";
    case SanProps::KNIGHT:
      return os << "knight";
    case SanProps::BISHOP:
      return os << "bishop";
    case SanProps::QUEEN:
      return os << "queen";
    case SanProps::KING:
      return os << "king";
    default:
      return os;
  }
}

std::ostream & operator<<(std::ostream & os, SanProps const & san) {
  return os << "type " << san.type
            << " piece " << san.piece
            << " file " << san.file
            << " rank " << san.rank
            << " isCapture " << san.isCapture
            << " isEnPassant " << san.isEnPassant
            << " fromFile " << san.fromFile
            << " fromRank " << san.fromRank
            << " isPromotion " << san.isPromotion
            << " promoteTo " << san.promoteTo
            << " isCheck " << san.isCheck
            << " isCheckmate " << san.isCheckmate;
}

bool operator!=(SanProps const & lhs, SanProps const & rhs) {
  return lhs.type != rhs.type ||
         lhs.piece != rhs.piece ||
         lhs.file != rhs.file ||
         lhs.rank != rhs.rank ||
         lhs.isCapture != rhs.isCapture ||
         lhs.isEnPassant != rhs.isEnPassant ||
         lhs.fromFile != rhs.fromFile ||
         lhs.fromRank != rhs.fromRank ||
         lhs.isPromotion != rhs.isPromotion ||
         lhs.promoteTo != rhs.promoteTo ||
         lhs.isCheck != rhs.isCheck ||
         lhs.isCheckmate != rhs.isCheckmate;
}

void testType(std::string const & str, SanProps::Type type) {
  SanProps exp;
  exp.type = type;
  test(str, exp);
}

void testCheckType(std::string const & str, SanProps::Type type) {
  SanProps exp;
  exp.type = type;
  exp.isCheck = true;
  test(str, exp);
}

SanProps makeMove(SanProps::Piece piece, char file, int rank, char fromFile = 0,
             int fromRank = 0) {
  SanProps san;
  san.type = SanProps::MOVE_PIECE;
  san.piece = piece;
  san.file = file;
  san.rank = rank;
  san.fromFile = fromFile;
  san.fromRank = fromRank;
  return san;
}

void testMove(std::string const & str, SanProps::Piece piece, char file, int rank,
              char fromFile = 0, int fromRank = 0) {
  test(str, makeMove(piece, file, rank, fromFile, fromRank));
}

void testCapture(std::string const & str, SanProps::Piece piece, char file, int rank,
                 char fromFile = 0, int fromRank = 0) {
  SanProps exp = makeMove(piece, file, rank, fromFile, fromRank);
  exp.isCapture = true;
  test(str, exp);
}

void testEnPassant(std::string const & str, char file, int rank, char fromFile) {
  SanProps exp = makeMove(SanProps::PAWN, file, rank, fromFile);
  exp.isCapture = true;
  exp.isEnPassant = true;
  test(str, exp);
}

void testPromotion(std::string const & str, char file, int rank, SanProps::Piece promoteTo) {
  SanProps exp = makeMove(SanProps::PAWN, file, rank);
  exp.isPromotion = true;
  exp.promoteTo = promoteTo;
  test(str, exp);
}

void testCheck(std::string const & str, SanProps::Piece piece, char file, int rank) {
  SanProps exp = makeMove(piece, file, rank);
  exp.isCheck = true;
  test(str, exp);
}

void testCheckmateCapture(std::string const & str, SanProps::Piece piece, char file,
                          int rank) {
  SanProps exp = makeMove(piece, file, rank);
  exp.isCheckmate = true;
  exp.isCapture = true;
  test(str, exp);
}

void testCheckmate(std::string const & str, SanProps::Piece piece, char file,
                   int rank) {
  SanProps exp = makeMove(piece, file, rank);
  exp.isCheckmate = true;
  test(str, exp);
}

} // namespace

void test(std::string const & str, SanProps const & exp) {
  SanProps const san = parseSan(str);
  if (exp != san) {
    std::cerr << "failed to parse " << str << std::endl
              << "san " << san << std::endl
              << "exp " << exp << std::endl;
    assert(0);
  }
}

void testParseSan() {
  try {
    testType("O-O", SanProps::KING_CASTLE);
    testCheckType("O-O+", SanProps::KING_CASTLE);
    testType("O-O-O", SanProps::QUEEN_CASTLE);
    testCheckType("O-O-O+", SanProps::QUEEN_CASTLE);
    testType("1/2-1/2", SanProps::DRAW);
    testType("1-0", SanProps::WHITE_WINS);
    testType("0-1", SanProps::BLACK_WINS);
    testMove("Be5", SanProps::BISHOP, 'e', 5);
    testMove("Nf3", SanProps::KNIGHT, 'f', 3);
    testMove("c5", SanProps::PAWN, 'c', 5);
    testCapture("Bxe5", SanProps::BISHOP, 'e', 5);
    testCapture("B:e5", SanProps::BISHOP, 'e', 5);
    testCapture("Be5:", SanProps::BISHOP, 'e', 5);
    testCapture("exd5", SanProps::PAWN, 'd', 5, 'e');
    testEnPassant("exd6e.p.", 'd', 6, 'e');
    testMove("Ngf3", SanProps::KNIGHT, 'f', 3, 'g');
    testMove("Ndf3", SanProps::KNIGHT, 'f', 3, 'd');
    testMove("N5f3", SanProps::KNIGHT, 'f', 3, 0, 5);
    testMove("N1f3", SanProps::KNIGHT, 'f', 3, 0, 1);
    testCapture("N5xf3", SanProps::KNIGHT, 'f', 3, 0, 5);
    testMove("Rdd5", SanProps::ROOK, 'd', 5, 'd');
    testMove("R3d5", SanProps::ROOK, 'd', 5, 0, 3);
    testCapture("Rdxd5", SanProps::ROOK, 'd', 5, 'd');
    testPromotion("e8Q", 'e', 8, SanProps::QUEEN);
    testPromotion("e8=Q", 'e', 8, SanProps::QUEEN);
    testCheck("Nd7+", SanProps::KNIGHT, 'd', 7);
    testCheck("Rb7+", SanProps::ROOK, 'b', 7);
    testCheck("Ba6+", SanProps::BISHOP, 'a', 6);
    testCheckmate("Rg7#", SanProps::ROOK, 'g', 7);
    testCheckmateCapture("Nxh6#", SanProps::KNIGHT, 'h', 6);
    testCheckmate("Rh4#", SanProps::ROOK, 'h', 4);
  } catch (std::runtime_error const & err) {
    std::cerr << "caught error while parsing san " << err.what() << std::endl;
  }
}

} // namespace Chessdb
