#include "BaseMaze.h"

#include "common/Constants.h"

#include <fstream>
#include <sstream>

/*----------------------------------------------------------------------------*/

namespace maze {

/*----------------------------------------------------------------------------*/

BaseMaze::BaseMaze ( int _size )
    :   m_size( _size )
{
    validateSize();
    reset();
}

/*----------------------------------------------------------------------------*/

void BaseMaze::validateSize () const
{
    if ( m_size > MAX_MAZE_SIZE )
    {
        std::ostringstream oss;
        oss << "BaseMaze size cannot exceed " << MAX_MAZE_SIZE
            << ". Provided size: " << m_size;
        throw std::invalid_argument( oss.str() );
    }
}

/*----------------------------------------------------------------------------*/

Grid const & BaseMaze::getGrid () const
{
    return m_grid;
}

/*----------------------------------------------------------------------------*/

int BaseMaze::getSize () const
{
    return m_size;
}

/*----------------------------------------------------------------------------*/

const Room& BaseMaze::getRoom ( int x, int y ) const
{
    return m_grid[ y ][ x ];
}

/*----------------------------------------------------------------------------*/

void BaseMaze::reset ()
{
    m_grid = Grid( m_size, Rooms( m_size ) );
}

/*----------------------------------------------------------------------------*/

void BaseMaze::save ( std::string const & _filename ) const
{
    std::ofstream file( _filename, std::ios::binary );
    if ( !file )
    {
        throw std::runtime_error( "Failed to open file for saving" );
    }

    file.write( reinterpret_cast< const char * >( &m_size ), sizeof( m_size ) );
    for ( const auto & row : m_grid )
    {
        for ( const auto & room : row )
        {
            file.write(
                    reinterpret_cast< const char * >( room.walls )
                ,   sizeof( room.walls )
            );
        }
    }
}

/*----------------------------------------------------------------------------*/

void BaseMaze::load ( std::string const & _filename )
{
    std::ifstream file( _filename, std::ios::binary );
    if ( !file )
    {
        throw std::runtime_error( "Failed to open file for loading" );
    }

    file.read( reinterpret_cast< char * >( &m_size ), sizeof( m_size ) );
    m_grid = Grid( m_size, Rooms( m_size ) );
    for ( auto & row : m_grid )
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

} // namespace maze

/*----------------------------------------------------------------------------*/
