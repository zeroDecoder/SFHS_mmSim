#include "micromouseserver.h"
#include "ui_micromouseserver.h"

microMouseServer::microMouseServer(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::microMouseServer)
{
    ui->setupUi(this);
}

microMouseServer::~microMouseServer()
{
    delete ui;
}
