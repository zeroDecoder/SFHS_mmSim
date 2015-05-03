#include "micromouseserver.h"
#include "ui_micromouseserver.h"
#include "mazeConst.h"
#include "mazegui.h"
#include <QFileDialog>
#include <QFile>
#include <QTextStream>



microMouseServer::microMouseServer(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::microMouseServer)
{
    maze = new mazeGui;
    ui->setupUi(this);
    linkMenu();
    connect(this->maze, SIGNAL(passTopWall(QPoint)), this, SLOT(addTopWall(QPoint)));
    connect(this->maze, SIGNAL(passBottomWall(QPoint)), this, SLOT(addBottomWall(QPoint)));
    connect(this->maze, SIGNAL(passLeftWall(QPoint)), this, SLOT(addLeftWall(QPoint)));
    connect(this->maze, SIGNAL(passRightWall(QPoint)), this, SLOT(addRightWall(QPoint)));
    connect(this->maze, SIGNAL(takeBottomWall(QPoint)),this,SLOT(removeBottomWall(QPoint)));
    connect(this->maze, SIGNAL(takeTopWall(QPoint)),this,SLOT(removeTopWall(QPoint)));
    connect(this->maze, SIGNAL(takeLeftWall(QPoint)),this,SLOT(removeLeftWall(QPoint)));
    connect(this->maze, SIGNAL(takeRightWall(QPoint)),this,SLOT(removeRightWall(QPoint)));



    //setup graphics scene
    ui->graphics->scale(1,-1);
    ui->graphics->setBackgroundBrush(QBrush(Qt::black));
    ui->graphics->setAutoFillBackground(true);
    ui->graphics->setScene(maze);

    this->initMaze();
    this->maze->drawGuideLines();
    this->maze->drawMaze(this->mazeData);
}


microMouseServer::~microMouseServer()
{
    delete ui;
}


void microMouseServer::on_tabWidget_tabBarClicked(int index)
{
    index = index;
}


void microMouseServer::linkMenu()
{
    connect(ui->menu_loadMaze, SIGNAL(triggered()), this, SLOT(loadMaze()));
    connect(ui->menu_saveMaze, SIGNAL(triggered()), this, SLOT(saveMaze()));
}


void microMouseServer::loadMaze()
{
    QString fileName = QFileDialog::getOpenFileName(this,
             tr("Open Maze File"), "./", tr("Maze Files (*.maz)"));
    QFile inFile(fileName);

    if (!inFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        ui->txt_debug->append("ERROR 202: file not found");
        return;
    }

    //read maze
    QTextStream mazeFile(&inFile);

    int largestX =0, largestY=0;
    int x, y, wallTop, wallBottom, wallLeft,wallRight;
    while(!mazeFile.atEnd())
    {
        mazeFile >> x;
        mazeFile >> y;
        mazeFile >> wallTop;
        mazeFile >> wallBottom;
        mazeFile >> wallLeft;
        mazeFile >> wallRight;

        //check formating
        if(x<0 || y<0 || wallTop<0 || wallBottom<0 || wallLeft<0 || wallRight<0 ||
           wallTop>1 || wallBottom>1 || wallLeft>1 || wallRight>1)
        {
            ui->txt_debug->append("ERROR 201: file formating error");
            return;
        }

        //check x boundary
        if(x > largestX)
        {
            if(x > MAZE_WIDTH)
            {
                ui->txt_debug->append("ERROR 204: maze file is wider than max maze size");
            }
            else
            {
                largestX = x;
            }
        }

        //check y boundary
        if(y > largestY)
        {
            if(y > MAZE_HEIGHT)
            {
                ui->txt_debug->append("ERROR 205: maze file is taller than max maze size");
            }
            else
            {
                largestY = y;
            }
        }

        baseMapNode *mover = &this->mazeData[x-1][y-1];
        //load data into maze
        mover->setXY(x,y);

        if(wallLeft)
        {
             mover->setWall(LEFT, NULL);
        }
        else
        {
            mover->setWall(LEFT, &this->mazeData[x-1][y]);
        }

        if(wallRight)
        {
            mover->setWall(RIGHT, NULL);
        }
        else
        {
            mover->setWall(RIGHT, &this->mazeData[x+1][y]);
        }
        if(wallTop)
        {
            mover->setWall(TOP, NULL);
        }
        else
        {
            mover->setWall(TOP, &this->mazeData[x][y+1]);
        }

        if(wallBottom)
        {
            mover->setWall(BOTTOM, NULL);
        }
        else
        {
            mover->setWall(BOTTOM, &this->mazeData[x][y-1]);
        }
    }
    ui->txt_debug->append("Maze loaded");
    mazeFile.flush();
    inFile.close();
    this->maze->drawMaze(this->mazeData);
}


void microMouseServer::saveMaze()
{
    QString fileName = QFileDialog::getSaveFileName(this,
             tr("Select Maze File"), "", tr("Maze Files (*.maz)"));
    QFile inFile(fileName);
    if (!inFile.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        ui->txt_debug->append("ERROR 202: file not found");
        return;
    }
    else
    {
        //read maze
        QTextStream mazeFile(&inFile);

        for(int i = 0; i < MAZE_WIDTH; i++)
        {
            for(int j = 0; j < MAZE_HEIGHT; j++)
            {
                int top = this->mazeData[i][j].isWallTop();
                int bottom = this->mazeData[i][j].isWallBottom();
                int left = this->mazeData[i][j].isWallLeft();
                int right = this->mazeData[i][j].isWallRight();

                mazeFile << this->mazeData[i][j].posX() << " " << this->mazeData[i][j].posY() << " " << top << " " << bottom << " " << left << " " << right << endl;
            }
        }

        mazeFile.flush();

        inFile.close();
        ui->txt_debug->append("Maze Saved to File.");
    }
}

void microMouseServer::addLeftWall(QPoint cell)
{
    this->mazeData[cell.x()][cell.y()].setWall(LEFT, NULL);
    if(cell.x() > 0)this->mazeData[cell.x()-1][cell.y()].setWall(RIGHT,NULL);
    this->maze->drawMaze(this->mazeData);
}

void microMouseServer::addRightWall(QPoint cell)
{
    this->mazeData[cell.x()][cell.y()].setWall(RIGHT, NULL);
    if(cell.x() < MAZE_WIDTH )this->mazeData[cell.x()+1][cell.y()].setWall(LEFT, NULL);
    this->maze->drawMaze(this->mazeData);
}

void microMouseServer::addTopWall(QPoint cell)
{
    this->mazeData[cell.x()][cell.y()].setWall(TOP, NULL);
    if(cell.y() < MAZE_HEIGHT)this->mazeData[cell.x()][cell.y()+1].setWall(BOTTOM,NULL);
    this->maze->drawMaze(this->mazeData);
}

void microMouseServer::addBottomWall(QPoint cell)
{
    this->mazeData[cell.x()][cell.y()].setWall(BOTTOM, NULL);
    if(cell.y() > 0)this->mazeData[cell.x()][cell.y()-1].setWall(TOP,NULL);
    this->maze->drawMaze(this->mazeData);
}

void microMouseServer::initMaze()
{
    for(int y =0; y < MAZE_HEIGHT; y++)
    {
        for(int x = 0; x < MAZE_WIDTH; x++)
        {
            baseMapNode *mover = &this->mazeData[x][y];
            mover->setXY(x+1,y+1);
            if(x == 0)
            {
                mover->setWall(LEFT, NULL);
            }
            else
            {
                mover->setWall(LEFT, &this->mazeData[x-1][y]);
            }
            if(x == MAZE_WIDTH-1)
            {
                mover->setWall(RIGHT, NULL);
            }
            else
            {
                mover->setWall(RIGHT, &this->mazeData[x+1][y]);
            }
            if(y == 0)
            {
                mover->setWall(BOTTOM, NULL);
            }
            else
            {
                mover->setWall(BOTTOM,  &this->mazeData[x][y-1]);
            }
            if(y == MAZE_HEIGHT-1)
            {
                mover->setWall(TOP, NULL);
            }
            else
            {
                mover->setWall(TOP,  &this->mazeData[x][y+1]);
            }
        }
    }
}

void microMouseServer::removeRightWall(QPoint cell)
{
    if(cell.x() < MAZE_WIDTH -1)
    {
        this->mazeData[cell.x()][cell.y()].setWall(RIGHT, &this->mazeData[cell.x()+1][cell.y()]);
        this->mazeData[cell.x()+1][cell.y()].setWall(LEFT, &this->mazeData[cell.x()][cell.y()]);
    }
    this->maze->drawMaze(this->mazeData);
}

void microMouseServer::removeLeftWall(QPoint cell)
{
    if(cell.x() > 0)
    {
        this->mazeData[cell.x()][cell.y()].setWall(LEFT, &this->mazeData[cell.x()-1][cell.y()]);
        this->mazeData[cell.x()-1][cell.y()].setWall(RIGHT, &this->mazeData[cell.x()][cell.y()]);
    }
    this->maze->drawMaze(this->mazeData);
}

void microMouseServer::removeTopWall(QPoint cell)
{
    if(cell.y() < MAZE_HEIGHT -1)
    {
        this->mazeData[cell.x()][cell.y()].setWall(TOP, &this->mazeData[cell.x()][cell.y()+1]);
        this->mazeData[cell.x()][cell.y()+1].setWall(BOTTOM, &this->mazeData[cell.x()][cell.y()]);
    }
    this->maze->drawMaze(this->mazeData);
}

void microMouseServer::removeBottomWall(QPoint cell)
{
    if(cell.y() > 0)
    {
        this->mazeData[cell.x()][cell.y()].setWall(BOTTOM, &this->mazeData[cell.x()][cell.y()-1]);
        this->mazeData[cell.x()][cell.y()-1].setWall(TOP, &this->mazeData[cell.x()][cell.y()]);
    }
    this->maze->drawMaze(this->mazeData);
}


