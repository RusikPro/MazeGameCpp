#include "pathfinding/PathFinder.h"
#include "maze/IMaze.h"
#include "common/Constants.h"
#include <algorithm>
#include <iostream>

/*----------------------------------------------------------------------------*/

namespace pathfinding {

/*----------------------------------------------------------------------------*/

bool BaseFrontier::contains ( Point const & _state ) const
{
    for ( auto node : frontier )
    {
        if ( node->state == _state )
            return true;
    }
    return false;
}

/*----------------------------------------------------------------------------*/

NodePtr StackFrontier::remove ()
{
    if ( empty() )
        throw std::runtime_error( "Frontier is empty!" );
    NodePtr node = frontier.back();
    frontier.pop_back();
    return node;
}

/*----------------------------------------------------------------------------*/

NodePtr QueueFrontier::remove ()
{
    if ( empty() )
        throw std::runtime_error( "Frontier is empty!" );
    auto node = frontier.front();
    frontier.erase( frontier.begin() );
    return node;
}

/*----------------------------------------------------------------------------*/

PathFinder::PathFinder ( maze::IMaze const & _maze )
    :   m_maze( _maze )
    ,   m_size( _maze.getSize() )
{
}

/*----------------------------------------------------------------------------*/

std::vector<  Point > PathFinder::neighbors ( Point _state )
{
    std::vector< Point > result;
    auto const & room = m_maze.getRoom( _state.x, _state.y );

    if ( !room.walls[ TOP_WALL ] && _state.y > 0 )
        result.emplace_back( Point{ _state.x, _state.y - 1 } );

    if ( !room.walls[ RIGHT_WALL ] && _state.x < m_size - 1 )
        result.emplace_back( Point{ _state.x + 1, _state.y } );

    if ( !room.walls[ BOTTOM_WALL ] && _state.y < m_size - 1 )
        result.emplace_back( Point{ _state.x, _state.y + 1 } );

    if (!room.walls[ LEFT_WALL ] && _state.x > 0)
        result.emplace_back( Point{ _state.x - 1, _state.y } );

    return result;
}

/*----------------------------------------------------------------------------*/

PathFinder::Path PathFinder::solve ( Point _start, Point _goal, bool _useBFS )
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

        for ( auto const & neighbor : neighbors( node->state ) )
        {
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
