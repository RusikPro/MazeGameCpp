#ifndef KRUSKALMAZE_H
#define KRUSKALMAZE_H

/*----------------------------------------------------------------------------*/

#include "BaseMaze.h"
#include "common/Point.h"

#include <random>
#include <algorithm>
#include <unordered_map>

/*----------------------------------------------------------------------------*/

namespace maze {

/*----------------------------------------------------------------------------*/

class KruskalMaze final
    :   public BaseMaze
{
public:

    explicit KruskalMaze ( int _size );

    void generateMaze () override;

    std::string algoName () const override { return "Kruskal"; }

private:

    struct Edge
    {
        Point cell1;
        Point cell2;
    };

    class UnionFind
    {
    public:
        UnionFind ( int _n );
        int find ( int _x );
        void unite ( int _x, int _y );
    private:
        std::vector< int > parent, rank;
    };

    void generateEdges ();

    void removeWall ( Edge const & _edge );
    int cellToId ( int x, int y ) const;

    std::vector< Edge > m_edges;
};

/*----------------------------------------------------------------------------*/

} // namespace maze

/*----------------------------------------------------------------------------*/

#endif // KRUSKALMAZE_H
