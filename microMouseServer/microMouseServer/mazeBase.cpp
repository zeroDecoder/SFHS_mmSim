#include "mazeBase.h"

baseMapNode::baseMapNode()
{
    this->_bottom=NULL;
    this->_top=NULL;
    this->_left=NULL;
    this->_right=NULL;
    this->_x =0;
    this->_y =0;
}

int baseMapNode::posX()
{

    return this->_x;
}

int baseMapNode::posY()
{
    return this->_y;
}
bool baseMapNode::isWallTop()
{
    return this->_top?0:1;
}
bool baseMapNode::isWallLeft()
{
    return this->_left?0:1;
}
bool baseMapNode::isWallRight()
{
    return this->_right?0:1;
}
bool baseMapNode::isWallBottom()
{
    return this->_bottom?0:1;
}

void baseMapNode::setX(int val)
{
    this->_x = val;
}

void baseMapNode::setY(int val)
{
   this->_y = val;
}

void baseMapNode::setXY(int valX, int valY)
{
    this->_x = valX;
    this->_y = valY;
}

void baseMapNode::setWall(Wall pos, baseMapNode *addr)
{
    if(pos == TOP)
    {
        this->_top = addr;
    }
    else if(pos == BOTTOM)
    {
        this->_bottom = addr;
    }
    else if(pos == LEFT)
    {
        this->_left = addr;
    }
    else if(pos == RIGHT)
    {
        this->_right = addr;
    }

}
