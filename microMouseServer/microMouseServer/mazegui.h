#ifndef MAZEGUI_H
#define MAZEGUI_H
#include "mazeBase.h"
#include <QLineF>
#include <QPen>
#include <QGraphicsScene>
#include <QGraphicsItemGroup>

class mazeGui : public QGraphicsScene
{
    Q_OBJECT
public:
    QPen wallPen();
    explicit mazeGui(QObject *parent = 0);
    virtual void mousePressEvent(QGraphicsSceneMouseEvent * mouseEvent);
    QGraphicsItemGroup *_bgGrid;
    QGraphicsItemGroup *mazeWalls;
    void drawMaze(baseMapNode data[][MAZE_HEIGHT]);
    void drawGuideLines();

signals:
    void passRightWall(QPoint cell);
    void passLeftWall(QPoint cell);
    void passTopWall(QPoint cell);
    void passBottomWall(QPoint cell);
    void takeRightWall(QPoint cell);
    void takeLeftWall(QPoint cell);
    void takeTopWall(QPoint cell);
    void takeBottomWall(QPoint cell);


public slots:

private:
    QPen *_wallPen;
    QPen *_guidePen;
};

#endif // MAZEGUI_H
