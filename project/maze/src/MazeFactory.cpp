#include "MazeFactory.h"

#include "KruskalMaze.h"
#include "EllerMaze.h"

#include <memory>

/*----------------------------------------------------------------------------*/

namespace maze {

/*----------------------------------------------------------------------------*/

std::unique_ptr< IMazeFactory > IMazeFactory::createFactory ()
{
    return std::make_unique< MazeFactory >();
}

/*----------------------------------------------------------------------------*/

std::unique_ptr< IMaze > MazeFactory::createMaze ( MazeAlgo _algo, int _size )
{
    if ( _algo == MazeAlgo::Kruskal )
    {
        return std::make_unique< KruskalMaze >( _size );
    }
    else if ( _algo == MazeAlgo::Eller )
    {
        return std::make_unique< EllerMaze >( _size );
    }

    return nullptr;
}

/*----------------------------------------------------------------------------*/

} // namespace maze

/*----------------------------------------------------------------------------*/

