#include "mazeConst.h"
#include "mazegui.h"
#include<QGraphicsSceneMoveEvent>
#include <QPen>

mazeGui::mazeGui(QObject *parent) :
    QGraphicsScene(parent)
{
    this->_wallPen = new QPen(QColor(0xFF,0xFF,0xFF,0xFF));
    this->_guidePen = new QPen(QColor(0xFF,0xFF,0xFF,0x20));
    this->_wallPen->setWidth(WALL_THICKNESS_PX);
    this->_guidePen->setWidth(WALL_THICKNESS_PX);

}
void mazeGui::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    qDebug("pos %f, %f",mouseEvent->scenePos().x(), mouseEvent->scenePos().y());
    //check if mouse event is a left click inside of the maze
    if(mouseEvent->button() == Qt::LeftButton &&
            mouseEvent->scenePos().x() > 0 &&
            mouseEvent->scenePos().y() > 0 &&
            mouseEvent->scenePos().x() < MAZE_WIDTH_PX &&
            mouseEvent->scenePos().y() < MAZE_HEIGHT_PX)
    {

        int xmod = (int)mouseEvent->scenePos().x() % PX_PER_UNIT;
        int ymod = (int)mouseEvent->scenePos().y() % PX_PER_UNIT;
        int xpos = (int)mouseEvent->scenePos().x() / PX_PER_UNIT;
        int ypos = (int)mouseEvent->scenePos().y() / PX_PER_UNIT;

        if((xmod < PX_PER_UNIT/3  || xmod > 2*PX_PER_UNIT/3 )&& ymod > PX_PER_UNIT/3  && ymod < 2*PX_PER_UNIT/3)
        {
            if(xmod >= PX_PER_UNIT /2)
            {
                emit passRightWall(QPoint(xpos, ypos));
            }
            else
            {
                emit passLeftWall(QPoint(xpos, ypos));
            }
        }
        else if((ymod < PX_PER_UNIT/3  || ymod > 2*PX_PER_UNIT/3 )&& xmod > PX_PER_UNIT/3  && xmod < 2*PX_PER_UNIT/3)
        {
            if(ymod >= PX_PER_UNIT/2 || ymod == 0)
            {
                emit passTopWall(QPoint(xpos, ypos));
            }
            else
            {
                emit passBottomWall(QPoint(xpos, ypos));
            }
        }
    }
    else if(mouseEvent->button() == Qt::RightButton &&
            mouseEvent->scenePos().x() > 0 &&
            mouseEvent->scenePos().y() > 0 &&
            mouseEvent->scenePos().x() <= MAZE_WIDTH_PX &&
            mouseEvent->scenePos().y() <= MAZE_HEIGHT_PX)
    {
        //remove wall
    }
}

QPen mazeGui::wallPen()
{
    return *_wallPen;
}
