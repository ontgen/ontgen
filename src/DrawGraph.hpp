#include <QtCore>
#include "xlsxdocument.h"
#include "xlsxconditionalformatting.h"

#include "Plane.hpp"

class DrawGraph
{
public:
	DrawGraph(Graph &, Plane &plane,string,int,int);
	~DrawGraph();
    ogdf::Graph constructGraphOGDF(Graph &,Plane &);
    string getFile();

	string file;//nome do arquivo que será armazenado a imagem
    vector<vector<double>> mAdjacents;//matriz de adjacências com a distância entre cada par de nós
    std::vector<ogdf::node> nodes;//nós do grafo
};
