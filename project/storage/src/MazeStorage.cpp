#include "storage/MazeStorage.h"
#include <fstream>
#include <stdexcept>

/*----------------------------------------------------------------------------*/

namespace storage {

/*----------------------------------------------------------------------------*/

void
MazeStorage::save ( maze::IMaze const & _maze, std::string const & _filename )
{
    std::ofstream file( _filename );
    if ( !file )
    {
        throw std::runtime_error( "Failed to open file for saving" );
    }

    int size = _maze.getSize();
    file << size << '\n';
    auto const & grid = _maze.getGrid();
    for ( auto const & row : grid )
    {
        for ( auto const & room : row )
        {
            for ( bool wall : room.walls )
            {
                file << wall << ' ';
            }
            file << '\n';
        }
    }
}

/*----------------------------------------------------------------------------*/

void
MazeStorage::load ( maze::IMaze & _maze, std::string const & _filename )
{
    std::ifstream file( _filename );
    if ( !file )
    {
        throw std::runtime_error( "Failed to open file for loading" );
    }

    int size;
    file >> size;
    _maze.reset();
    auto & grid = _maze.takeGrid();
    for ( auto & row : grid )
    {
        for ( auto & room : row )
        {
            for ( bool & wall : room.walls )
            {
                file >> wall;
            }
        }
    }
}

/*----------------------------------------------------------------------------*/

} // namespace storage

/*----------------------------------------------------------------------------*/
