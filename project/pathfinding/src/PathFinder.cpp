#include "pathfinding/PathFinder.h"
#include "Frontier.h"

#include "maze/IMaze.h"
#include "common/Constants.h"

#include <unordered_set>

/*----------------------------------------------------------------------------*/

namespace pathfinding {

/*----------------------------------------------------------------------------*/

PathFinder::PathFinder ( maze::IMaze const & _maze )
    :   m_maze( _maze )
    ,   m_size( _maze.getSize() )
{
}

/*----------------------------------------------------------------------------*/

int PathFinder::getNeighbors (
        Point const & _state
    ,   Point outNeighbors[ MAX_NEIGHBORS ]
) const
{
    int count = 0;
    auto const & room = m_maze.getRoom( _state.x, _state.y );

    if ( !room.walls[ TOP_WALL ] && _state.y > 0 )
        outNeighbors[ count++ ] = { _state.x, _state.y - 1 };

    if ( !room.walls[ RIGHT_WALL ] && _state.x < m_size - 1 )
        outNeighbors[ count++ ] = { _state.x + 1, _state.y };

    if ( !room.walls[ BOTTOM_WALL ] && _state.y < m_size - 1 )
        outNeighbors[ count++ ] = { _state.x, _state.y + 1 };

    if (!room.walls[ LEFT_WALL ] && _state.x > 0)
        outNeighbors[ count++ ] = { _state.x - 1, _state.y };

    return count;
}

/*----------------------------------------------------------------------------*/

PathFinder::Path PathFinder::solve (
    Point const & _start, Point const & _goal, bool _useBFS
)
{
    std::unique_ptr< Frontier > frontier;

    if ( _useBFS )
        frontier = std::make_unique< QueueFrontier >();
    else
        frontier = std::make_unique< StackFrontier >();

    auto startNode = std::make_shared< Node >( _start, nullptr );
    frontier->add(startNode);

    std::unordered_set< Point > explored;

    while ( !frontier->empty() )
    {
        NodePtr node = frontier->remove();

        if ( node->state == _goal )
        {
            Path path;
            NodePtr current = node;
            while ( current )
            {
                path.push_back( current->state );
                current = current->parent;
            }
            std::reverse( path.begin(), path.end() );
            return path;
        }

        explored.insert( node->state );

        Point neighbors[ MAX_NEIGHBORS ];
        int n = getNeighbors( node->state, neighbors );

        // for ( auto const & neighbor : neighbors( node->state ) )
        for ( int i = 0; i < n; ++i )
        {
            auto const & neighbor = neighbors[ i ];
            if (    !frontier->contains( neighbor )
                &&  explored.find( neighbor ) == explored.end()
            )
            {
                auto child = std::make_shared< Node >( neighbor, node );
                frontier->add(child);
            }
        }
    }

    throw std::runtime_error( "No solution found!" );
}

/*----------------------------------------------------------------------------*/

} // namespace pathfinding

/*----------------------------------------------------------------------------*/
