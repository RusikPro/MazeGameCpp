#include "gui/GameWidget.h"
#include "pathfinding/PathFinder.h"
#include "common/Constants.h"
#include "common/Timer.h"

#include <QGraphicsRectItem>
#include <QPen>
#include <QBrush>
#include <QLabel>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QApplication>
#include <cstdlib>
#include <ctime>

#include <iostream>

/*----------------------------------------------------------------------------*/

GameWidget::GameWidget ( int _mazeSize, QWidget * parent )
    :   QWidget( parent )
    ,   maze( _mazeSize )
    ,   elapsedTime( 0 )
{
    m_pGenerateButton = std::make_unique< QPushButton >( "Generate", this );
    m_pFindPathButton = std::make_unique< QPushButton >( "Find Path", this );

    connect(
            m_pGenerateButton.get()
        ,   &QPushButton::clicked
        ,   this
        ,   &GameWidget::handleGenerateButton
    );
    connect(
            m_pFindPathButton.get()
        ,   &QPushButton::clicked
        ,   this
        ,   &GameWidget::handleFindPathButton
    );

    m_pMainLayout = std::make_unique< QVBoxLayout >(this);

    QHBoxLayout * pButtonLayout = new QHBoxLayout();
    pButtonLayout->addWidget( m_pGenerateButton.get() );
    pButtonLayout->addWidget( m_pFindPathButton.get() );
    pButtonLayout->addStretch();

    m_pMainLayout->addLayout( pButtonLayout );
    setupScene();
    m_pMainLayout->addWidget( m_pView.get() );

    setLayout( m_pMainLayout.get() );

    generateNewMaze();

    // Enable keyboard focus
    setFocusPolicy( Qt::StrongFocus );
    setFocus();

    // Timer setup
    m_pTimer = std::make_unique< QTimer >( this );
    connect( m_pTimer.get(), &QTimer::timeout, this, &GameWidget::updateTimer );
    m_pTimer->start( TIMER_INTERVAL_MS ); // Update every second
}

/*----------------------------------------------------------------------------*/

GameWidget::~GameWidget ()
{
}

/*----------------------------------------------------------------------------*/

void GameWidget::setupScene ()
{
    // Calculate the cell size dynamically based on maze size
    m_cellSize = MAX_RENDER_SIZE / std::max( maze.getSize(), MAX_CELLS_FOR_SCALING );

    // Calculate the actual maze width and height
    int const mazeWidth = m_cellSize * maze.getSize();
    int const mazeHeight = m_cellSize * maze.getSize();

    // Create the scene with padding
    m_pScene = std::make_unique< QGraphicsScene >( this );
    m_pScene->setSceneRect(
        -SCENE_PADDING, -SCENE_PADDING, mazeWidth + SCENE_PADDING * 2, mazeHeight + SCENE_PADDING * 2
    );

    // Create the view
    m_pView = std::make_unique< QGraphicsView >( m_pScene.get(), this );
    m_pView->setFixedSize(
        mazeWidth + SCENE_PADDING * 2 + BORDER_WIDTH, mazeHeight + SCENE_PADDING * 2 + BORDER_WIDTH
    );
    m_pView->setRenderHint( QPainter::Antialiasing );
    m_pView->setParent( this );

    // Draw the outer border
    QPen borderPen( Qt::black );
    borderPen.setWidth( 2 );
    m_pScene->addRect( 0, 0, mazeWidth, mazeHeight, borderPen );
}

/*----------------------------------------------------------------------------*/

void GameWidget::drawMaze ()
{
    QPen wallPen( Qt::black );
    wallPen.setWidth( BORDER_WIDTH );

    for ( int y = 0; y < maze.getSize(); ++y )
    {
        for ( int x = 0; x < maze.getSize(); ++x )
        {
            Room const & room = maze.getRoom( x, y );

            // Top wall
            if ( room.walls[ TOP_WALL ] )
            {
                m_pScene->addLine(
                        x * m_cellSize
                    ,   y * m_cellSize
                    ,   ( x + 1 ) * m_cellSize
                    ,   y * m_cellSize
                    ,   wallPen
                );
            }
            // Left wall
            if ( room.walls[ LEFT_WALL ] )
            {
                m_pScene->addLine(
                        x * m_cellSize
                    ,   y * m_cellSize
                    ,   x * m_cellSize
                    ,   ( y + 1 ) * m_cellSize
                    ,   wallPen
                );
            }
            // Right wall
            if ( room.walls[ RIGHT_WALL ] )
            {
                m_pScene->addLine(
                        ( x + 1 ) * m_cellSize
                    ,   y * m_cellSize
                    ,   ( x + 1 ) * m_cellSize
                    ,   ( y + 1 ) * m_cellSize
                    ,   wallPen
                );
            }
            // Bottom wall
            if ( room.walls[ BOTTOM_WALL ] )
            {
                m_pScene->addLine(
                        x * m_cellSize
                    ,   ( y + 1 ) * m_cellSize
                    ,   ( x + 1 ) * m_cellSize
                    ,   ( y + 1 ) * m_cellSize
                    ,   wallPen
                );
            }
        }
    }
}

/*----------------------------------------------------------------------------*/

void GameWidget::placePlayerAndDestination ()
{
    // Place the player randomly on the left side
    srand( static_cast< unsigned >( time( nullptr ) ) );
    player.x = 0;
    player.y = rand() % maze.getSize();
    m_pPlayerItem = m_pScene->addEllipse(
            player.x * m_cellSize + m_cellSize / CELL_MARGIN_FACTOR
        ,   player.y * m_cellSize + m_cellSize / CELL_MARGIN_FACTOR
        ,   m_cellSize / 2
        ,   m_cellSize / 2
        ,   QPen( Qt::red )
        ,   QBrush( Qt::red )
    );

    // Place the destination randomly on the right side
    int destY = rand() % maze.getSize();
    m_pDestinationItem = m_pScene->addRect(
            ( maze.getSize() - 1 ) * m_cellSize
        ,   destY * m_cellSize
        ,   m_cellSize
        ,   m_cellSize
        ,   QPen( Qt::green )
        ,   QBrush( Qt::green )
    );
}

/*----------------------------------------------------------------------------*/

void GameWidget::keyPressEvent ( QKeyEvent * event )
{
    if ( event->key() == Qt::Key_P )
    {
        visualizePath();
        return;
    }

    if ( event->key() == Qt::Key_G )
    {
        generateNewMaze();
        return;
    }

    int newX = player.x;
    int newY = player.y;

    // Determine new position based on key press
    if (    event->key() == Qt::Key_Up
        &&  !maze.getRoom( player.x, player.y ).walls[ TOP_WALL ]
    )
    {
        newY--;
    }
    else if (   event->key() == Qt::Key_Down
            &&  !maze.getRoom( player.x, player.y ).walls[ BOTTOM_WALL ] )
    {
        newY++;
    }
    else if (   event->key() == Qt::Key_Left
            &&  !maze.getRoom( player.x, player.y ).walls[ LEFT_WALL ] )
    {
        newX--;
    }
    else if (   event->key() == Qt::Key_Right
            &&  !maze.getRoom( player.x, player.y ).walls[ RIGHT_WALL ] )
    {
        newX++;
    }

    // Update player position
    if (    newX >= 0 && newX < maze.getSize()
        &&  newY >= 0 && newY < maze.getSize()
    )
    {
        player.x = newX;
        player.y = newY;

        m_pPlayerItem->setRect(
                player.x * m_cellSize + m_cellSize / 4
            ,   player.y * m_cellSize + m_cellSize / 4
            ,   m_cellSize / 2
            ,   m_cellSize / 2
        );

        checkVictory();
    }
}

/*----------------------------------------------------------------------------*/

void GameWidget::checkVictory ()
{
    if ( m_pPlayerItem->collidesWithItem( m_pDestinationItem ) )
    {
        m_pTimer->stop();

        QMessageBox::information(
                this
            ,   "Victory!"
            ,   QString(
                    "Congratulations! You reached the goal in %1 seconds!"
                ).arg( elapsedTime )
        );
        QApplication::quit();
    }
}

/*----------------------------------------------------------------------------*/

void GameWidget::handleGenerateButton ()
{
    generateNewMaze();
}

/*----------------------------------------------------------------------------*/

void GameWidget::handleFindPathButton ()
{
    visualizePath();
}

/*----------------------------------------------------------------------------*/

void GameWidget::updateTimer ()
{
    elapsedTime++;
    setWindowTitle(
        QString( "Maze Game - Time: %1 seconds" ).arg( elapsedTime )
    );
}

/*----------------------------------------------------------------------------*/

void GameWidget::generateNewMaze ()
{
    {
        maze.reset();
        Timer< std::milli > timer( "maze.generateMaze" );
        maze.generateMaze();
    }

    // Clear the scene
    m_pScene->clear();

    // Clear stored solution path
    m_solutionPath.clear();

    // Properly clean up path lines
    m_pathLines.clear();

    // Reset visualization flag
    m_isPathVisualized = false;

    // Reset player and destination positions
    placePlayerAndDestination();

    // Redraw the maze
    drawMaze();

    // Redraw the outer border
    QPen borderPen(Qt::black);
    borderPen.setWidth(2);
    int mazeWidth = m_cellSize * maze.getSize();
    int mazeHeight = m_cellSize * maze.getSize();
    m_pScene->addRect(0, 0, mazeWidth, mazeHeight, borderPen);
}
/*----------------------------------------------------------------------------*/

void GameWidget::visualizePath ()
{
    if ( m_isPathVisualized )
    {
        // Remove path visualization
        for (auto *line : m_pathLines)
        {
            m_pScene->removeItem(line);
            delete line; // Clean up memory
        }
        m_pathLines.clear();
        m_isPathVisualized = false;
        return;
    }

    // If not visualized, compute or reuse path
    if (m_solutionPath.empty())
    {
        // Create PathFinder instance
        PathFinder pathFinder( maze );
        Point start{player.x, player.y};
        Point goal{maze.getSize() - 1, static_cast<int>(m_pDestinationItem->rect().y() / m_cellSize)};

        try
        {
            Timer< std::milli > timer( "pathFinder.solve" );
            m_solutionPath = pathFinder.solve( start, goal, true ); // BFS by default
        }
        catch (const std::exception &e)
        {
            QMessageBox::warning(this, "Pathfinder Error", e.what());
            return;
        }
    }

    // Visualize the path
    QPen pathPen(Qt::blue);
    pathPen.setWidth(3);

    for (size_t i = 1; i < m_solutionPath.size(); ++i)
    {
        Point from = m_solutionPath[i - 1];
        Point to = m_solutionPath[i];

        QGraphicsLineItem *line = m_pScene->addLine(
            from.x * m_cellSize + m_cellSize / 2,
            from.y * m_cellSize + m_cellSize / 2,
            to.x * m_cellSize + m_cellSize / 2,
            to.y * m_cellSize + m_cellSize / 2,
            pathPen
        );

        m_pathLines.push_back(line);
    }

    m_isPathVisualized = true; // Path is now visualized
}



/*----------------------------------------------------------------------------*/
