/**
  This is Network Topologies Generator, a tool with the objective to assist network architects in creating new survivor topologies.
    Copyright (C) 2015  Silvana Trindade

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
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
    QPointF p = this->mapToScene(e->pos());


    foreach(QGraphicsItem *item, scene->items())
    {
        int type = item->type();

        QGraphicsEllipseItem *ellipse = (QGraphicsEllipseItem *) item;

        if (type != QGraphicsEllipseItem::Type)
        {
            continue;
        }

        QRectF rect = ellipse->boundingRect();

        if (rect.contains(p))
        {
            editor->addEdge(rect.x(), rect.y());
            t = true;
            break;
        }
    }

    if(t == false) editor->addNode(p.x() - 15, p.y() - 15);
}

void GraphicsView::mouseMoveEvent(QMouseEvent *e)
{
    QPointF p = this->mapToScene(e->pos());
    //nó selecionado
    foreach(QGraphicsItem *item, scene->items())
    {
        int type = item->type();
        QGraphicsEllipseItem *ellipse = (QGraphicsEllipseItem *) item;

        if (type != QGraphicsEllipseItem::Type)
        {
            continue;
        }

        QRectF rect = ellipse->boundingRect();

        if (rect.contains(p))
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
        int type = item->type();
        QGraphicsPathItem *path = (QGraphicsPathItem *) item;

        if (type != QGraphicsPathItem::Type)
        {
            continue;
        }


        if (path->contains(p))
        {
            path->setSelected(true);
        }
        else
        {
            path->setSelected(false);
        }
    }
    QGraphicsView::mouseMoveEvent(e);
}



void GraphicsView::mouseDoubleClickEvent(QMouseEvent *e)
{
    QPointF p = this->mapToScene(e->pos());
    //aresta selecionada para inserir distância
    foreach(QGraphicsItem *item, scene->items())
    {
        int type = item->type();

        if (type != QGraphicsPathItem::Type)
        {
            continue;
        }

        EditorPathItem *path = (EditorPathItem *) item;

        if (path->contains(p))
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

            if (distance < 1)
            {
                return;
            }

            //janela para digitar a distância entre um par de nós
            QString text = QInputDialog::getText(this, "Enter the distance of the link", "Distance:", QLineEdit::Normal, QString::fromStdString(to_string(distance)), &ok);

            if (ok)
            {
                distance = text.toInt();
                editor->GA.intWeight(edge) = distance;
                editor->rerender();
            }

            return;
        }
    }

}
