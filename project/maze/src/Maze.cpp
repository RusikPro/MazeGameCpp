#include "maze/Maze.h"
#include "common/Constants.h"
#include "common/FlexibleIterator.h"

#include <cstdlib>
#include <ctime>
#include <sstream>
#include <thread>
#include <unordered_map>

/*----------------------------------------------------------------------------*/

namespace maze {

/*----------------------------------------------------------------------------*/

Maze::Maze ( int _size )
    :   m_size( _size )
    ,   m_grid( _size, Rooms( _size ) )
    ,   m_leftToRight( true )
{
    validateSize();
    reset();
}

/*----------------------------------------------------------------------------*/

void Maze::validateSize () const
{
    if ( m_size > MAX_MAZE_SIZE )
    {
        std::ostringstream oss;
        oss << "Maze size cannot exceed " << MAX_MAZE_SIZE
            << ". Provided size: " << m_size;
        throw std::invalid_argument( oss.str() );
    }
}

/*----------------------------------------------------------------------------*/

Maze::Grid const & Maze::getGrid () const
{
    return m_grid;
}

/*----------------------------------------------------------------------------*/

int Maze::getSize () const
{
    return m_size;
}

/*----------------------------------------------------------------------------*/

const Room& Maze::getRoom ( int x, int y ) const
{
    return m_grid[ y ][ x ];
}

/*----------------------------------------------------------------------------*/

void Maze::reset ()
{
    m_grid = Grid( m_size, Rooms( m_size ) );
}

/*----------------------------------------------------------------------------*/

void Maze::generateMaze ()
{
    srand( static_cast< unsigned >( time( nullptr ) ) );

    // Step 1: Initialize row sets
    RowSets rowSets( m_size );
    int nextSetId = 0;

    initializeRowSets( rowSets, nextSetId );

    // Step 2: Process each row
    for ( int row = 0; row < m_size - 1; ++row, m_leftToRight = !m_leftToRight )
    {
        // Step 2.1: Merge horizontally
        mergeHorizontalWalls( row, rowSets );

        // Step 2.2: Create vertical connections
        createVerticalConnections( row, rowSets );

        // Step 2.4: Prepare the next row
        prepareTheNextRow( row, rowSets, nextSetId );
    }

    // Step 3: Handle the last row
    handleTheLastRow( rowSets );

    // Randomize the maze orientation at the end
    RotationDirection randomDirection = ( rand() % 2 == 0 )
            ?   RotationDirection::Clockwise
            :   RotationDirection::Counterclockwise;

    int randomRotations = rand() % 4; // Rotate between 0 and 3 times
    // rotateGrid( randomDirection, randomRotations );
}

/*----------------------------------------------------------------------------*/

void Maze::initializeRowSets ( RowSets & _rowSets, int & _nextSetId ) const
{
    for ( int col = 0; col < m_size; ++col )
    {
        _rowSets[ col ] = _nextSetId++;
    }
}

/*----------------------------------------------------------------------------*/

void Maze::mergeHorizontalWalls ( int _row, RowSets & _rowSets )
{
    for ( FlexibleIterator it( m_size - 1, m_leftToRight ); it.hasNext(); )
    {
        int col = it.next();
        if (    rand() % 100 < HORIZONTAL_MERGE_PROBABILITY
            &&  _rowSets[ col ] != _rowSets[ col + 1 ]
        )
        {
            // Remove the right wall
            m_grid[ _row ][ col ].walls[ RIGHT_WALL ] = false;
            m_grid[ _row ][ col + 1 ].walls[ LEFT_WALL ] = false;

            // Merge the sets
            int oldSet = _rowSets[ col + 1 ];
            int newSet = _rowSets[ col ];
            for ( int i = 0; i < m_size; ++i )
            {
                if ( _rowSets[ i ] == oldSet )
                {
                    _rowSets[ i ] = newSet;
                }
            }
        }
    }
}

/*----------------------------------------------------------------------------*/

void Maze::createVerticalConnections ( int _row, RowSets & _rowSets )
{
    std::unordered_map< int, bool > setHasVerticalConnection;

    for ( FlexibleIterator it( m_size, m_leftToRight ); it.hasNext(); )
    {
        int col = it.next();

        if (    rand() % 100 < VERTICAL_CONNECTION_PROBABILITY
            ||  !setHasVerticalConnection[ _rowSets[ col ] ]
        )
        {
            // Remove the bottom wall
            m_grid[ _row ][ col ].walls[ BOTTOM_WALL ] = false;
            m_grid[ _row + 1 ][ col ].walls[ TOP_WALL ] = false;

            setHasVerticalConnection[ _rowSets[ col ] ] = true;
        }
    }

    // Ensure every set has at least one vertical connection
    for ( FlexibleIterator it( m_size, m_leftToRight ); it.hasNext(); )
    {
        int col = it.next();
        if ( !setHasVerticalConnection[ _rowSets[ col ] ] )
        {
            m_grid[ _row ][ col ].walls[ BOTTOM_WALL ] = false;
            m_grid[ _row + 1 ][ col ].walls[ TOP_WALL ] = false;
            setHasVerticalConnection[ _rowSets[ col ] ] = true;
        }
    }
}

/*----------------------------------------------------------------------------*/

void Maze::prepareTheNextRow ( int _row, RowSets & _rowSets, int & _nextSetId )
{
    RowSets newRowSets( m_size, -1 );

    for ( FlexibleIterator it( m_size, m_leftToRight ); it.hasNext(); )
    {
        int col = it.next();
        if ( m_grid[ _row ][ col ].walls[ BOTTOM_WALL ] )
        {
            newRowSets[ col ] = _nextSetId++;
        }
        else
        {
            newRowSets[ col ] = _rowSets[ col ];
        }
    }
    _rowSets = newRowSets;
}

/*----------------------------------------------------------------------------*/

void Maze::handleTheLastRow ( RowSets & _rowSets )
{
    for ( FlexibleIterator it( m_size - 1, m_leftToRight ); it.hasNext(); )
    {
        int col = it.next();

        if ( _rowSets[ col ] != _rowSets[ col + 1 ] )
        {
            m_grid[ m_size - 1 ][ col ].walls[ RIGHT_WALL ] = false;
            m_grid[ m_size - 1 ][ col + 1 ].walls[ LEFT_WALL ] = false;

            // Merge the sets
            int oldSet = _rowSets[ col + 1 ];
            int newSet = _rowSets[ col ];
            for ( int i = 0; i < m_size; ++i )
            {
                if ( _rowSets[ i ] == oldSet )
                {
                    _rowSets[ i ] = newSet;
                }
            }
        }
    }
}

/*----------------------------------------------------------------------------*/

void Maze::rotateGrid ( RotationDirection direction, int _times )
{
    _times = _times % 4;
    if ( _times == 0 ) return;

    for ( int t = 0; t < _times; ++t )
    {
        Grid rotatedGrid( m_size, Rooms( m_size ) );

        std::vector< std::thread > threads;

        const int numThreads = 4;
        const int chunkSize = m_size / numThreads;

        for ( int threadId = 0; threadId < numThreads; ++threadId )
        {
            threads.emplace_back( [ =, &rotatedGrid ] ()
            {
                const int startRow = threadId * chunkSize;
                const int endRow = ( threadId == numThreads - 1 )
                    ?   m_size
                    :   ( threadId + 1 ) * chunkSize
                ;

                for ( int row = startRow; row < endRow; ++row )
                {
                    for ( int col = 0; col < m_size; ++col )
                    {
                        if ( direction == RotationDirection::Clockwise )
                        {
                            rotatedGrid[ col ][ m_size - 1 - row ]
                                = m_grid[ row ][ col ];
                            rotatedGrid[ col ][ m_size - 1 - row ].walls[ TOP_WALL ]
                                = m_grid[ row ][ col ].walls[ LEFT_WALL ];
                            rotatedGrid[ col ][ m_size - 1 - row ].walls[ RIGHT_WALL ]
                                = m_grid[ row ][ col ].walls[ TOP_WALL ];
                            rotatedGrid[ col ][ m_size - 1 - row ].walls[ BOTTOM_WALL ]
                                = m_grid[ row ][ col ].walls[ RIGHT_WALL ];
                            rotatedGrid[ col ][ m_size - 1 - row ].walls[ LEFT_WALL ]
                                = m_grid[ row ][ col ].walls[ BOTTOM_WALL ];
                        }
                        else
                        {
                            rotatedGrid[ m_size - 1 - col ][ row ]
                                = m_grid[ row ][ col ];
                            rotatedGrid[ m_size - 1 - col ][ row ].walls[ TOP_WALL ]
                                = m_grid[ row ][ col ].walls[ RIGHT_WALL ];
                            rotatedGrid[ m_size - 1 - col ][ row ].walls[ RIGHT_WALL ]
                                = m_grid[ row ][ col ].walls[ BOTTOM_WALL ];
                            rotatedGrid[ m_size - 1 - col ][ row ].walls[ BOTTOM_WALL ]
                                = m_grid[ row ][ col ].walls[ LEFT_WALL ];
                            rotatedGrid[ m_size - 1 - col ][ row ].walls[ LEFT_WALL ]
                                = m_grid[ row ][ col ].walls[ TOP_WALL ];
                        }
                    }
                }
            } );
        }

        for ( auto& thread : threads )
        {
            thread.join();
        }

        m_grid = rotatedGrid;
    }
}

/*----------------------------------------------------------------------------*/

} // namespace maze

/*----------------------------------------------------------------------------*/
