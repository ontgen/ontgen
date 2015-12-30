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
        EditorPathItem *path = (EditorPathItem *) item;

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

            if (distance < 1) {
                return;
            }
            //janela para digitar a distância entre um par de nós
            QString text = QInputDialog::getText(this, "Enter the distance of the link", "Distance:", QLineEdit::Normal, QString::fromStdString(to_string(distance)), &ok);

            if (ok) {
                distance = text.toDouble();
                editor->GA.intWeight(edge) = distance;
                editor->rerender();
            }

            return;
        }
    }

}
