#ifndef TEST_UTILS_H
#define TEST_UTILS_H
#include <iostream>
#include <map>
#include <vector>
#include "move.h"
#include "round.h"
#include "tagpair.h"
namespace jl {

std::ostream & operator<<(std::ostream & os, Move const & move) {
  return os << "{ piece " << move.piece
            << " file " << move.file
            << " rank " << move.rank
            << " capture " << move.capture
            << " fromFile " << move.fromFile
            << " fromRank " << move.fromRank
            << " enPassant " << move.enPassant
            << " promoteTo " << move.promoteTo
            << " }";
}

std::ostream & operator<<(std::ostream & os, TagPair const & tagPair) {
  return os << "{ key " << tagPair.key << " value " << tagPair.value << " }";
}

std::ostream & operator<<(std::ostream & os, Round const & round) {
  return os << "{ num " << round.num << " san " << round.san
            << " comment " << round.comment << " }";
}

template <typename T>
std::ostream & operator<<(std::ostream & os, std::vector<T> const & v) {
  for (std::vector<T>::const_iterator it = v.begin(), end = v.end();
      it != end; ++it) {
    os << *it << std::endl;
  }
  return os;
}

template <typename K, typename V>
std::ostream & operator<<(std::ostream & os, std::map<K, V> const & m) {
  for (std::map<K, V>::const_iterator it = m.begin(), end = m.end();
      it != end; ++it) {
    os << it->first << ":" << it->second << std::endl;
  }
  return os;
}

} // namespace jl
#endif
