#ifndef MICROMOUSESERVER_H
#define MICROMOUSESERVER_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsItem>

namespace Ui {
class microMouseServer;
}

class microMouseServer : public QMainWindow
{
    Q_OBJECT

public:
    explicit microMouseServer(QWidget *parent = 0);
    ~microMouseServer();

private slots:
    void on_tabWidget_tabBarClicked(int index);

private:
    Ui::microMouseServer *ui;

    QGraphicsScene *map;
    QGraphicsEllipseItem *mouse;
};

#endif // MICROMOUSESERVER_H
