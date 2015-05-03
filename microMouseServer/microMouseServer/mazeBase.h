#ifndef MAZE_BASE_H
#define MAZE_BASE_H

#include"mazeConst.h"
#include<QGraphicsLineItem>
enum Wall {TOP,BOTTOM,LEFT,RIGHT};

class baseMapNode
{
private:
    int _x,_y;
    struct baseMapNode *_top, *_bottom, *_left, *_right;

public:
    baseMapNode();


    int posX();
    int posY();
    bool isWallTop();
    bool isWallLeft();
    bool isWallRight();
    bool isWallBottom();
    void setX(int val);
    void setY(int val);
    void setXY(int valX,int valY);
    void setWall(Wall pos, baseMapNode *addr);
    void clearWall(Wall pos);
};

#endif
