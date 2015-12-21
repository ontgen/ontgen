#include "DrawGraph.hpp"

DrawGraph::DrawGraph(Graph &g)
{
 
    // randomSimpleGraph(g, 10, 20); // grapth genarate.
 
    GraphAttributes GA( g, GraphAttributes::nodeGraphics |
        GraphAttributes::edgeGraphics |
        GraphAttributes::nodeLabel |
        GraphAttributes::nodeStyle |
        GraphAttributes::edgeType |
        GraphAttributes::edgeArrow |
        GraphAttributes::edgeStyle ); // Create graph attributes for this graph
 
    node v;
    forall_nodes( v, g ){ // iterate through all the node in the graph
        GA.fillColor( v ) = Color( "#FFFF00" ); // set node color to yellow
 
        GA.height( v ) = 20.0; // set the height to 20.0
        GA.width( v ) = 20.0; // set the width to 40.0
        GA.shape(v) = shEllipse;
 
        string s = to_string(v->index());
        char const *pchar = s.c_str(); //use char const* as target type
        GA.label( v ) = pchar;
    }
 
    edge e;
    forall_edges(e ,g) // set default edge color and type
    {
        GA.bends(e);
        GA.arrowType(e) = eaNone;
        GA.strokeColor(e) = Color("#0000FF");
    }
 
    SugiyamaLayout SL; //Compute a hierarchical drawing of G (using SugiyamaLayout)
    SL.setRanking( new OptimalRanking );
    SL.setCrossMin( new MedianHeuristic );
 
    OptimalHierarchyLayout *ohl = new OptimalHierarchyLayout;
 
    SL.setLayout( ohl );
    SL.call( GA );
 
    GraphIO::drawSVG( GA, "Output.svg" );
 
}

DrawGraph::~DrawGraph(){}

ogdf::Graph DrawGraph::constructGraphOGDF(Graph g)
{
	ogdf::Graph g;

	

	return g;
}