#ifndef CHESSDB_INFER_MOVE_H
#define CHESSDB_INTER_MOVE_H
namespace Chessdb {

class Move;
class SanProps;
class State;

void inferMove(SanProps const & sanProps, State const & state, Move * move);

} // namespace Chessdb
#endif
