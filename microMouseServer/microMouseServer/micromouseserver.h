#ifndef MICROMOUSESERVER_H
#define MICROMOUSESERVER_H
#include "mazeConst.h"
#include "mazeBase.h"
#include "mazegui.h"
#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsItem>
#include <QEvent>
#include <QGraphicsSceneMouseEvent>
#include <QLineF>

namespace Ui {
class microMouseServer;
}

class microMouseServer : public QMainWindow
{
    Q_OBJECT

public:
    explicit microMouseServer(QWidget *parent = 0);
    ~microMouseServer();

private slots:
    void on_tabWidget_tabBarClicked(int index);
    void loadMaze();
    void saveMaze();
    void addWall(QLineF *wall);

private:
    Ui::microMouseServer *ui;

    mazeGui *map;

    std::vector<QGraphicsLineItem*> backgroundGrid;
    QGraphicsItemGroup *bgGrid;
    QGraphicsItemGroup *mazeWalls;
    struct baseMapNode mazeData[MAZE_WIDTH][MAZE_HEIGHT];

    void linkMenu();
    void drawMaze();
    void drawGuideLines();
    void removeGuideLines();
};

#endif // MICROMOUSESERVER_H
