#include "graphicsview.h"
#include <QRectF>
#include <QGraphicsItem>

GraphicsView::GraphicsView(QWidget *parent) : QGraphicsView(parent)
{
    setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform | QPainter::HighQualityAntialiasing);
}

void GraphicsView::mousePressEvent(QMouseEvent *e)
{
    bool t = false;

    foreach(QGraphicsItem *item, scene->items())
    {
        QGraphicsEllipseItem *ellipse = (QGraphicsEllipseItem *) item;

        if (!ellipse)
        {
            break;
        }

        QRectF rect = ellipse->boundingRect();

        if (rect.contains(e->pos()))
        {
            editor->addEdge(rect.x(), rect.y());
            t = true;
            break;
        }
    }

    if(t == false) editor->addNode(e->pos().x(), e->pos().y());
}
