#ifndef MICROMOUSESERVER_H
#define MICROMOUSESERVER_H

#include <QMainWindow>

namespace Ui {
class microMouseServer;
}

class microMouseServer : public QMainWindow
{
    Q_OBJECT

public:
    explicit microMouseServer(QWidget *parent = 0);
    ~microMouseServer();

private:
    Ui::microMouseServer *ui;
};

#endif // MICROMOUSESERVER_H
