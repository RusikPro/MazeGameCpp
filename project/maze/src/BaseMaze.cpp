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

Grid & BaseMaze::takeGrid ()
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

} // namespace maze

/*----------------------------------------------------------------------------*/
