#ifndef ELLERMAZE_H
#define ELLERMAZE_H

/*----------------------------------------------------------------------------*/

#include "BaseMaze.h"

/*----------------------------------------------------------------------------*/

namespace maze {

/*----------------------------------------------------------------------------*/

enum class RotationDirection
{
    Clockwise,
    Counterclockwise
};

/*----------------------------------------------------------------------------*/

class EllerMaze final
    :   public BaseMaze
{
public:

    EllerMaze ( int _size );

    void generateMaze () override;

    std::string algoName () const override { return "Eller"; }

private:

    using RowSets = std::vector< int >;

    void rotateGrid (
            RotationDirection direction = RotationDirection::Clockwise
        ,   int times = 1
    );

    void initializeRowSets ( RowSets & _rowSets, int & _nextSetId ) const;
    void mergeHorizontalWalls ( int _row, RowSets & _rowSets );
    void createVerticalConnections ( int _row, RowSets & _rowSets );
    void prepareTheNextRow ( int _row, RowSets & _rowSets, int & _nextSetId );
    void handleTheLastRow ( RowSets & _rowSets );

private:

    bool m_leftToRight;
};

/*----------------------------------------------------------------------------*/

} // namespace maze

/*----------------------------------------------------------------------------*/

#endif // ELLERMAZE_H
