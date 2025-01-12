#ifndef MAZESTORAGE_H
#define MAZESTORAGE_H

/*----------------------------------------------------------------------------*/

#include "maze/IMaze.h"
#include <string>

/*----------------------------------------------------------------------------*/

namespace storage {

/*----------------------------------------------------------------------------*/

class MazeStorage
{
public:
    static void
    save ( maze::IMaze const & _maze, std::string const & _filename );

    static void
    load ( maze::IMaze & _maze, std::string const & _filename );
};

/*----------------------------------------------------------------------------*/

} // namespace storage

/*----------------------------------------------------------------------------*/

#endif // MAZESTORAGE_H
