#ifndef IMAZEFACTORY_H
#define IMAZEFACTORY_H

/*----------------------------------------------------------------------------*/

#include <memory>

/*----------------------------------------------------------------------------*/

namespace maze {

/*----------------------------------------------------------------------------*/

class IMaze;

/*----------------------------------------------------------------------------*/

enum class MazeAlgo
{
    Eller,
    Kruskal
};

/*----------------------------------------------------------------------------*/

class IMazeFactory
{
public:

    virtual ~IMazeFactory () = default;

    virtual std::unique_ptr< IMaze > createMaze ( MazeAlgo, int ) = 0;

    static std::unique_ptr< IMazeFactory > createFactory ();

};

/*----------------------------------------------------------------------------*/

} // namespace maze

/*----------------------------------------------------------------------------*/

#endif // IMAZEFACTORY_H
