#ifndef MAZEGUI_H
#define MAZEGUI_H
#include <QLineF>
#include <QPen>
#include <QGraphicsScene>

class mazeGui : public QGraphicsScene
{
    Q_OBJECT
public:
    QPen wallPen();
    explicit mazeGui(QObject *parent = 0);
    virtual void mousePressEvent(QGraphicsSceneMouseEvent * mouseEvent);

signals:
    void passRightWall(QPoint cell);
    void passLeftWall(QPoint cell);
    void passTopWall(QPoint cell);
    void passBottomWall(QPoint cell);

public slots:

private:
    QPen *_wallPen;
    QPen *_guidePen;

};

#endif // MAZEGUI_H
