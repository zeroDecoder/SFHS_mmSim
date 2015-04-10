#include "micromouseserver.h"
#include "ui_micromouseserver.h"
#include "mazeConst.h"

microMouseServer::microMouseServer(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::microMouseServer)
{
    ui->setupUi(this);
    ui->graphics->setBackgroundBrush(QBrush(Qt::black));
    ui->graphics->setAutoFillBackground(true);

    map = new QGraphicsScene(this);
    ui->graphics->setScene(map);

    QBrush blackBrush(Qt::black);
    QBrush greenBrush(Qt::green);
    QPen wallPen(Qt::white);
    wallPen.setWidth(WALL_THICKNESS_PX);

    mouse = map->addEllipse(100,0,80,100, wallPen, greenBrush);



}

microMouseServer::~microMouseServer()
{
    delete ui;
}

void microMouseServer::on_tabWidget_tabBarClicked(int index)
{

    index = index;
}
