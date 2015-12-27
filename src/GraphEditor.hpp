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

    bool addingNode;
    bool addingEdge;
	
    QGraphicsView *view;
    ogdf::Graph g;
	ogdf::GraphAttributes GA;
};

#endif
