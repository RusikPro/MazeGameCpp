#include "storage/MazeStorage.h"
#include <fstream>
#include <stdexcept>

/*----------------------------------------------------------------------------*/

namespace storage {

/*----------------------------------------------------------------------------*/

void
MazeStorage::save ( maze::IMaze const & _maze, std::string const & _filename )
{
    std::ofstream file( _filename, std::ios::binary );
    if ( !file )
    {
        throw std::runtime_error( "Failed to open file for saving" );
    }

    int size = _maze.getSize();

    file.write( reinterpret_cast< const char * >( &size ), sizeof( size ) );
    const auto& grid = _maze.getGrid();
    for ( auto const & row : grid )
    {
        for ( auto const & room : row )
        {
            file.write(
                    reinterpret_cast< const char * >( room.walls )
                ,   sizeof( room.walls )
            );
        }
    }
}

/*----------------------------------------------------------------------------*/

void
MazeStorage::load ( maze::IMaze & _maze, std::string const & _filename )
{
    std::ifstream file( _filename, std::ios::binary );
    if ( !file )
    {
        throw std::runtime_error( "Failed to open file for loading" );
    }

    int size;
    file.read( reinterpret_cast< char * >( &size ), sizeof( size ) );
    _maze.reset();
    auto & grid = _maze.takeGrid();
    for ( auto & row : grid )
    {
        for ( auto & room : row )
        {
            file.read(
                reinterpret_cast< char * >( room.walls ), sizeof( room.walls )
            );
        }
    }
}

/*----------------------------------------------------------------------------*/

} // namespace storage

/*----------------------------------------------------------------------------*/
