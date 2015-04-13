#include "micromouseserver.h"
#include "ui_micromouseserver.h"
#include "mazeConst.h"
#include <mazegui.h>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>



microMouseServer::microMouseServer(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::microMouseServer)
{
    map = new mazeGui;
    //setup ui and interface
    ui->setupUi(this);
    linkMenu();
    connect(this->map, SIGNAL(passWall(QLineF*)), this, SLOT(addWall(QLineF*)));

    //setup graphics scene
    ui->graphics->setBackgroundBrush(QBrush(Qt::black));
    ui->graphics->setAutoFillBackground(true);
    bgGrid = map->createItemGroup(map->selectedItems());
    mazeWalls = map->createItemGroup(map->selectedItems());



    map->setSceneRect(QRectF(QPoint(0,0), QPoint(MAZE_WIDTH_PX,MAZE_HEIGHT_PX)));
    ui->graphics->setScene(map);

    drawGuideLines();
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
        //load data into maze
        this->mazeData[x-1][y-1].x = x;
        this->mazeData[x-1][y-1].y = y;

        if(wallLeft)
        {
            this->mazeData[x-1][y-1].wallLeft = true;
            this->mazeData[x-1][y-1].left = NULL;
        }
        else
        {
            this->mazeData[x-1][y-1].wallLeft = false;
            this->mazeData[x-1][y-1].left = &this->mazeData[x-2][y-1];
        }

        if(wallRight)
        {
            this->mazeData[x-1][y-1].wallRight = true;
            this->mazeData[x-1][y-1].right = NULL;
        }
        else
        {
            this->mazeData[x-1][y-1].wallRight = false;
            this->mazeData[x-1][y-1].right = &this->mazeData[x][y-1];
        }

        if(wallTop)
        {
            this->mazeData[x-1][y-1].wallTop = true;
            this->mazeData[x-1][y-1].top = NULL;
        }
        else
        {
            this->mazeData[x-1][y-1].wallTop = false;
            this->mazeData[x-1][y-1].top = &this->mazeData[x-1][y];
        }

        if(wallBottom)
        {
            this->mazeData[x-1][y-1].wallBottom = true;
            this->mazeData[x-1][y-1].bottom = NULL;
        }
        else
        {
            this->mazeData[x-1][y-1].wallBottom = false;
            this->mazeData[x-1][y-1].bottom = &this->mazeData[x-1][y-2];
        }
    }
    ui->txt_debug->append("Maze loaded");
    mazeFile.flush();
    inFile.close();
    this->drawMaze();
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

    //read maze
    QTextStream mazeFile(&inFile);

    for(int i = 0; i < MAZE_WIDTH; i++)
    {
        for(int j = 0; j < MAZE_HEIGHT; j++)
        {

            mazeFile << this->mazeData[i][j].x << " " << this->mazeData[i][j].y << " " << this->mazeData[i][j].wallTop << " " << this->mazeData[i][j].wallBottom << " " << this->mazeData[i][j].wallLeft << " " << this->mazeData[i][j].wallRight << endl;
        }
    }

    mazeFile.flush();
    inFile.close();
    ui->txt_debug->append("Maze Saved to File.");
}


void microMouseServer::drawMaze()
{
    //delete old maze walls from GUI
    this->map->removeItem(mazeWalls);
    while (mazeWalls->childItems().size()>0)
    {
       delete (mazeWalls->childItems().first());
    }
    this->map->addItem(mazeWalls);

    QPen wallPen(QColor(0xFF,0xFF,0xFF,0xFF));
    wallPen.setWidth(WALL_THICKNESS_PX);

    //Draw maze walls
    for(int i = 0; i < MAZE_WIDTH; i++)
    {
        for(int j = 0; j < MAZE_HEIGHT; j++)
        {
            if(this->mazeData[i][j].wallBottom)
            {
                this->mazeWalls->addToGroup(
                            this->map->addLine(
                                QLineF(i*PX_PER_UNIT,MAZE_WIDTH*PX_PER_UNIT-j*PX_PER_UNIT,(i+1)*PX_PER_UNIT,MAZE_WIDTH*PX_PER_UNIT-j*PX_PER_UNIT),
                                wallPen));
            }
            if(this->mazeData[i][j].wallTop)
            {
                this->mazeWalls->addToGroup(
                            this->map->addLine(
                                QLineF(i*PX_PER_UNIT,MAZE_WIDTH*PX_PER_UNIT-(j+1)*PX_PER_UNIT,(i+1)*PX_PER_UNIT,MAZE_WIDTH*PX_PER_UNIT-(j+1)*PX_PER_UNIT),
                                wallPen));

            }
            if(this->mazeData[i][j].wallLeft)
            {
                this->mazeWalls->addToGroup(
                            this->map->addLine(
                                QLineF(i*PX_PER_UNIT,MAZE_WIDTH*PX_PER_UNIT-j*PX_PER_UNIT, i*PX_PER_UNIT,MAZE_WIDTH*PX_PER_UNIT-(j+1)*PX_PER_UNIT),
                                wallPen));
            }
            if(this->mazeData[i][j].wallRight)
            {
                this->mazeWalls->addToGroup(
                            this->map->addLine(
                                QLineF((i+1)*PX_PER_UNIT,MAZE_WIDTH*PX_PER_UNIT-j*PX_PER_UNIT,(i+1)*PX_PER_UNIT,MAZE_WIDTH*PX_PER_UNIT-(j+1)*PX_PER_UNIT),
                                wallPen));
            }
        }
    }
}


void microMouseServer::drawGuideLines()
{
    QPen wallPen(QColor(0xFF,0xFF,0xFF,0x20));
    wallPen.setWidth(WALL_THICKNESS_PX);

    for(int i = 0 ; i <= MAZE_WIDTH; i++)
    {
       this->bgGrid->addToGroup(this->map->addLine(QLineF(i*PX_PER_UNIT,0,i*PX_PER_UNIT,MAZE_WIDTH_PX),wallPen));
    }
    for(int i = 0; i <= MAZE_HEIGHT; i++)
    {
       this->bgGrid->addToGroup(this->map->addLine(QLineF(0,i*PX_PER_UNIT,MAZE_HEIGHT_PX,i*PX_PER_UNIT), wallPen));
    }
}


void microMouseServer::removeGuideLines()
{
    this->map->removeItem(bgGrid);
    while(bgGrid->childItems().size()>0)
    {
        delete bgGrid->childItems().first();
    }
}

void microMouseServer::addWall(QLineF *wall)
{
    this->mazeWalls->addToGroup(this->map->addLine(*wall, this->map->wallPen()));
}



