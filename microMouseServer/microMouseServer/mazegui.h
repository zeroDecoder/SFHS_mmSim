#ifndef MAZEGUI_H
#define MAZEGUI_H
#include "mazeBase.h"
#include <QLineF>
#include <QPen>
#include <QGraphicsScene>
#include <QGraphicsItemGroup>
#include <QGraphicsItem>
#include <QPainter>
#include <QPoint>
#include <QStyleOptionGraphicsItem>

class mazeGui : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit mazeGui(QObject *parent = 0);
    ~mazeGui();
    virtual void mousePressEvent(QGraphicsSceneMouseEvent * mouseEvent);
    QPen wallPen();

    QGraphicsItemGroup *mazeWalls;

    void drawMaze(baseMapNode data[][MAZE_HEIGHT]);
    void drawMouse(QPoint cell, mDirection direction);
    void drawGuideLines();

    int mouseX();
    int mouseY();
    void setMouseX();
    void setMouseY();
    mDirection mouseDir();

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
    QGraphicsItemGroup *_bgGrid;
    QGraphicsEllipseItem *_mouse;
    QPen *_wallPen;
    QPen *_guidePen;
    QPen *_mousePen;
    QBrush *_mouseBrush;
    QPoint _mousePos;
    mDirection _mouseDir;
};

#endif // MAZEGUI_H
