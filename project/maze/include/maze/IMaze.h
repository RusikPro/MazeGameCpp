#ifndef IMAZE_H
#define IMAZE_H

#include <string>
#include <vector>

namespace maze {

/*----------------------------------------------------------------------------*/

struct Room
{
    bool walls[ 4 ] = { true, true, true, true }; // {top, right, bottom, left}
};

/*----------------------------------------------------------------------------*/

using Rooms = std::vector< Room >;
using Grid = std::vector< Rooms >;

/*----------------------------------------------------------------------------*/

class IMaze
{
public:
    virtual ~IMaze () = default;

    virtual Grid const & getGrid () const = 0;
    virtual Grid & takeGrid () = 0;

    virtual int getSize () const = 0;
    virtual const Room& getRoom ( int x, int y ) const = 0;

    virtual void reset () = 0;

    virtual void generateMaze () = 0;

    virtual std::string algoName () const = 0;
};

/*----------------------------------------------------------------------------*/

} // namespace maze

#endif // IMAZE_H
