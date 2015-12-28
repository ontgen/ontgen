#include "graphicsview.h"
#include <QRectF>
#include <QGraphicsItem>
#include <QInputDialog>
#include <QDir>
#include "editorpathitem.h"

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

void GraphicsView::mouseMoveEvent(QMouseEvent *e)
{
    //nó selecionado
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
            ellipse->setSelected(true);
        }
        else
        {
            ellipse->setSelected(false);
        }
    }

    //aresta selecionada
    foreach(QGraphicsItem *item, scene->items())
    {
        QGraphicsPathItem *path = (QGraphicsPathItem *) item;

        if (!path)
        {
            break;
        }


        if (path->contains(e->pos()))
        {
            path->setSelected(true);
        }
        else
        {
            path->setSelected(false);
        }
    }
}



void GraphicsView::mouseDoubleClickEvent(QMouseEvent *e)
{

    //aresta selecionada
    foreach(QGraphicsItem *item, scene->items())
    {
        QGraphicsPathItem *path = (QGraphicsPathItem *) item;

        if (!path)
        {
            break;
        }


        if (path->contains(e->pos()))
        {
            path->setSelected(true);
            bool ok;
            int index = ((EditorPathItem *)path)->edgeIndex;
            int distance = 0;
            ogdf::edge t;
            ogdf::edge edge;

            int k = 0;

            forall_edges(t, editor->g)
            {
                if (k == index)
                {
                    distance = editor->GA.intWeight(t);
                    edge = t;
                    break;
                }

                k++;
            }

            QString text = QInputDialog::getText(this, "Enter the distance of the link", "Distance:", QLineEdit::Normal, QString::fromStdString(to_string(distance)), &ok);
            cout << "text: " << text.toStdString() << endl;
            cout << "ok? " << ok << endl;
            cout << "edge index " << index << endl;

            if (ok) {
                distance = text.toDouble();
                editor->GA.intWeight(edge) = distance;
                editor->rerender();
            }
        }
    }

}
