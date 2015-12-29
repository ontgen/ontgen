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
#ifndef GRAPH_EDITOR_H
#define GRAPH_EDITOR_H

#include "Graph.hpp"
#include "svgview.h"
#include <QFile>
#include <QGraphicsScene>

class GraphEditor
{
public:
    GraphEditor(QGraphicsView*);
	~GraphEditor();

	void addNode(int x, int y);
	void rerender();
    void addEdge(int x,int y);//adiciona aresta se for o segundo nó selecionado
    bool checkEdgeExists(int,int);
    void saveAsSVG(QString);
    void clearGraph();
    void constructGraph(Graph &);

    bool addingNode;
    bool addingEdge;
	
    QGraphicsView *view;
    ogdf::Graph g;
	ogdf::GraphAttributes GA;
    ogdf::node selected;
    int nSelected;
};

#endif
