#ifndef POINT_H
#define POINT_H

/*----------------------------------------------------------------------------*/

#include <functional>

/*----------------------------------------------------------------------------*/

struct Point
{
    int x;
    int y;

    // Equality operator
    bool operator== ( Point const & _other ) const
    {
        return x == _other.x && y == _other.y;
    }
};

/*----------------------------------------------------------------------------*/

namespace std
{
    template <>
    struct hash< Point >
    {
        size_t operator() ( Point const & _point ) const
        {
            return hash< int >()( _point.x ) ^ ( hash<int>()( _point.y ) << 1 );
        }
    };
}

/*----------------------------------------------------------------------------*/

#endif // POINT_H
