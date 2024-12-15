#include "Maze.h"
#include "Constants.h"

#include <cstdlib>
#include <ctime>
#include <sstream>
#include <thread>
#include <unordered_map>

/*----------------------------------------------------------------------------*/

Maze::Maze ( int _size )
    :   m_size( _size )
    ,   m_grid( _size, std::vector< Room >( _size ) )
{
    if ( _size > MAX_MAZE_SIZE )
    {
        std::ostringstream oss;
        oss << "Maze size cannot exceed " << MAX_MAZE_SIZE
            << ". Provided size: " << _size;
        throw std::invalid_argument( oss.str() );
    }

    generateMaze();
}

/*----------------------------------------------------------------------------*/

std::vector< std::vector< Room > > const & Maze::getGrid () const
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

void Maze::generateMaze ()
{
    srand( static_cast< unsigned >( time( nullptr ) ) );

    // Step 1: Initialize row sets
    std::vector< std::vector< Room > > grid( m_size, std::vector< Room >( m_size ) );
    std::vector< int > rowSets( m_size );
    int nextSetId = 0;

    for ( int col = 0; col < m_size; ++col )
    {
        rowSets[ col ] = nextSetId++;
    }

    // Step 2: Process each row
    for ( int row = 0; row < m_size; ++row )
    {
        // Step 2.1: Merge horizontally
        for ( int col = 0; col < m_size - 1; ++col )
        {
            if (    rand() % 100 < HORIZONTAL_MERGE_PROBABILITY
                &&  rowSets[ col ] != rowSets[ col + 1 ]
            )
            {
                // Remove the right wall
                grid[ row ][ col ].walls[ RIGHT_WALL ] = false;
                grid[ row ][ col + 1 ].walls[ LEFT_WALL ] = false;

                // Merge the sets
                int oldSet = rowSets[ col + 1 ];
                int newSet = rowSets[ col ];
                for ( int i = 0; i < m_size; ++i )
                {
                    if ( rowSets[ i ] == oldSet )
                    {
                        rowSets[ i ] = newSet;
                    }
                }
            }
        }

        // Step 2.2: Create vertical connections
        if ( row < m_size - 1 )
        {
            std::unordered_map< int, bool > setHasVerticalConnection;

            // Determine the number of columns to treat as "priority columns" for vertical connections
            int priorityColumns = static_cast< int >(
                m_size * VERTICAL_CONNECTION_PRIORITY_PERCENTAGE
            );


            for ( int col = 0; col < m_size; ++col )
            {
                // Check if this column is within the "priority" subset
                bool isPriorityColumn = col < priorityColumns;

                bool retainBottomWall =
                    ( isPriorityColumn && rand() % 100 < LEFTMOST_BOTTOM_WALL_PROBABILITY )
                ;

                if (    !retainBottomWall
                    &&  (   rand() % 100 < VERTICAL_CONNECTION_PROBABILITY
                        ||  !setHasVerticalConnection[ rowSets[ col ] ]
                        )
                )
                {
                    // Remove the bottom wall
                    grid[ row ][ col ].walls[ BOTTOM_WALL ] = false;
                    grid[ row + 1 ][ col ].walls[ TOP_WALL ] = false;

                    setHasVerticalConnection[ rowSets[ col ] ] = true;
                }
            }

            // Force a vertical connection for the leftmost column (col = 0)
            if (!grid[ row ][ 0 ].walls[ BOTTOM_WALL ])
            {
                grid[ row ][ 0 ].walls[ BOTTOM_WALL ] = false;
                grid[ row + 1 ][ 0 ].walls[ TOP_WALL ] = false;
                setHasVerticalConnection[ rowSets[ 0 ] ] = true;
            }

            // Ensure vertical connections for the rightmost column as well
            if ( !setHasVerticalConnection[ rowSets[ m_size - 1 ] ] )
            {
                grid[ row ][ m_size - 1 ].walls[ BOTTOM_WALL ] = false;
                grid[ row + 1 ][ m_size - 1 ].walls[ TOP_WALL ] = false;
                setHasVerticalConnection[ rowSets[ m_size - 1 ] ] = true;
            }

            // Step 2.3: Ensure every set has at least one vertical connection
            // Randomly decide whether to iterate left-to-right or right-to-left
            bool iterateLeftToRight = ( rand() % 2 == 0 ); // 50% chance for each direction
            if ( iterateLeftToRight )
            {
                for ( int col = 0; col < m_size; ++col )
                {
                    if ( !setHasVerticalConnection[ rowSets[ col ] ] )
                    {
                        grid[ row ][ col ].walls[ BOTTOM_WALL ] = false;
                        grid[ row + 1 ][ col ].walls[ TOP_WALL ] = false;
                        setHasVerticalConnection[ rowSets[ col ] ] = true;
                    }
                }
            }
            else
            {
                for ( int col = m_size - 1; col >= 0; --col )
                {
                    if ( !setHasVerticalConnection[ rowSets[ col ] ] )
                    {
                        grid[ row ][ col ].walls[ BOTTOM_WALL ] = false;
                        grid[ row + 1 ][ col ].walls[ TOP_WALL ] = false;
                        setHasVerticalConnection[ rowSets[ col ]] = true;
                    }
                }
            }
        }

        // Step 2.4: Prepare the next row
        if ( row < m_size - 1 )
        {
            std::vector< int > newRowSets( m_size, -1 );
            for ( int col = 0; col < m_size; ++col )
            {
                if ( grid[ row ][ col ].walls[ BOTTOM_WALL ] )
                {
                    newRowSets[ col ] = nextSetId++;
                }
                else
                {
                    newRowSets[ col ] = rowSets[ col ];
                }
            }
            rowSets = newRowSets;
        }
    }

    // Step 3: Handle the last row
    for ( int col = 0; col < m_size - 1; ++col )
    {
        if ( rowSets[ col ] != rowSets[ col + 1 ] )
        {
            // Remove the right wall
            grid[ m_size - 1 ][ col ].walls[ RIGHT_WALL ] = false;
            grid[ m_size - 1 ][ col + 1 ].walls[ LEFT_WALL ] = false;

            // Merge the sets
            int oldSet = rowSets[ col + 1 ];
            int newSet = rowSets[ col ];
            for ( int i = 0; i < m_size; ++i )
            {
                if ( rowSets[ i ] == oldSet )
                {
                    rowSets[ i ] = newSet;
                }
            }
        }
    }

    // Save the grid to the maze's internal structure
    this->m_grid = grid;

    // Randomize the maze orientation at the end
    RotationDirection randomDirection = (rand() % 2 == 0)
            ?   RotationDirection::Clockwise
            :   RotationDirection::Counterclockwise;

    int randomRotations = rand() % 4; // Rotate between 0 and 3 times
    rotateGrid( randomDirection, randomRotations );
}

/*----------------------------------------------------------------------------*/

void Maze::rotateGrid ( RotationDirection direction, int _times )
{
    _times = _times % 4;
    if ( _times == 0 ) return;

    for ( int t = 0; t < _times; ++t )
    {
        std::vector< std::vector< Room > > rotatedGrid(
            m_size, std::vector< Room >( m_size )
        );
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
