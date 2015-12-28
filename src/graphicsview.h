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
    QGraphicsScene *scene;

signals:

public slots:
    void mousePressEvent(QMouseEvent *e);//preciona o nó
    void mouseMoveEvent(QMouseEvent *e);//passa com o mouse sobre a aresta
    void mouseDoubleClickEvent(QMouseEvent *e);
};

#endif // GRAPHICSVIEW_H
