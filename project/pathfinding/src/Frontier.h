#include "common/Point.h"
#include <memory>

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
    std::vector< NodePtr > m_frontier;
};

/*----------------------------------------------------------------------------*/

class StackFrontier : public BaseFrontier
{

public:
    void add ( NodePtr _node ) override { m_frontier.push_back( _node ); }
    NodePtr remove () override;
    bool empty () const override { return m_frontier.empty(); }
};

/*----------------------------------------------------------------------------*/

class QueueFrontier : public BaseFrontier
{
private:
    std::vector< NodePtr > m_frontier;

public:
    void add ( NodePtr _node ) override { m_frontier.push_back( _node ); }
    NodePtr remove () override;
    bool empty () const override { return m_frontier.empty(); }
};

/*----------------------------------------------------------------------------*/

} // namespace pathfinding

/*----------------------------------------------------------------------------*/
