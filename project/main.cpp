#include "gui/GameWidget.h"
#include "common/Constants.h"

#include <QApplication>
#include <iostream>
#include <cstdlib>

/*----------------------------------------------------------------------------*/

int main ( int argc, char *argv[] )
{
    QApplication app( argc, argv );

    int mazeSize = DEFAULT_MAZE_SIZE;

    if ( argc > 1 )
    {
        try
        {
            mazeSize = std::stoi( argv[ 1 ] );
            if ( mazeSize <= 0 )
            {
                throw std::invalid_argument(
                    "Maze size must be a positive integer."
                );
            }
        }
        catch ( std::exception const & _e )
        {
            std::cerr << "Error: " << _e.what() << std::endl;
            return EXIT_FAILURE;
        }
    }

    try
    {
        gui::GameWidget widget( mazeSize );
        widget.show();
        return app.exec();
    }
    catch ( std::exception const & _e )
    {
        std::cerr << "Error: " << _e.what() << std::endl;
        return EXIT_FAILURE;
    }
    catch ( ... )
    {
        std::cerr << "An unknown error occurred." << std::endl;
        return EXIT_FAILURE;
    }
}

/*----------------------------------------------------------------------------*/
