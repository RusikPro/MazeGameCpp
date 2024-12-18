#ifndef PATHFINDER_H
#define PATHFINDER_H

/*----------------------------------------------------------------------------*/

#include "common/Point.h"

#include <memory>
#include <string>
#include <unordered_set>
#include <vector>

/*----------------------------------------------------------------------------*/

namespace maze {

class IMaze;

} // namespace maze

/*----------------------------------------------------------------------------*/

namespace pathfinding {

/*----------------------------------------------------------------------------*/

class Node;
using NodePtr = std::shared_ptr< Node >;

/*----------------------------------------------------------------------------*/

class Node
{
public:
    Point state;
    NodePtr parent;

    Node ( Point _state, NodePtr _pParent )
        :   state( _state )
        ,   parent( _pParent )
    {
    }
};

/*----------------------------------------------------------------------------*/

class Frontier
{
public:
    virtual void add ( NodePtr node ) = 0;
    virtual NodePtr remove () = 0;
    virtual bool empty () const = 0;
    virtual bool contains ( Point const & _state ) const = 0;
    virtual ~Frontier() = default;
};

/*----------------------------------------------------------------------------*/

class BaseFrontier
    :   public Frontier
{
public:
    bool contains ( Point const & _state ) const override;

protected:
    std::vector< NodePtr > frontier;
};

/*----------------------------------------------------------------------------*/

class StackFrontier : public BaseFrontier
{

public:
    void add ( NodePtr node ) override { frontier.push_back( node ); }
    NodePtr remove () override;
    bool empty () const override { return frontier.empty(); }
};

/*----------------------------------------------------------------------------*/

class QueueFrontier : public BaseFrontier
{
private:
    std::vector< NodePtr > frontier;

public:
    void add ( NodePtr node ) override { frontier.push_back( node ); }
    NodePtr remove () override;
    bool empty () const override { return frontier.empty(); }
};

/*----------------------------------------------------------------------------*/

class PathFinder
{
public:
    using Path = std::vector< Point >;

    explicit PathFinder ( maze::IMaze const & _maze );
    Path solve ( Point _start, Point _goal, bool useBFS );

private:
    maze::IMaze const & m_maze;
    int m_size;

    std::vector< Point > neighbors ( Point _state );
};

/*----------------------------------------------------------------------------*/

} // namespace pathfinding

/*----------------------------------------------------------------------------*/

#endif // PATHFINDER_H
