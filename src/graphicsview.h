#ifndef GRAPHICSVIEW_H
#define GRAPHICSVIEW_H

#include "graphicsview.h"
#include <QMouseEvent>
#include "GraphEditor.hpp"

class GraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit GraphicsView(QWidget *parent=0);
    GraphEditor *editor;

signals:

public slots:
    void mousePressEvent(QMouseEvent *e);
private:
    QGraphicsScene *scene;
};

#endif // GRAPHICSVIEW_H
