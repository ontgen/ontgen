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
#include "editorpathitem.h"
#include <QPainter>
#include <iostream>

//troca de cor quando passar o mouse por cima da aresta
void EditorPathItem::hoverEnterEvent(QGraphicsSceneHoverEvent * event)
{
    isHover = true;
    update();
}

//volta a cor normal da aresta
void EditorPathItem::hoverLeaveEvent(QGraphicsSceneHoverEvent * event)
{
    isHover = false;
    update();
}

void EditorPathItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QPen pen("#333");
    pen.setWidth(2);

    if (isHover) {
        pen.setColor(QColor("#0000ff"));
    }

    painter->setPen(pen);

    painter->drawPath(this->path());
}

int EditorPathItem::type()
{
    return QGraphicsPathItem::Type;
}
