#ifndef __GUI_INCLUDE__GAMEWIDGET_H__
#define __GUI_INCLUDE__GAMEWIDGET_H__

/*----------------------------------------------------------------------------*/

#include "maze/IMaze.h"
#include "common/Point.h"

#include <QGraphicsEllipseItem>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QKeyEvent>
#include <QPushButton>
#include <QTimer>
#include <QVBoxLayout>
#include <QWidget>

#include <memory>

/*----------------------------------------------------------------------------*/

namespace gui {

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

    void generateNewMaze ();

    void visualizePath ( bool _useBfs );

    void resetAll ();
    void resetPath ();
    void resetTimer ();
    void clearPlayerAndDestination ();

private slots:

    void handleGenerateKruskalButton ();
    void handleGenerateEllerButton ();
    void handleFindPathBfsButton ();
    void handleFindPathDfsButton ();
    void handleSaveButton ();
    void handleLoadButton ();
    void handleReshuffleButton ();
    void updateTimer ();

private:

    std::unique_ptr< QGraphicsScene > m_pScene;
    std::unique_ptr< QGraphicsView > m_pView;

    QGraphicsEllipseItem * m_pPlayerItem;
    QGraphicsRectItem * m_pDestinationItem;

    maze::IMaze * m_pMaze; // Current maze
    std::unique_ptr< maze::IMaze > m_pKruskalMaze;
    std::unique_ptr< maze::IMaze > m_pEllerMaze;


    Player player;

    std::unique_ptr< QTimer > m_pTimer;
    int m_elapsedTime;
    int m_cellSize;

    // Visualization mmembers
    std::vector< Point > m_solutionPath;
    std::vector< QGraphicsLineItem * > m_pathLines;
    bool m_isPathVisualized = false;

    // UI Elements for Buttons
    std::unique_ptr< QPushButton > m_pGenerateKruskalButton;
    std::unique_ptr< QPushButton > m_pGenerateEllerButton;
    std::unique_ptr< QPushButton > m_pFindPathBfsButton;
    std::unique_ptr< QPushButton > m_pFindPathDfsButton;
    std::unique_ptr< QPushButton > m_pSaveButton;
    std::unique_ptr< QPushButton > m_pLoadButton;
    std::unique_ptr<QPushButton> m_pReshuffleButton;
    std::unique_ptr< QVBoxLayout > m_pMainLayout;
};

/*----------------------------------------------------------------------------*/

} // namespace gui

/*----------------------------------------------------------------------------*/

#endif // __GUI_INCLUDE__GAMEWIDGET_H__
