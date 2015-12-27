#include "graphicsview.h"

GraphicsView::GraphicsView(QWidget *parent) : QGraphicsView(parent)
{

}

void GraphicsView::mousePressEvent(QMouseEvent *e)
{
    editor->addNode(e->pos().x(), e->pos().y());
}

