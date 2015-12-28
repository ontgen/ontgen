#include "GraphEditor.hpp"
#include "graphicsview.h"
#include <QGraphicsTextItem>
#include "editorpathitem.h"

GraphEditor::GraphEditor(QGraphicsView *q)
{
    this->view = q;
    this->GA = ogdf::GraphAttributes( this->g, ogdf::GraphAttributes::nodeGraphics |
        ogdf::GraphAttributes::edgeGraphics |
        ogdf::GraphAttributes::nodeLabel |
        ogdf::GraphAttributes::nodeStyle |
        ogdf::GraphAttributes::edgeType |
        ogdf::GraphAttributes::edgeArrow |
        ogdf::GraphAttributes::edgeStyle |
        ogdf::GraphAttributes::edgeDoubleWeight ); // cria atributos para o grafo

    this->view->setBackgroundBrush(QBrush(QColor("#fff")));
    rerender();
    ((GraphicsView *)this->view)->editor = this;
    this->nSelected = 0;
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


    for (ogdf::edge iterateEdge = g.firstEdge(); iterateEdge; iterateEdge = iterateEdge->succ())
    {
        auto polyLine = GA.bends(iterateEdge);
        QPainterPath path;

        if ((int)polyLine.size() == 0)
        {
            path.moveTo(GA.x(iterateEdge->adjSource()->theNode())+15, GA.y(iterateEdge->adjSource()->theNode())+10);
            path.lineTo(GA.x(iterateEdge->adjTarget()->theNode())+15, GA.y(iterateEdge->adjTarget()->theNode())+10);
        }
        else
        {
            auto pointIterator = polyLine.begin();

            path.moveTo((*pointIterator).m_x, (*pointIterator).m_y);

            if ((int)polyLine.size() % 3 != 0) {
                ++pointIterator;
            }

            for(uint i = 0; i < polyLine.size() / 3; i++) {
                auto point1 = *(pointIterator++);
                auto point2 = *(pointIterator++);
                auto point3 = *(pointIterator++);

                path.cubicTo(point1.m_x, point1.m_y, point2.m_x, point2.m_y, point3.m_x, point3.m_y);
            }

            if ( ( (int)polyLine.size() % 3 ) == 2)
            {
                path.lineTo((*pointIterator).m_x, (*pointIterator).m_y);
                ++pointIterator;
            }
        }

        QPen pathPen;
        pathPen.setWidth(2);
        pathPen.setColor(QColor("#333"));

        QGraphicsPathItem *s = scene->addPath(path, pathPen);
        ((EditorPathItem *)s)->edgeIndex = iterateEdge->index();
        QPointF middle = path.pointAtPercent(path.percentAtLength(path.length() / 2));

        QGraphicsTextItem *label = scene->addText(QString::fromStdString(to_string(GA.doubleWeight(iterateEdge))));
        label->setPos(middle);

        s->setAcceptHoverEvents(true);
        s->setActive(true);
        s->setFlag(QGraphicsItem::ItemIsSelectable, true);
    }

    for(ogdf::node iterateNode = g.firstNode(); iterateNode; iterateNode = iterateNode->succ()){

        double x = GA.x(iterateNode);
        double y = GA.y(iterateNode);

        QPen pathPen;
        pathPen.setWidth(2);
        pathPen.setColor(QColor("#333"));

        QGraphicsEllipseItem *s = scene->addEllipse(x,y,30,30, pathPen, QBrush(QColor("#ffffff")));
        s->setCursor(Qt::PointingHandCursor);
        s->setAcceptHoverEvents(true);
        s->setActive(true);
        s->setFlag(QGraphicsItem::ItemIsSelectable, true);

        //configurações da fonte
        string in = to_string(iterateNode->index());
        QGraphicsTextItem *text =  scene->addText(QString::fromStdString(in));
        text->setCursor(Qt::PointingHandCursor);
        text->setAcceptHoverEvents(true);
        text->setPos(x+(iterateNode->index() < 10 ? 6 : 2),y);//posição do indice do nó
    }

    this->view->setScene(scene);
    ((GraphicsView *)this->view)->scene = scene;
    this->view->show();
}

void GraphEditor::addEdge(int x, int y)
{
    this->nSelected++;

    for(ogdf::node iterateNode = g.firstNode(); iterateNode; iterateNode = iterateNode->succ()){
        if(GA.x(iterateNode) == x + 1 && GA.y(iterateNode) == y + 1)
        {
            if(this->nSelected <= 1)
            {
                this->selected = iterateNode;
            }
            else
            {
                if(iterateNode->index() != this->selected->index())
                {
                    this->g.newEdge(this->selected, iterateNode);

                    rerender();

                }

                this->nSelected = 0;
            }

            break;
        }
    }
}
