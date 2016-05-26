#ifndef CHESSDB_PARSE_SAN_H
#define CHESSDB_PARSE_SAN_H
#include <string>
#include "san_props.h"
namespace Chessdb {

SanProps parseSan(std::string const & str);

} // namespace Chessdb
#endif
