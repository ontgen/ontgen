#include <ogdf/basic/graphics.h>
#include <ogdf/basic/Graph.h>
#include <ogdf/basic/graph_generators.h>
#include <ogdf/layered/DfsAcyclicSubgraph.h>
#include <ogdf/fileformats/GraphIO.h>
#include <ogdf/layered/SugiyamaLayout.h>
#include <ogdf/layered/OptimalRanking.h>
#include <ogdf/layered/MedianHeuristic.h>
#include <ogdf/layered/OptimalHierarchyLayout.h>

#include "Graph.hpp"

class DrawGraph
{
public:
	DrawGraph(Graph &,string);
	~DrawGraph();

	string file;//nome do arquivo que será armazenado a imagem
};