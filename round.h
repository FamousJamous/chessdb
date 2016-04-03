#ifndef ROUND_H
#define ROUND_H
#include <string>
namespace jl {

struct Round {
  Round(int num, std::string const & san, std::string const & comment):
    num(num), san(san), comment(comment) {
  }
  int num;
  std::string san;
  std::string comment;
};

} // namespace jl
#endif
