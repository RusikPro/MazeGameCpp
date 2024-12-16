#ifndef MAZE_H
#define MAZE_H

#include <vector>

struct Room
{
    bool walls[ 4 ] = { true, true, true, true }; // {top, right, bottom, left}
};

enum class RotationDirection
{
    Clockwise,
    Counterclockwise
};

class Maze
{
public:

    using Rooms = std::vector< Room >;
    using Grid = std::vector< Rooms >;
    using RowSets = std::vector< int >;

    Maze ( int _size );

    Grid const & getGrid () const;
    int getSize () const;
    const Room& getRoom ( int x, int y ) const;

    void reset ();

    void generateMaze (); // Generates the maze using Eller's algorithm

    void rotateGrid (
            RotationDirection direction = RotationDirection::Clockwise
        ,   int times = 1
    );

private:

    void validateSize () const;

    void initializeRowSets ( RowSets & _rowSets, int & _nextSetId ) const;
    void mergeHorizontalWalls ( int _row, RowSets & _rowSets );
    void createVerticalConnections ( int _row, RowSets & _rowSets );
    void prepareTheNextRow ( int _row, RowSets & _rowSets, int & _nextSetId );
    void handleTheLastRow ( RowSets & _rowSets );

private:

    int m_size;
    Grid m_grid;

    bool m_leftToRight;
};

#endif // MAZE_H
