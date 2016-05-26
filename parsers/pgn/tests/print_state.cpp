#include "print_state.h"
#include <iostream>
#include "coord.h"
#include "state.h"
namespace Chessdb {

namespace {

class FileIter {
 public:
  virtual void reset() = 0;
  virtual operator bool () const = 0;
  virtual void operator++() = 0;
  char operator*() const {
    return file;
  }
 protected:
  int file;
};

class WhiteFileIter: public FileIter {
 public:
  virtual void reset() {
    file = 'a';
  }
  virtual operator bool () const {
    return file <= 'h';
  }
  virtual void operator++() {
    ++file;
  }
};

class BlackFileIter: public FileIter {
 public:
  virtual void reset() {
    file = 'h';
  }
  virtual operator bool () const {
    return 'a' <= file;
  }
  virtual void operator++() {
    --file;
  }
};

class RankIter {
 public:
  virtual void reset() = 0;
  virtual operator bool () const = 0;
  virtual void operator++() = 0;
  char operator*() const {
    return rank;
  }
 protected:
  char rank;
};

class WhiteRankIter: public RankIter {
 public:
  virtual void reset() {
    rank = 8;
  }
  virtual operator bool () const {
    return 1 <= rank;
  }
  virtual void operator++() {
    --rank;
  }
};

class BlackRankIter: public RankIter {
 public:
  virtual void reset() {
    rank = 1;
  }
  virtual operator bool () const {
    return rank <= 8;
  }
  virtual void operator++() {
    ++rank;
  }
};

void printFileLabels(FileIter & fileIt) {
  std::cout << "  ";
  for (fileIt.reset(); fileIt; ++fileIt) {
    std::cout << "   " << *fileIt;
  }
  std::cout << std::endl;
}

void printLine() {
  std::cout << "   ";
  for (char file = 'a'; file <= 'h'; ++file) {
    std::cout << "+---";
  }
  std::cout << "+" << std::endl;
}

void printRank(int rank, State const & state, FileIter & fileIt) {
  std::cout << " " << rank;
  for (fileIt.reset(); fileIt; ++fileIt) {
    std::cout << " | " << state.getPiece(Coord(*fileIt, rank));
  }
  std::cout << " | " << rank << std::endl;
}

void printRanks(State const & state, FileIter & fileIt, RankIter & rankIt) {
  for (rankIt.reset(); rankIt; ++rankIt) {
    printLine();
    printRank(*rankIt, state, fileIt);
  }
  printLine();
}

void printSideBoard(State const & state, FileIter & fileIt, RankIter & rankIt) {
  printFileLabels(fileIt);
  printRanks(state, fileIt, rankIt);
  printFileLabels(fileIt);
}

} // namespace

void printState(State const & state,
                BoardSide::Value const boardSide) {
  printBoard(state, boardSide);
  printTurn(state);
}

void printBoard(State const & state,
                BoardSide::Value const boardSide) {
  if (BoardSide::WHITE == boardSide) {
    printSideBoard(state, WhiteFileIter(), WhiteRankIter());
    return;
  }
  if (BoardSide::BLACK == boardSide) {
    printSideBoard(state, BlackFileIter(), BlackRankIter());
    return;
  }
  if (State::WHITE == state.getTurn()) {
    printSideBoard(state, WhiteFileIter(), WhiteRankIter());
  } else {
    printSideBoard(state, BlackFileIter(), BlackRankIter());
  }
}

void printTurn(State const & state) {
  std::cout << (State::WHITE == state.getTurn() ? "white" : "black") << "'s turn" << std::endl;
}

} // namespace Chessdb
