#include "DrawGraph.hpp"

DrawGraph::DrawGraph(Graph &graph, Plane &plane,string file)
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
    this->file = "Output.svg";
    ogdf::GraphIO::drawSVG( GA, this->file, s );//gera imagem
 
}

DrawGraph::~DrawGraph(){}

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
            cout<<" "<<i<<" "<<j<<" = "<<n[i].getWeightEdge(j)<<endl;
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
