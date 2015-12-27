#include "GraphEditor.hpp"
#include "graphicsview.h"

GraphEditor::GraphEditor(QGraphicsView *q)
{
    this->view = q;
    this->GA = ogdf::GraphAttributes( this->g, ogdf::GraphAttributes::nodeGraphics |
        ogdf::GraphAttributes::edgeGraphics |
        ogdf::GraphAttributes::nodeLabel |
        ogdf::GraphAttributes::nodeStyle |
        ogdf::GraphAttributes::edgeType |
        ogdf::GraphAttributes::edgeArrow |
        ogdf::GraphAttributes::edgeStyle ); // cria atributos para o grafo

    this->view->setBackgroundBrush(QBrush(QColor("#fff")));
    rerender();
    ((GraphicsView *)this->view)->editor = this;
}
GraphEditor::~GraphEditor(){}

void GraphEditor::addNode(int x, int y)
{
    cout << addingNode << endl;
    if (!addingNode) {
        return;
    }

    ogdf::node n = this->g.newNode();

     this->GA.fillColor( n ) = ogdf::Color( "#fff" );//adiciona cor branca ao interior do nó
     this->GA.height(n ) = 20; // set the height to
     this->GA.width( n) = 20; // set the width to

     this->GA.shape(n) = ogdf::shEllipse;

     string s = to_string(n->index());
     char const *pchar = s.c_str(); //use char const* as target type
     this->GA.label( n) = pchar;

     //posições x e y da imagem
     this->GA.x(n) = x;
     this->GA.y(n) = y;

    rerender();
    this->addingNode = false;
}

void GraphEditor::rerender()
{
    QGraphicsScene *scene = new QGraphicsScene();//cria a cena
    scene->setSceneRect(QRect(0,0,view->width(),view->height()));

    QPen pen;
    pen.setColor(QColor("#000000"));

    for(ogdf::node iterateNode = g.firstNode(); iterateNode; iterateNode = iterateNode->succ()){
        double x = GA.x(iterateNode);
        double y = GA.y(iterateNode);

        scene->addEllipse(x,y,20,20, pen, QBrush(QColor("#ffffff")));
    }

    this->view->setScene(scene);
    this->view->show();
}
