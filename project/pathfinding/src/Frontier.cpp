#include "Frontier.h"

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

} // namespace pathfinding

/*----------------------------------------------------------------------------*/
