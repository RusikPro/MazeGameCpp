#ifndef MAZEFACTORY_H
#define MAZEFACTORY_H

/*----------------------------------------------------------------------------*/

#include "maze/IMazeFactory.h"

#include <memory>

/*----------------------------------------------------------------------------*/

namespace maze {

/*----------------------------------------------------------------------------*/

class MazeFactory
    :   public IMazeFactory
{
public:

    std::unique_ptr< IMaze > createMaze ( MazeAlgo _algo, int _size ) override;

};

/*----------------------------------------------------------------------------*/

} // namespace maze

/*----------------------------------------------------------------------------*/

#endif // MAZEFACTORY_H
