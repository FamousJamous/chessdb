#include "infer_move.h"
#include <sstream>
#include <stdexcept>
#include <vector>
#include "coord.h"
#include "move.h"
#include "san_props.h"
#include "state.h"
namespace Chessdb {

namespace {

std::ostream & operator<<(std::ostream & os, Coord const & coord) {
  return os << "[ " << coord.file << ", " << coord.rank << " ]";
}

Coord getBackOne(SanProps const & sanProps, State const & state) {
  int rank = sanProps.rank + (State::WHITE == state.getTurn() ? -1 : 1);
  return Coord(sanProps.file, rank);
}

bool outOfRange(Coord const & coord) {
  return coord.file < 'a' || 'h' < coord.file ||
         coord.rank < 1 || 8 < coord.rank;
}

bool isOwn(Coord const & coord, State const & state, char lookFor) {
  if (outOfRange(coord)) {
    return false;
  }
  char piece = state.getPiece(coord);
  if (State::BLACK == state.getTurn()) {
    lookFor -= ('a' - 'A');
  }
  return piece == lookFor;
}

bool isOwnPawn(Coord const & coord, State const & state) {
  return isOwn(coord, state, 'p');
}

bool isEmpty(Coord const & coord, State const & state) {
  return ' ' == state.getPiece(coord);
}

void inferPawnCapture(SanProps const & sanProps, State const & state,
                      Coord * from, Coord * enPassCap) {
  Coord backOne = getBackOne(sanProps, state);
  if (1 != std::abs(backOne.file - sanProps.fromFile)) {
    std::ostringstream err;
    err << "expected file difference of one for pawn capture."
        << " got file " << sanProps.file
        << " from file " << sanProps.fromFile;
    throw std::runtime_error(err.str());
  }
  if (isEmpty(Coord(sanProps.file, sanProps.rank), state)) {
    *enPassCap = backOne;
  }
  backOne.file = sanProps.fromFile;
  if (!isOwnPawn(backOne, state)) {
    std::ostringstream err;
    err << "expected own pawn at " << backOne.file << backOne.rank;
    throw std::runtime_error(err.str());
  }
  *from = backOne;
}

Coord getBackTwo(SanProps const & sanProps, State const & state) {
  int rank = sanProps.rank + (State::WHITE == state.getTurn() ? -2 : 2);
  return Coord(sanProps.file, rank);
}

char getPromPiece(SanProps::Piece const promoteTo) {
  switch (promoteTo) {
    case SanProps::ROOK:
      return 'R';
    case SanProps::KNIGHT:
      return 'N';
    case SanProps::BISHOP:
      return 'B';
    case SanProps::QUEEN:
      return 'Q';
    default:
      std::ostringstream err;
      err << "unexpected promotion piece " << promoteTo;
      throw std::runtime_error(err.str());
  }
}

void setPromotion(SanProps const & sanProps, Move * move) {
  if (!sanProps.isPromotion) {
    return;
  }
  move->promPiece = getPromPiece(sanProps.promoteTo);
}

void inferPawnMove(SanProps const & sanProps, State const & state,
                   Move * move) {
  setPromotion(sanProps, move);
  Coord * from = &move->fromTo.from;
  if (sanProps.fromFile) {
    return inferPawnCapture(sanProps, state, from, &move->enPassCap);
  }
  /* capture without from file not supported yet
  if (sanProps.isCapture) {
    return inferPawnCaptureFrom(sanProps, state, from);
  }
  */
  Coord backOne = getBackOne(sanProps, state);
  if (isOwnPawn(backOne, state)) {
    *from = backOne;
    return;
  }
  Coord backTwo = getBackTwo(sanProps, state);
  if (isEmpty(backOne, state) && isOwnPawn(backTwo, state)) {
    *from = backTwo;
    return;
  }
  throw std::runtime_error("can't find viable pawn");
}

bool isOwnRook(Coord const & coord, State const & state) {
  return isOwn(coord, state, 'r');
}

Coord getLeft(SanProps const & sanProps, State const & state) {
  char file = sanProps.file - 1;
  int rank = sanProps.rank;
  while ('a' <= file && isEmpty(Coord(file, rank), state)) {
    --file;
  }
  return Coord(file, rank);
}

Coord getRight(SanProps const & sanProps, State const & state) {
  char file = sanProps.file + 1;
  int rank = sanProps.rank;
  while (file <= 'h' && isEmpty(Coord(file, rank), state)) {
    ++file;
  }
  return Coord(file, rank);
}

Coord getUp(SanProps const & sanProps, State const & state) {
  char file = sanProps.file;
  int rank = sanProps.rank + 1;
  while (rank <= 8 && isEmpty(Coord(file, rank), state)) {
    ++rank;
  }
  return Coord(file, rank);
}

Coord getDown(SanProps const & sanProps, State const & state) {
  char file = sanProps.file;
  int rank = sanProps.rank - 1;
  while (1 <= rank && isEmpty(Coord(file, rank), state)) {
    --rank;
  }
  return Coord(file, rank);
}

void getStraightFroms(SanProps const & sanProps, State const & state,
                      bool (*isOwnPiece)(Coord const &, State const &),
                      std::vector<Coord> * froms) {
  Coord left = getLeft(sanProps, state);
  if (isOwnPiece(left, state)) {
    froms->push_back(left);
  }
  Coord right = getRight(sanProps, state);
  if (isOwnPiece(right, state)) {
    froms->push_back(right);
  }
  Coord up = getUp(sanProps, state);
  if (isOwnPiece(up, state)) {
    froms->push_back(up);
  }
  Coord down = getDown(sanProps, state);
  if (isOwnPiece(down, state)) {
    froms->push_back(down);
  }
}

bool disambiguateFromsBySan(SanProps const & sanProps,
                            std::vector<Coord> const & froms,
                            Coord * from) {
  char fromFile = sanProps.fromFile;
  int fromRank = sanProps.fromRank;
  for (int i = 0, n = froms.size(); i < n; ++i) {
    Coord const & fromI = froms[i];
    if (fromFile == fromI.file) {
      *from = fromI;
      return true;
    }
    if (fromRank == fromI.rank) {
      *from = fromI;
      return true;
    }
  }
  return false;
}

State applyFrom(Coord const & from, State const & state, Move * move) {
  move->fromTo.from = from;
  State copy(state);
  copy.makeMove(*move);
  return copy;
}

Coord findOtherKing(State const & state) {
  char kingCh = (State::WHITE == state.getTurn()) ? 'K' : 'k';
  for (char file = 'a'; file <= 'h'; ++file) {
    for (int rank = 1; rank <= 8; ++rank) {
      Coord coord(file, rank);
      if (kingCh == state.getPiece(coord)) {
        return coord;
      }
    }
  }
  std::ostringstream err;
  err << "couldn't find " << kingCh << " on board";
  throw std::runtime_error(err.str());
}

bool inCheck(State const & state) {
  Coord king = findOtherKing(state);
  SanProps sanProps;
  sanProps.file = king.file;
  sanProps.rank = king.rank;

  Move dum;

  SanProps::Piece pieces[] = { SanProps::ROOK, SanProps::KNIGHT,
                               SanProps::BISHOP, SanProps::QUEEN };
  for (int i = 0, n = sizeof(pieces)/sizeof(pieces[0]); i < n; ++i) {
    sanProps.piece = pieces[i];
    try {
      inferMove(sanProps, state, &dum);
    } catch (std::runtime_error const & err) {
      continue;
    }
    return true;
  }
  return false;
}

bool disambiguateFromsByCheck(SanProps const & sanProps,
                              std::vector<Coord> const & froms,
                              State const & state,
                              Coord * from) {
  Move move;
  move.fromTo = FromTo(Coord(),
                       Coord(sanProps.file, sanProps.rank));
  for (int i = 0, n = froms.size(); i < n; ++i) {
    State copy = applyFrom(froms[i], state, &move);
    if (!inCheck(copy)) {
      *from = froms[i];
      return true;
    }
  }
  return false;
}

void disambiguateFroms(SanProps const & sanProps,
                       std::vector<Coord> const & froms,
                       std::string const & name,
                       State const & state,
                       Coord * from) {
  if (1 == froms.size()) {
    *from = froms[0];
    return;
  }
  if (disambiguateFromsBySan(sanProps, froms, from)) {
    return;
  }
  if (disambiguateFromsByCheck(sanProps, froms, state, from)) {
    return;
  }
  std::ostringstream err;
  err << "unable to disambiguate " << name << " moves to "
      << sanProps.file << sanProps.rank;
  throw std::runtime_error(err.str());
}

void inferRookFrom(SanProps const & sanProps, State const & state, Coord * from) {
  std::vector<Coord> froms;
  getStraightFroms(sanProps, state, isOwnRook, &froms);
  disambiguateFroms(sanProps, froms, "rook", state, from);
}

bool isOwnKnight(Coord const & coord, State const & state) {
  return isOwn(coord, state, 'n');
}

void pushKnightMove(SanProps const & sanProps, State const & state,
                    char fileOffset, int rankOffset,
                    std::vector<Coord> * froms) {
  char file = sanProps.file + fileOffset;
  if (file < 'a' || 'h' < file) {
    return;
  }
  int rank = sanProps.rank + rankOffset;
  if (rank < 1 || 8 < rank) {
    return;
  }
  Coord coord(file, rank);
  if (isOwnKnight(coord, state)) {
    froms->push_back(coord);
  }
}

void getKnightFroms(SanProps const & sanProps, State const & state,
                    std::vector<Coord> * froms) {
  pushKnightMove(sanProps, state, 2, 1, froms);
  pushKnightMove(sanProps, state, 2, -1, froms);
  pushKnightMove(sanProps, state, -2, 1, froms);
  pushKnightMove(sanProps, state, -2, -1, froms);
  pushKnightMove(sanProps, state, 1, 2, froms);
  pushKnightMove(sanProps, state, 1, -2, froms);
  pushKnightMove(sanProps, state, -1, 2, froms);
  pushKnightMove(sanProps, state, -1, -2, froms);
}

void inferKnightFrom(SanProps const & sanProps, State const & state, Coord * from) {
  std::vector<Coord> froms;
  getKnightFroms(sanProps, state, &froms);
  disambiguateFroms(sanProps, froms, "knight", state, from);
}

Coord getDiagUpLeft(SanProps const & sanProps, State const & state) {
  char file = sanProps.file - 1;
  int rank = sanProps.rank + 1;
  while ('a' < file && rank < 8 && isEmpty(Coord(file, rank), state)) {
    --file;
    ++rank;
  }
  return Coord(file, rank);
}

Coord getDiagUpRight(SanProps const & sanProps, State const & state) {
  char file = sanProps.file + 1;
  int rank = sanProps.rank + 1;
  while (file < 'h' && rank < 8 && isEmpty(Coord(file, rank), state)) {
    ++file;
    ++rank;
  }
  return Coord(file, rank);
}

Coord getDiagDownLeft(SanProps const & sanProps, State const & state) {
  char file = sanProps.file - 1;
  int rank = sanProps.rank - 1;
  while ('a' < file && 1 < rank && isEmpty(Coord(file, rank), state)) {
    --file;
    --rank;
  }
  return Coord(file, rank);
}

Coord getDiagDownRight(SanProps const & sanProps, State const & state) {
  char file = sanProps.file + 1;
  int rank = sanProps.rank - 1;
  while (file < 'h' && 1 < rank && isEmpty(Coord(file, rank), state)) {
    ++file;
    --rank;
  }
  return Coord(file, rank);
}

bool isOwnBishop(Coord const & coord, State const & state) {
  return isOwn(coord, state, 'b');
}

void getDiagFroms(SanProps const & sanProps, State const & state,
                  bool (*isOwnPiece)(Coord const &, State const &),
                  std::vector<Coord> * froms) {
  Coord diagUpLeft = getDiagUpLeft(sanProps, state);
  if (isOwnPiece(diagUpLeft, state)) {
    froms->push_back(diagUpLeft);
  }
  Coord diagUpRight = getDiagUpRight(sanProps, state);
  if (isOwnPiece(diagUpRight, state)) {
    froms->push_back(diagUpRight);
  }
  Coord diagDownLeft = getDiagDownLeft(sanProps, state);
  if (isOwnPiece(diagDownLeft, state)) {
    froms->push_back(diagDownLeft);
  }
  Coord diagDownRight = getDiagDownRight(sanProps, state);
  if (isOwnPiece(diagDownRight, state)) {
    froms->push_back(diagDownRight);
  }
}

void inferBishopFrom(SanProps const & sanProps, State const & state, Coord * from) {
  std::vector<Coord> froms;
  getDiagFroms(sanProps, state, isOwnBishop, &froms);
  disambiguateFroms(sanProps, froms, "bishop", state, from);
}

bool isOwnQueen(Coord const & coord, State const & state) {
  return isOwn(coord, state, 'q');
}

void inferQueenFrom(SanProps const & sanProps, State const & state, Coord * from) {
  std::vector<Coord> froms;
  getDiagFroms(sanProps, state, isOwnQueen, &froms);
  getStraightFroms(sanProps, state, isOwnQueen, &froms);
  disambiguateFroms(sanProps, froms, "queen", state, from);
}

bool isOwnKing(Coord const & coord, State const & state) {
  return isOwn(coord, state, 'k');
}

bool isKingFrom(SanProps const & sanProps, State const & state, char fileOffset,
                int rankOffset, Coord * from) {
  char file = sanProps.file + fileOffset;
  if (file < 'a' || 'h' < file) {
    return false;
  }
  int rank = sanProps.rank + rankOffset;
  if (rank < 1 || 8 < rank) {
    return false;
  }
  Coord coord(file, rank);
  if (isOwnKing(coord, state)) {
    *from = coord;
    return true;
  }
  return false;
}

void inferKingFrom(SanProps const & sanProps, State const & state, Coord * from) {
  isKingFrom(sanProps, state, -1, 1, from) ||
  isKingFrom(sanProps, state, -1, 0, from) ||
  isKingFrom(sanProps, state, -1, -1, from) ||
  isKingFrom(sanProps, state, 0, 1, from) ||
  isKingFrom(sanProps, state, 0, -1, from) ||
  isKingFrom(sanProps, state, 1, 1, from) ||
  isKingFrom(sanProps, state, 1, 0, from) ||
  isKingFrom(sanProps, state, 1, -1, from);
}

void inferMovePiece(SanProps const & sanProps, State const & state, Move * move) {
  FromTo & fromTo = move->fromTo;
  fromTo.to = Coord(sanProps.file, sanProps.rank);
  switch (sanProps.piece) {
    case SanProps::PAWN:
      return inferPawnMove(sanProps, state, move);
    case SanProps::ROOK:
      return inferRookFrom(sanProps, state, &fromTo.from);
    case SanProps::KNIGHT:
      return inferKnightFrom(sanProps, state, &fromTo.from);
    case SanProps::BISHOP:
      return inferBishopFrom(sanProps, state, &fromTo.from);
    case SanProps::QUEEN:
      return inferQueenFrom(sanProps, state, &fromTo.from);
    case SanProps::KING:
      return inferKingFrom(sanProps, state, &fromTo.from);
    default:
      std::ostringstream err;
      err << "unsupported piece type " << sanProps.piece;
      throw std::runtime_error(err.str());
  }
}

void pushCastleFromTos(char kingFromFile, char kingToFile,
                       char rookFromFile, char rookToFile,
                       State::Color turn, Move * move) {
  int rank = (State::WHITE == turn) ? 1 : 8;
  move->fromTo = FromTo(Coord(kingFromFile, rank),
                        Coord(kingToFile, rank));
  move->castle = FromTo(Coord(rookFromFile, rank),
                        Coord(rookToFile, rank));
}

void inferKingCastle(State::Color turn, Move * move) {
  pushCastleFromTos('e', 'g', 'h', 'f', turn, move);
}

void inferQueenCastle(State::Color turn, Move * move) {
  pushCastleFromTos('e', 'c', 'a', 'd', turn, move);
}

} // namespace

void inferMove(SanProps const & sanProps, State const & state, Move * move) {
  move->check = sanProps.isCheck;
  switch (sanProps.type) {
    case SanProps::MOVE_PIECE:
      return inferMovePiece(sanProps, state, move);
    case SanProps::KING_CASTLE:
      return inferKingCastle(state.getTurn(), move);
    case SanProps::QUEEN_CASTLE:
      return inferQueenCastle(state.getTurn(), move);
    default:
      std::ostringstream err;
      err << "nothing to infer for san props type " << sanProps.type;
      throw std::runtime_error(err.str());
  }
}

} // namespace Chessdb
