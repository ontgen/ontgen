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
