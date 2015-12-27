#include "Graph.hpp"
#include "svgview.h"
#include <QFile>

class GraphEditor
{
public:
    GraphEditor(SvgView*);
	~GraphEditor();

	void addNode(int x, int y);
	void rerender();
	
	SvgView *svg;
    ogdf::Graph g;
	ogdf::GraphAttributes GA;
};
