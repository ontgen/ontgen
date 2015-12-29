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
#include "DrawGraph.hpp"

DrawGraph::DrawGraph(Graph &graph, Plane &plane,string dateTime, int index,int simulation)
{
 
    ogdf::Graph g = constructGraphOGDF(graph,plane);//constrói grafo no formato da biblioteca

    ogdf::GraphAttributes GA( g, ogdf::GraphAttributes::nodeGraphics |
        ogdf::GraphAttributes::edgeGraphics |
        ogdf::GraphAttributes::nodeLabel |
        ogdf::GraphAttributes::nodeStyle |
        ogdf::GraphAttributes::edgeType |
        ogdf::GraphAttributes::edgeArrow |
        ogdf::GraphAttributes::edgeStyle ); // cria atributos para o grafo
 
    ogdf::node v;

    int i = 0, f = 50, fn = 20, fontSize = 12;

    if(plane.getSqrtArea() > 30)
    {
        f = 18;
        fn = 50;
        fontSize = 24;
    }

    forall_nodes( v, g ){ // itera sobre todos os nós do grafo
        GA.fillColor( v ) = ogdf::Color( "#fff" );//adiciona cor branca ao interior do nó
 
        GA.height( v ) = fn; // set the height to
        GA.width( v ) = fn; // set the width to

        GA.shape(v) = ogdf::shEllipse;
 
        string s = to_string(v->index());
        char const *pchar = s.c_str(); //use char const* as target type
        GA.label( v ) = pchar;

        //posições x e y da imagem
        GA.x(v) = plane.getCoordinateX(i)*f;
        GA.y(v) = plane.getCoordinateY(i)*f;

        i++;
    }
 
    ogdf::edge e;
    forall_edges(e ,g) // adiciona cor as arestas
    {
        GA.bends(e);
        GA.arrowType(e) = ogdf::eaNone;
        GA.strokeColor(e) = ogdf::Color("#444");
        GA.strokeWidth(e) = 3;
    }
 
    ogdf::SugiyamaLayout SL; //Computa o desenho hierárquico do grafo g (usando SugiyamaLayout)
    SL.setRanking( new ogdf::OptimalRanking );
    SL.setCrossMin( new ogdf::MedianHeuristic );
 
    ogdf::GraphIO::SVGSettings s;
    s.fontSize(fontSize);

    QString temp = QDir::homePath();
    temp.append("/simulations");
    /**
     * Cria diretório caso não exista
     */
    QDir dir(temp);

    if (!dir.exists())
    {
        dir.mkpath(".");
    }

    //adicionando caminho e nome de arquivo completo
    //indice da simulação+indice da topologia+instante da simulação
    temp.append("/topology_");
    temp.append(QString::fromStdString(to_string(simulation)));
    temp.append("_");
    temp.append(QString::fromStdString(to_string(index)));
    temp.append("_");
    temp.append(QString::fromStdString(dateTime));
    temp.append(".svg");
    this->file = temp.toStdString();
    
    ogdf::GraphIO::drawSVG( GA,this->file, s );//gera imagem
 
}

DrawGraph::~DrawGraph(){}

string DrawGraph::getFile()
{
    return this->file;
}

ogdf::Graph DrawGraph::constructGraphOGDF(Graph &g,Plane &plane)
{
	ogdf::Graph _g;
	         
	//cria todos os nós do grafo   
	for(int i = 0; i < g.getNumberOfNodes(); i++)
	{
	    nodes.push_back(_g.newNode());
	}

    vector<Node> n = g.getNodes();
    mAdjacents = vector<vector<double>> (g.getNumberOfNodes(),vector<double>(g.getNumberOfNodes(),0.0f));
    
    //Constrói matriz de adjacencias
    for (int i = 0; i < g.getNumberOfNodes(); i++)
    {
        vector<int> adjacents = n[i].getAdjacentsNodes();

        for (int j = 0; j < (int)adjacents.size(); j++)
        {
            mAdjacents[i][ adjacents[j] ] = n[i].getWeightEdge(j);
        }
    }

	//faz as ligações entre os nós
	for (int i = 0; i < g.getNumberOfNodes()-1; i++)
	{
		for (int j = i+1; j < g.getNumberOfNodes(); j++)
		{
            if(mAdjacents[i][j] > 0.0f) _g.newEdge(nodes[i], nodes[j]);
		}
	}

	return _g;
}
