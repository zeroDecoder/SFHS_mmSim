#include "mazeConst.h"
#include "mazegui.h"
#include<QGraphicsSceneMoveEvent>

mazeGui::mazeGui(QObject *parent) :
    QGraphicsScene(parent)
{
    this->_wallPen = new QPen(QColor(0xFF,0xFF,0xFF,0xFF));
    this->_guidePen = new QPen(QColor(0xFF,0xFF,0xFF,0x20));
    this->_wallPen->setWidth(WALL_THICKNESS_PX);
    this->_guidePen->setWidth(WALL_THICKNESS_PX);

    this->_bgGrid = this->createItemGroup(this->selectedItems());
    this->mazeWalls = this->createItemGroup(this->selectedItems());

    this->setSceneRect(QRectF(QPoint(0,0), QPoint(MAZE_WIDTH_PX,MAZE_HEIGHT_PX)));
}
void mazeGui::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
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

        int xmod = (int)mouseEvent->scenePos().x() % PX_PER_UNIT;
        int ymod = (int)mouseEvent->scenePos().y() % PX_PER_UNIT;
        int xpos = (int)mouseEvent->scenePos().x() / PX_PER_UNIT;
        int ypos = (int)mouseEvent->scenePos().y() / PX_PER_UNIT;

        if((xmod < PX_PER_UNIT/3  || xmod > 2*PX_PER_UNIT/3 )&& ymod > PX_PER_UNIT/3  && ymod < 2*PX_PER_UNIT/3)
        {
            if(xmod >= PX_PER_UNIT /2)
            {
                emit takeRightWall(QPoint(xpos, ypos));
            }
            else
            {
                emit takeLeftWall(QPoint(xpos, ypos));
            }
        }
        else if((ymod < PX_PER_UNIT/3  || ymod > 2*PX_PER_UNIT/3 )&& xmod > PX_PER_UNIT/3  && xmod < 2*PX_PER_UNIT/3)
        {
            if(ymod >= PX_PER_UNIT/2 || ymod == 0)
            {
                emit takeTopWall(QPoint(xpos, ypos));
            }
            else
            {
                emit takeBottomWall(QPoint(xpos, ypos));
            }
        }
    }
}

QPen mazeGui::wallPen()
{
    return *_wallPen;
}


void mazeGui::drawGuideLines()
{
    this->_bgGrid = this->createItemGroup(this->selectedItems());
    for(int i = 0 ; i <= MAZE_WIDTH; i++)
    {
       this->_bgGrid->addToGroup(this->addLine(i*PX_PER_UNIT,0,i*PX_PER_UNIT,MAZE_WIDTH_PX,*this->_guidePen));
    }
    for(int i = 0; i <= MAZE_HEIGHT; i++)
    {
       this->_bgGrid->addToGroup(this->addLine(0,i*PX_PER_UNIT,MAZE_HEIGHT_PX,i*PX_PER_UNIT, *this->_guidePen));
    }
}

void mazeGui::drawMaze(baseMapNode data[][MAZE_HEIGHT])
{
    //delete old maze walls from GUI
    this->removeItem(this->mazeWalls);
    while (this->mazeWalls->childItems().size()>0)
    {
       delete (this->mazeWalls->childItems().first());
    }
    this->addItem(this->mazeWalls);

    //Draw maze walls
    for(int i = 0; i < MAZE_WIDTH; i++)
    {
        for(int j = 0; j < MAZE_HEIGHT; j++)
        {
            baseMapNode *mover = &data[i][j];
            if(mover->isWallBottom())
            {
                this->mazeWalls->addToGroup(this->addLine(QLineF(i*PX_PER_UNIT,j*PX_PER_UNIT,(i+1)*PX_PER_UNIT,j*PX_PER_UNIT),*_wallPen));
            }
            if(mover->isWallTop())
            {
                this->mazeWalls->addToGroup(this->addLine(QLineF(i*PX_PER_UNIT,(j+1)*PX_PER_UNIT,(i+1)*PX_PER_UNIT,(j+1)*PX_PER_UNIT),*_wallPen));
            }
            if(mover->isWallLeft())
            {
                this->mazeWalls->addToGroup(this->addLine(QLineF(i*PX_PER_UNIT,j*PX_PER_UNIT, i*PX_PER_UNIT,(j+1)*PX_PER_UNIT),*_wallPen));
            }
            if(mover->isWallRight())
            {
                this->mazeWalls->addToGroup(this->addLine(QLineF((i+1)*PX_PER_UNIT,j*PX_PER_UNIT,(i+1)*PX_PER_UNIT,(j+1)*PX_PER_UNIT),*_wallPen));
            }
        }
    }
}

