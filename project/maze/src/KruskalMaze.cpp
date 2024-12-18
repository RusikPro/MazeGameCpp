#include "KruskalMaze.h"

#include "common/Constants.h"

namespace maze {

/*----------------------------------------------------------------------------*/

KruskalMaze::KruskalMaze ( int _size )
    :   BaseMaze( _size )
{
    generateEdges();
}

/*----------------------------------------------------------------------------*/

void KruskalMaze::generateEdges ()
{
    // Generate all possible edges
    m_edges.clear();
    for ( int y = 0; y < m_size; ++y )
    {
        for ( int x = 0; x < m_size; ++x )
        {
            if ( x < m_size - 1 ) // Right edge
                m_edges.push_back( { { x, y }, { x + 1, y } } );
            if ( y < m_size - 1 ) // Bottom edge
                m_edges.push_back( { { x, y }, { x, y + 1 } } );
        }
    }
}

/*----------------------------------------------------------------------------*/

void KruskalMaze::generateMaze()
{
    reset();
    generateEdges();

    // Step 1: Shuffle edges
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle( m_edges.begin(), m_edges.end(), g );

    // Step 2: Initialize Union-Find
    UnionFind uf( m_size * m_size );

    // Step 3: Process edges
    for ( auto const & edge : m_edges )
    {
        int id1 = cellToId( edge.cell1.first, edge.cell1.second );
        int id2 = cellToId( edge.cell2.first, edge.cell2.second );

        if ( uf.find( id1 ) != uf.find( id2 ) )
        {
            uf.unite( id1, id2 );
            removeWall( edge );
        }
    }
}

/*----------------------------------------------------------------------------*/

int KruskalMaze::cellToId ( int _x, int _y ) const
{
    return _y * m_size + _x;
}

/*----------------------------------------------------------------------------*/

void KruskalMaze::removeWall ( Edge const & _edge )
{
    int dx = _edge.cell2.first - _edge.cell1.first;
    int dy = _edge.cell2.second - _edge.cell1.second;

    if ( dx == 1 ) // Right wall
    {
        m_grid[ _edge.cell1.second ][ _edge.cell1.first ].walls[ RIGHT_WALL ] = false;
        m_grid[ _edge.cell2.second ][ _edge.cell2.first ].walls[ LEFT_WALL ] = false;
    }
    else if ( dy == 1 ) // Bottom wall
    {
        m_grid[ _edge.cell1.second ][ _edge.cell1.first ].walls[ BOTTOM_WALL ] = false;
        m_grid[ _edge.cell2.second ][ _edge.cell2.first ].walls[ TOP_WALL ] = false;
    }
}

/*----------------------------------------------------------------------------*/

KruskalMaze::UnionFind::UnionFind ( int _n )
    :   parent( _n )
    ,   rank( _n, 0 )
{
    for ( int i = 0; i < _n; ++i )
        parent[ i ] = i;
}

/*----------------------------------------------------------------------------*/

int KruskalMaze::UnionFind::find ( int _x )
{
    if ( parent[ _x ] != _x )
        parent[ _x ] = find( parent[ _x ] );
    return parent[ _x ];
}

/*----------------------------------------------------------------------------*/

void KruskalMaze::UnionFind::unite ( int _x, int _y )
{
    int rootX = find( _x );
    int rootY = find( _y );

    if ( rootX != rootY )
    {
        if (rank[ rootX ] < rank[ rootY ] )
            parent[ rootX ] = rootY;
        else if (rank[ rootX ] > rank[ rootY ] )
            parent[ rootY ] = rootX;
        else
        {
            parent[ rootY ] = rootX;
            rank[ rootX ]++;
        }
    }
}

/*----------------------------------------------------------------------------*/

} // namespace maze

/*----------------------------------------------------------------------------*/
