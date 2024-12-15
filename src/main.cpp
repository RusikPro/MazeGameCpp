#include "GameWidget.h"
#include <QApplication>
#include <iostream>
#include <cstdlib>

int main ( int argc, char *argv[] )
{
    QApplication app( argc, argv );

    int mazeSize = 30;

    if ( argc > 1 )
    {
        try
        {
            mazeSize = std::stoi( argv[ 1 ] );
            if ( mazeSize <= 0 )
            {
                throw std::invalid_argument( "Maze size must be a positive integer." );
            }
        }
        catch ( const std::exception &e )
        {
            std::cerr << "Error: " << e.what() << std::endl;
            return EXIT_FAILURE;
        }
    }

    try
    {
        GameWidget widget( mazeSize );
        widget.show();
        return app.exec();
    }
    catch ( const std::exception &e )
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    catch ( ... )
    {
        std::cerr << "An unknown error occurred." << std::endl;
        return EXIT_FAILURE;
    }
}
