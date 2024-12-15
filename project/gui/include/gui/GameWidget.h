#ifndef GAMEWIDGET_H
#define GAMEWIDGET_H

#include "maze/Maze.h"

#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsEllipseItem>
#include <QKeyEvent>
#include <QTimer>

#include <memory>

/*----------------------------------------------------------------------------*/

class GameWidget
    :   public QWidget
{
    Q_OBJECT

public:

    explicit GameWidget ( int _mazeSize, QWidget * _pParent = nullptr );

    ~GameWidget ();

protected:

    void keyPressEvent ( QKeyEvent * _event ) override;

private:

    struct Player
    {
        int x, y;
    };

    void setupScene ();
    void drawMaze ();
    void placePlayerAndDestination ();
    void checkVictory ();

    std::unique_ptr< QGraphicsScene > m_pScene;
    std::unique_ptr< QGraphicsView > m_pView;

    QGraphicsEllipseItem * m_pPlayerItem;
    QGraphicsRectItem * m_pDestinationItem;

    Maze maze;
    Player player;

    std::unique_ptr< QTimer > m_pTimer;
    int elapsedTime;
    int m_cellSize;

private slots:
    void updateTimer();
};

/*----------------------------------------------------------------------------*/

#endif // GAMEWIDGET_H
