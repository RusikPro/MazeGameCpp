#ifndef PATHFINDER_H
#define PATHFINDER_H

/*----------------------------------------------------------------------------*/

#include "maze/Maze.h"
#include "common/Point.h"

#include <memory>
#include <string>
#include <unordered_set>
#include <vector>

/*----------------------------------------------------------------------------*/

class Node
{
public:
    Point state;
    Node * parent;

    Node ( Point _state, Node * _pParent )
        :   state( _state )
        ,   parent( _pParent )
    {
    }
};

/*----------------------------------------------------------------------------*/

class Frontier
{
public:
    virtual void add ( Node * node ) = 0;
    virtual Node * remove () = 0;
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
    std::vector< Node * > frontier;
};

/*----------------------------------------------------------------------------*/

class StackFrontier : public BaseFrontier
{

public:
    void add ( Node * node ) override { frontier.push_back( node ); }
    Node * remove () override;
    bool empty () const override { return frontier.empty(); }
};

/*----------------------------------------------------------------------------*/

class QueueFrontier : public BaseFrontier
{
private:
    std::vector< Node * > frontier;

public:
    void add ( Node * node ) override { frontier.push_back( node ); }
    Node * remove () override;
    bool empty () const override { return frontier.empty(); }
};

/*----------------------------------------------------------------------------*/

class PathFinder
{
public:
    using Path = std::vector<Point>;

    explicit PathFinder(const Maze &maze);
    Path solve(Point start, Point goal, bool useBFS);

private:
    const Maze &m_maze;
    int m_size;

    std::vector< Point > neighbors ( Point _state );
};

#endif // PATHFINDER_H
