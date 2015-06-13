#include "mazeConst.h"
#include "mazegui.h"
#include<QGraphicsSceneMoveEvent>

mazeGui::mazeGui(QObject *parent) :
    QGraphicsScene(parent)
{
    //initialize gui colors
    this->_wallPen = new QPen(QColor(0xFF,0xFF,0xFF,0xFF));
    this->_guidePen = new QPen(QColor(0xFF,0xFF,0xFF,0x20));
    this->_mousePen = new QPen(QColor(0xFF,0xFF,0x00,0xFF));
    this->_mouseBrush = new QBrush(QColor(0xFF, 0xFF, 0X00, 0xFF));
    this->_wallPen->setWidth(WALL_THICKNESS_PX);
    this->_guidePen->setWidth(WALL_THICKNESS_PX);

    //initialize graphics groups
    this->_bgGrid = this->createItemGroup(this->selectedItems());
    this->mazeWalls = this->createItemGroup(this->selectedItems());
    this->_mouse = NULL;

    //Generate maze window
    this->setSceneRect(QRectF(QPoint(0,0), QPoint(MAZE_WIDTH_PX,MAZE_HEIGHT_PX)));

    _mousePos.setX(1);
    _mousePos.setY(1);
    _mouseDir = dUP;
}

mazeGui::~mazeGui()
{
    delete _wallPen;
    delete _guidePen;
    delete _mousePen;
    delete _mouseBrush;
    delete _bgGrid;
    delete mazeWalls;
    delete _mouse;
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

        //This needs to be redone so it is more robust, missed clicks when x/ymod=0;
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
    //checks for right clicks, very inefficient since the bound checks are done twice.
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
       //this is super inefficient, causes noticeable lag when changing maze
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
    drawMouse(_mousePos, _mouseDir);
}

void mazeGui::drawMouse(QPoint cell, mDirection direction)
{
    _mouseDir = direction;
    _mousePos = cell;
    //math!
    float xPos = (cell.x()-1) * PX_PER_UNIT + PX_PER_UNIT *.125;
    float yPos = (cell.y()-1) * PX_PER_UNIT + PX_PER_UNIT *.125;
    QRectF boundingBox(xPos,
                       yPos,
                       PX_PER_UNIT * .75,
                       PX_PER_UNIT * .75);

    int startAngle = (45 + 90*direction)*16, spanAngle = (360-90)*16;
    QGraphicsEllipseItem *delHolder = _mouse;
    _mouse = this->addEllipse(boundingBox, *_mousePen, *_mouseBrush);
    _mouse->setStartAngle(startAngle);
    _mouse->setSpanAngle(spanAngle);
    if(delHolder) delete delHolder;
}

int mazeGui::mouseX()
{
   return _mousePos.x();
}
int mazeGui::mouseY()
{
    return _mousePos.y();
}
mDirection mazeGui::mouseDir()
{
    return _mouseDir;
}
