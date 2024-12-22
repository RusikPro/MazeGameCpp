#ifndef PATHFINDER_H
#define PATHFINDER_H

/*----------------------------------------------------------------------------*/

#include "common/Point.h"

#include <vector>

/*----------------------------------------------------------------------------*/

namespace maze {

class IMaze;

} // namespace maze

/*----------------------------------------------------------------------------*/

namespace pathfinding {

/*----------------------------------------------------------------------------*/

class PathFinder
{
public:
    using Path = std::vector< Point >;

    explicit PathFinder ( maze::IMaze const & _maze );
    Path solve ( Point const & _start, Point const & _goal, bool _useBFS );

private:
    maze::IMaze const & m_maze;
    int m_size;

    std::vector< Point > neighbors ( Point const & _state );
};

/*----------------------------------------------------------------------------*/

} // namespace pathfinding

/*----------------------------------------------------------------------------*/

#endif // PATHFINDER_H
