#include "gui/GameWidget.h"

#include "maze/IMazeFactory.h"
#include "storage/MazeStorage.h"
#include "pathfinding/PathFinder.h"
#include "common/Constants.h"
#include "common/Timer.h"

#include <QApplication>
#include <QBrush>
#include <QFileDialog>
#include <QGraphicsRectItem>
#include <QGuiApplication>
#include <QMessageBox>
#include <QLabel>
#include <QPen>
#include <QScreen>
#include <QVBoxLayout>

#include <cstdlib>
#include <ctime>
#include <iostream>
#include <thread>

/*----------------------------------------------------------------------------*/

namespace gui {

/*----------------------------------------------------------------------------*/

GameWidget::GameWidget ( int _mazeSize, QWidget * parent )
    :   QWidget( parent )
    ,   m_pMaze( nullptr )
    ,   m_elapsedTime( 0 )
    ,   m_pKruskalMaze( nullptr )
    ,   m_pEllerMaze( nullptr )
    ,   m_pPlayerItem( nullptr )
    ,   m_pDestinationItem( nullptr )
{
    auto mazeFactory = maze::IMazeFactory::createFactory();
    m_pKruskalMaze = mazeFactory->createMaze( maze::MazeAlgo::Kruskal, _mazeSize );
    m_pEllerMaze = mazeFactory->createMaze( maze::MazeAlgo::Eller, _mazeSize );

    m_pMaze = m_pKruskalMaze.get();

    m_pGenerateKruskalButton = new QPushButton( "Generate (Kruskal)", this );
    m_pGenerateEllerButton = new QPushButton( "Generate (Eller)", this );
    m_pFindPathBfsButton = new QPushButton( "Find Path (BFS)", this );
    m_pFindPathDfsButton = new QPushButton( "Find Path (DFS)", this );
    m_pSaveButton = new QPushButton( "Save", this );
    m_pLoadButton = new QPushButton( "Load", this );
    m_pReshuffleButton = new QPushButton( "Reshuffle", this );

    connect(
            m_pGenerateKruskalButton, &QPushButton::clicked,
            this, &GameWidget::handleGenerateKruskalButton
    );
    connect(
            m_pGenerateEllerButton, &QPushButton::clicked,
            this, &GameWidget::handleGenerateEllerButton
    );
    connect(
            m_pFindPathBfsButton, &QPushButton::clicked,
            this, &GameWidget::handleFindPathBfsButton
    );
    connect(
            m_pFindPathDfsButton, &QPushButton::clicked,
            this, &GameWidget::handleFindPathDfsButton
    );
    connect(
            m_pSaveButton, &QPushButton::clicked,
            this, &GameWidget::handleSaveButton
    );
    connect(
            m_pLoadButton, &QPushButton::clicked,
            this, &GameWidget::handleLoadButton
    );
    connect(
            m_pReshuffleButton, &QPushButton::clicked,
            this, &GameWidget::handleReshuffleButton
    );

    m_pMainLayout = std::make_unique< QVBoxLayout >(this);

    QHBoxLayout * pButtonLayout = new QHBoxLayout();
    pButtonLayout->addWidget( m_pGenerateKruskalButton );
    pButtonLayout->addWidget( m_pGenerateEllerButton );
    pButtonLayout->addWidget( m_pFindPathBfsButton );
    pButtonLayout->addWidget( m_pFindPathDfsButton );
    pButtonLayout->addWidget( m_pSaveButton );
    pButtonLayout->addWidget( m_pLoadButton );
    pButtonLayout->addWidget( m_pReshuffleButton );
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
    QScreen * screen = QGuiApplication::primaryScreen();
    QRect screenGeometry = screen->availableGeometry();

    int screenWidth = screenGeometry.width();
    int screenHeight = screenGeometry.height();

    int dynamicRenderSize = std::min(
        { MAX_RENDER_SIZE, screenWidth - 50, screenHeight - 100 }
    );

    m_cellSize
        = dynamicRenderSize / std::max( m_pMaze->getSize(), MAX_CELLS_FOR_SCALING )
    ;

    // Calculate actual maze dimensions
    int mazeWidth = m_cellSize * m_pMaze->getSize();
    int mazeHeight = m_cellSize * m_pMaze->getSize();

    // Ensure the maze dimensions do not exceed the adjusted render size
    int adjustedMazeWidth = std::min( mazeWidth, dynamicRenderSize );
    int adjustedMazeHeight = std::min( mazeHeight, dynamicRenderSize );

    // Create the scene with padding
    m_pScene = std::make_unique<QGraphicsScene>( this );
    m_pScene->setSceneRect(
        -SCENE_PADDING, -SCENE_PADDING,
        adjustedMazeWidth + SCENE_PADDING * 2,
        adjustedMazeHeight + SCENE_PADDING * 2
    );

    // Create the view
    m_pView = std::make_unique< QGraphicsView >( m_pScene.get(), this );
    m_pView->setFixedSize(
        adjustedMazeWidth + SCENE_PADDING * 2 + BORDER_WIDTH,
        adjustedMazeHeight + SCENE_PADDING * 2 + BORDER_WIDTH
    );
    m_pView->setRenderHint( QPainter::Antialiasing );
    m_pView->setParent(this);

    // Draw the outer border
    QPen borderPen( Qt::black );
    borderPen.setWidth( BORDER_WIDTH );
    m_pScene->addRect( 0, 0, adjustedMazeWidth, adjustedMazeHeight, borderPen );
}

/*----------------------------------------------------------------------------*/

void GameWidget::drawMaze ()
{
    QPen wallPen( Qt::black );
    wallPen.setWidth( BORDER_WIDTH );

    for ( int y = 0; y < m_pMaze->getSize(); ++y )
    {
        for ( int x = 0; x < m_pMaze->getSize(); ++x )
        {
            auto const & room = m_pMaze->getRoom( x, y );

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
    player.y = rand() % m_pMaze->getSize();
    m_pPlayerItem = m_pScene->addEllipse(
            player.x * m_cellSize + m_cellSize / CELL_MARGIN_FACTOR
        ,   player.y * m_cellSize + m_cellSize / CELL_MARGIN_FACTOR
        ,   m_cellSize / 2
        ,   m_cellSize / 2
        ,   QPen( Qt::red )
        ,   QBrush( Qt::red )
    );

    // Place the destination randomly on the right side
    int destY = rand() % m_pMaze->getSize();
    m_pDestinationItem = m_pScene->addRect(
            ( m_pMaze->getSize() - 1 ) * m_cellSize
        ,   destY * m_cellSize
        ,   m_cellSize
        ,   m_cellSize
        ,   QPen( Qt::green )
        ,   QBrush( Qt::green )
    );
}

/*----------------------------------------------------------------------------*/

void GameWidget::keyPressEvent ( QKeyEvent * _event )
{
    if ( _event->key() == Qt::Key_B )
    {
        handleFindPathBfsButton();
        return;
    }

    if ( _event->key() == Qt::Key_D )
    {
        handleFindPathDfsButton();
        return;
    }

    if ( _event->key() == Qt::Key_K )
    {
        handleGenerateKruskalButton();
        return;
    }
    else if ( _event->key() == Qt::Key_E )
    {
        handleGenerateEllerButton();
        return;
    }

    int newX = player.x;
    int newY = player.y;

    // Determine new position based on key press
    auto const & room = m_pMaze->getRoom( player.x, player.y );

    if (    _event->key() == Qt::Key_Up && !room.walls[ TOP_WALL ] )
    {
        newY--;
    }
    else if (   _event->key() == Qt::Key_Down && !room.walls[ BOTTOM_WALL ] )
    {
        newY++;
    }
    else if (   _event->key() == Qt::Key_Left && !room.walls[ LEFT_WALL ] )
    {
        newX--;
    }
    else if (   _event->key() == Qt::Key_Right && !room.walls[ RIGHT_WALL ] )
    {
        newX++;
    }

    // Update player position
    if (    newX >= 0 && newX < m_pMaze->getSize()
        &&  newY >= 0 && newY < m_pMaze->getSize()
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
                ).arg( m_elapsedTime )
        );
        QApplication::quit();
    }
}

/*----------------------------------------------------------------------------*/

void GameWidget::handleGenerateKruskalButton ()
{
    m_pMaze = m_pKruskalMaze.get();
    generateNewMaze();
}
/*----------------------------------------------------------------------------*/

void GameWidget::handleGenerateEllerButton ()
{
    m_pMaze = m_pEllerMaze.get();
    generateNewMaze();
}

/*----------------------------------------------------------------------------*/

void GameWidget::handleFindPathBfsButton ()
{
    visualizePath( true );
}

/*----------------------------------------------------------------------------*/

void GameWidget::handleFindPathDfsButton ()
{
    visualizePath( false );
}

/*----------------------------------------------------------------------------*/

void GameWidget::handleSaveButton ()
{
    QString filename = QFileDialog::getSaveFileName(
        this, "Save Maze", "", "Maze Files (*.maze)"
    );
    if ( !filename.isEmpty() )
    {
        std::thread saveThread( [ this, filename ] ()
        {
            try
            {
                storage::MazeStorage::save( *m_pMaze, filename.toStdString() );

            }
            catch ( std::exception const& _e )
            {
                QMessageBox::critical( this, "Error", _e.what() );
            }
        } );
        saveThread.detach();
    }
}

/*----------------------------------------------------------------------------*/

void GameWidget::handleLoadButton ()
{
    QString filename = QFileDialog::getOpenFileName(
        this, "Load Maze", "", "Maze Files (*.maze)"
    );
    if ( !filename.isEmpty() )
    {
        try
        {
            storage::MazeStorage::load( *m_pMaze, filename.toStdString() );

            resetAll();

            drawMaze();

            placePlayerAndDestination();
        }
        catch ( std::exception const& _e )
        {
            QMessageBox::critical( this, "Error", _e.what() );
        }
    }
}

/*----------------------------------------------------------------------------*/

void GameWidget::handleReshuffleButton ()
{
    m_solutionPath.clear();
    resetPath();

    clearPlayerAndDestination();
    placePlayerAndDestination();
}

/*----------------------------------------------------------------------------*/

void GameWidget::updateTimer ()
{
    m_elapsedTime++;
    setWindowTitle(
        QString( "Maze Game - Time: %1 seconds" ).arg( m_elapsedTime )
    );
}

/*----------------------------------------------------------------------------*/

void GameWidget::generateNewMaze ()
{
    {
        m_pMaze->reset();
        Timer< std::milli > timer( m_pMaze->algoName() );
        m_pMaze->generateMaze();
    }

    resetAll();

    // Reset player and destination positions
    placePlayerAndDestination();

    // Redraw the maze
    drawMaze();

    // Redraw the outer border
    QPen borderPen(Qt::black);
    borderPen.setWidth(2);
    int mazeWidth = m_cellSize * m_pMaze->getSize();
    int mazeHeight = m_cellSize * m_pMaze->getSize();
    m_pScene->addRect(0, 0, mazeWidth, mazeHeight, borderPen);
}
/*----------------------------------------------------------------------------*/

void GameWidget::visualizePath ( bool _useBfs )
{
    if ( m_isPathVisualized )
    {
        resetPath();
        return;
    }

    // If not visualized, compute or reuse path
    if ( m_solutionPath.empty() )
    {
        pathfinding::PathFinder pathFinder( *m_pMaze );
        Point start{ player.x, player.y };
        Point goal{
                m_pMaze->getSize() - 1
            ,   static_cast< int >(
                    m_pDestinationItem->rect().y() / m_cellSize
                )
        };

        try
        {
            std::string timerTitle = "Path finding - ";
            timerTitle += ( _useBfs ) ? "BFS" : "DFS";
            Timer< std::milli > timer( timerTitle );
            m_solutionPath = pathFinder.solve( start, goal, _useBfs );
        }
        catch (const std::exception &e)
        {
            QMessageBox::warning( this, "Pathfinder Error", e.what() );
            return;
        }
    }

    // Visualize the path
    QPen pathPen( Qt::blue );
    pathPen.setWidth( PATH_WIDTH );

    for (size_t i = 1; i < m_solutionPath.size(); ++i)
    {
        Point from = m_solutionPath[i - 1];
        Point to = m_solutionPath[i];

        QGraphicsLineItem * line = m_pScene->addLine(
            from.x * m_cellSize + m_cellSize / 2,
            from.y * m_cellSize + m_cellSize / 2,
            to.x * m_cellSize + m_cellSize / 2,
            to.y * m_cellSize + m_cellSize / 2,
            pathPen
        );

        m_pathLines.push_back( line );
    }

    m_isPathVisualized = true; // Path is now visualized
}

/*----------------------------------------------------------------------------*/

void GameWidget::resetAll ()
{
    resetPath();
    clearPlayerAndDestination();
    m_solutionPath.clear();
    m_pScene->clear();

    resetTimer();
}

/*----------------------------------------------------------------------------*/

void GameWidget::resetPath ()
{
    if ( !m_isPathVisualized )
    {
        return;
    }

    // Remove path visualization
    for ( auto * pLine : m_pathLines )
    {
        m_pScene->removeItem( pLine );
        delete pLine;
    }

    m_pathLines.clear();

    m_isPathVisualized = false;
}

/*----------------------------------------------------------------------------*/

void GameWidget::resetTimer ()
{
    m_elapsedTime = -1;
}

/*----------------------------------------------------------------------------*/

void GameWidget::clearPlayerAndDestination ()
{
    if ( m_pPlayerItem )
    {
        m_pScene->removeItem( m_pPlayerItem );
        delete m_pPlayerItem;
        m_pPlayerItem = nullptr;
    }

    if ( m_pDestinationItem )
    {
        m_pScene->removeItem( m_pDestinationItem );
        delete m_pDestinationItem;
        m_pDestinationItem = nullptr;
    }
}

/*----------------------------------------------------------------------------*/

} // namespace gui

/*----------------------------------------------------------------------------*/
