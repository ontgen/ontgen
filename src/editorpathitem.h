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
#ifndef EDITORPATHITEM_H
#define EDITORPATHITEM_H

#include <QGraphicsPathItem>

class EditorPathItem : public QGraphicsPathItem
{
public:

    void hoverLeaveEvent(QGraphicsSceneHoverEvent *);
    void hoverEnterEvent(QGraphicsSceneHoverEvent *);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    int type();

    int edgeIndex;
    bool isHover;
};

#endif // EDITORPATHITEM_H
