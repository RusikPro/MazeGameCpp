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
    Maze ( int _size );

    std::vector< std::vector< Room > > const & getGrid () const;
    int getSize () const;
    const Room& getRoom ( int x, int y ) const;

    void generateMaze (); // Generates the maze using Eller's algorithm

    void rotateGrid (
            RotationDirection direction = RotationDirection::Clockwise
        ,   int times = 1
    );

private:

    int m_size;
    std::vector< std::vector< Room > > m_grid;
};

#endif // MAZE_H
