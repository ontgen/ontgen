#include "GraphEditor.hpp"

GraphEditor::GraphEditor(SvgView *s)
{
    this->svg = s;
    this->GA = ogdf::GraphAttributes( this->g, ogdf::GraphAttributes::nodeGraphics |
        ogdf::GraphAttributes::edgeGraphics |
        ogdf::GraphAttributes::nodeLabel |
        ogdf::GraphAttributes::nodeStyle |
        ogdf::GraphAttributes::edgeType |
        ogdf::GraphAttributes::edgeArrow |
        ogdf::GraphAttributes::edgeStyle ); // cria atributos para o grafo
 
}
GraphEditor::~GraphEditor(){}

void GraphEditor::addNode(int x, int y)
{
    cout<<"um"<<endl;
    cout << g.numberOfNodes() << endl;

    ogdf::node n = this->g.newNode();

    cout<<"dois"<<endl;
     this->GA.fillColor( n ) = ogdf::Color( "#fff" );//adiciona cor branca ao interior do nó
     this->GA.height(n ) = 20; // set the height to
     this->GA.width( n) = 20; // set the width to

     this->GA.shape(n) = ogdf::shEllipse;

     string s = to_string(n->index());
     char const *pchar = s.c_str(); //use char const* as target type
     this->GA.label( n) = pchar;
     cout<<"tres"<<endl;

     //posições x e y da imagem
     this->GA.x(n) = x;
     this->GA.y(n) = y;

    rerender();
}

void GraphEditor::rerender()
{
    cout<<"rerender"<<endl;
    ogdf::GraphIO::drawSVG( this->GA,"/home/silvana/temp.svg");//gera imagem
    this->svg->openFile(QFile("/home/silvana/temp.svg"));
}
