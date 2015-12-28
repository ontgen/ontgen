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

    for (auto iterator = scene->items().begin(); iterator != scene->items().end(); iterator++)
    {
        QGraphicsItem *item = *iterator;

        if (!item)
        {
            break;
        }

        QRectF rect = item->boundingRect();

        if (rect.contains(e->pos()))
        {
            editor->addEdge(rect.x(), rect.y());
            t = true;
            break;
        }
    }

    if(t == false) editor->addNode(e->pos().x(), e->pos().y());
}
