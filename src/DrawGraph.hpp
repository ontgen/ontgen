#include <ogdf/basic/graphics.h>
#include <ogdf/basic/Graph.h>
#include <ogdf/basic/graph_generators.h>
#include <ogdf/layered/DfsAcyclicSubgraph.h>
#include <ogdf/fileformats/GraphIO.h>
#include <ogdf/layered/SugiyamaLayout.h>
#include <ogdf/layered/OptimalRanking.h>
#include <ogdf/layered/MedianHeuristic.h>
#include <ogdf/layered/OptimalHierarchyLayout.h>

#include "Plane.hpp"

class DrawGraph
{
public:
	DrawGraph(Graph &, Plane &plane,string);
	~DrawGraph();
    ogdf::Graph constructGraphOGDF(Graph &,Plane &);

	string file;//nome do arquivo que será armazenado a imagem
    vector<vector<double>> mAdjacents;//matriz de adjacências com a distância entre cada par de nós
    std::vector<ogdf::node> nodes;//nós do grafo
};
