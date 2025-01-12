#include "Frontier.h"

/*----------------------------------------------------------------------------*/

namespace pathfinding {

/*----------------------------------------------------------------------------*/

bool BaseFrontier::contains ( Point const & _state ) const
{
    for ( auto node : m_frontier )
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
    NodePtr node = m_frontier.back();
    m_frontier.pop_back();
    return node;
}

/*----------------------------------------------------------------------------*/

NodePtr QueueFrontier::remove ()
{
    if ( empty() )
        throw std::runtime_error( "Frontier is empty!" );
    auto node = m_frontier.front();
    m_frontier.erase( m_frontier.begin() );
    return node;
}

/*----------------------------------------------------------------------------*/

} // namespace pathfinding

/*----------------------------------------------------------------------------*/
