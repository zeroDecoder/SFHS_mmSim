#include "mazeConst.h"
#include "mazegui.h"
#include<QGraphicsSceneMoveEvent>
#include <QPen>

mazeGui::mazeGui(QObject *parent) :
    QGraphicsScene(parent)
{
    this->_wallPen = new QPen;
    _wallPen->setWidth(WALL_THICKNESS_PX);
}
void mazeGui::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if(mouseEvent->button() == Qt::LeftButton)
    {
        qDebug("caught mouse event at %f, %f", mouseEvent->scenePos().x(), mouseEvent->scenePos().y());

    }
}

QPen mazeGui::wallPen()
{
    return *_wallPen;
}
