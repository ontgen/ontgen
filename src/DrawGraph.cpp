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
#include "graphicsview.h"
#include <QGraphicsTextItem>
#include "editorpathitem.h"
#include <QMessageBox>

DrawGraph::DrawGraph(QGraphicsView *q)
{
    this->view = q;
    this->GA = ogdf::GraphAttributes( this->g, ogdf::GraphAttributes::nodeGraphics |
        ogdf::GraphAttributes::edgeGraphics |
        ogdf::GraphAttributes::nodeLabel |
        ogdf::GraphAttributes::nodeStyle |
        ogdf::GraphAttributes::edgeType |
        ogdf::GraphAttributes::edgeArrow |
        ogdf::GraphAttributes::edgeStyle |
        ogdf::GraphAttributes::edgeDoubleWeight); // cria atributos para o grafo

    this->view->setBackgroundBrush(QBrush(QColor("#fff")));
    rerender();
    this->nSelected = 0;
}

DrawGraph::~DrawGraph(){}

void DrawGraph::addNode(int x, int y)
{
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

void DrawGraph::rerender()
{
    QGraphicsScene *scene = new QGraphicsScene();//cria a cena
    scene->setSceneRect(QRect(0,0,2000,2000));

    if(this->backgroundImage) {
        QGraphicsPixmapItem *img = new QGraphicsPixmapItem();
        scene->setSceneRect(QRect(0,0,this->backgroundImage->width(), this->backgroundImage->height()));

        img->setPixmap(*(this->backgroundImage));
        scene->addItem(img);
    }

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

            for(uint i = 0; i < (uint)polyLine.size() / 3; i++)
            {
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

        EditorPathItem *item = new EditorPathItem();
        item->edgeIndex = iterateEdge->index();
        item->setPath(path);
        item->setPen(pathPen);
        item->setAcceptHoverEvents(true);
        item->setActive(true);

        scene->addItem(item);
        QPointF middle = path.pointAtPercent(path.percentAtLength(path.length() / 2));

        QGraphicsTextItem *label = scene->addText(QString::fromStdString(to_string(GA.doubleWeight(iterateEdge))));
        label->setPos(middle);
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

void DrawGraph::addEdge(int x, int y)
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
                bool edgeAlreadyExists = checkEdgeExists(this->selected->index(), iterateNode->index());

                if(iterateNode->index() != this->selected->index() && edgeAlreadyExists == false)
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

bool DrawGraph::checkEdgeExists(int u,int v)
{
    ogdf::edge e;

    forall_edges(e,g)
    {
        int a = e->adjSource()->theNode()->index(), b = e->adjTarget()->theNode()->index();

        if( ( a == u && b == v ) || ( a == v && b == u ) )
        {
            return true;
        }
    }

    return false;
}

void DrawGraph::saveAsPNG(QString filename)
{
    //Salvar em modo PNG
    ((GraphicsView *)this->view)->scene->clearSelection();
    QRectF before = ((GraphicsView *)this->view)->scene->sceneRect();

    ((GraphicsView *)this->view)->scene->setSceneRect(((GraphicsView *)this->view)->scene->itemsBoundingRect());                          // Re-shrink the scene to it's bounding contents
    QImage image(((GraphicsView *)this->view)->scene->sceneRect().size().toSize(), QImage::Format_ARGB32);  // Create the image with the exact size of the shrunk scene
    image.fill(Qt::transparent);                                              // Start all pixels transparent

    QPainter painter(&image);
    painter.setRenderHint(QPainter::Antialiasing);
    ((GraphicsView *)this->view)->scene->render(&painter);
    image.save(filename);
    ((GraphicsView *)this->view)->scene->setSceneRect(before);
    ((GraphicsView *)this->view)->centerOn(before.center());
}

void DrawGraph::clearGraph()
{
    this->g.clear();
    this->backgroundImage = NULL;//remove plano de fundo

    rerender();
}

void DrawGraph::constructGraph(Graph &graph)
{
    graph.setNumberOfNodes(g.numberOfNodes()); //número de nós

    graph.setMinimumDegree(2);//grau mínimo

    graph.setMaximumDegree(g.numberOfNodes()-1);//grau máximo

    graph.setMinimumDistanceOfNode(0);//distância mínima entre dois nós

    graph.memsetGraph();

    ogdf::edge e;

    forall_edges(e,g)
    {
        int u = e->adjSource()->theNode()->index();
        int v = e->adjTarget()->theNode()->index();

        graph.setEdge(u,v);
        graph.setDistancePairOfNodes(u,v,(double)GA.intWeight(e));
    }

}

//Carrega imagem
void DrawGraph::loadBackgroundImage(QString filename)
{
    QImage image(filename);

    if(image.isNull())
    {
        QMessageBox::information(0,QString("Graph Editor"),QString("Couldn't load image"), QMessageBox::Ok);

        return;
    }

    this->backgroundImage = new QPixmap(filename);

    rerender();
}


void DrawGraph::loadTopology(QString filename)
{

    bool successful = ogdf::GraphIO::readDOT(this->GA,this->g,filename.toStdString());

    if(!successful)
    {
        QMessageBox::information(0,QString("Graph Editor"),QString("Couldn't load file"), QMessageBox::Ok);

    }

    rerender();
}

void DrawGraph::saveAsGML(QString filename)
{
    ogdf::GraphIO::writeDOT(this->GA,filename.toStdString());
}

void DrawGraph::setOGDFGraph(Graph &graph, Plane &plane, string dateTime, int index,int simulation)
{
    clearGraph();
    constructGraphOGDF(graph);//constrói grafo no formato da biblioteca

    ogdf::node v;

    int i = 0, f = 50, fn = 20, fontSize = 12;

    if(plane.getSqrtArea() > 30)
    {
        f = 18;
        fn = 50;
        fontSize = 24;
    }

    forall_nodes( v, this->g ){ // itera sobre todos os nós do grafo
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
    forall_edges(e ,this->g) // adiciona cor as arestas
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
    temp.append(".png");
    this->file = temp.toStdString();
}


ogdf::Graph DrawGraph::constructGraphOGDF(Graph &graph)
{
    ogdf::Graph _g;

    //cria todos os nós do grafo
    for(int i = 0; i < graph.getNumberOfNodes(); i++)
    {
        nodes.push_back(g.newNode());
    }

    vector<Node> n = graph.getNodes();
    mAdjacents = vector<vector<double>> (graph.getNumberOfNodes(),vector<double>(graph.getNumberOfNodes(),0.0f));

    //Constrói matriz de adjacencias
    for (int i = 0; i < graph.getNumberOfNodes(); i++)
    {
        vector<int> adjacents = n[i].getAdjacentsNodes();

        for (int j = 0; j < (int)adjacents.size(); j++)
        {
            mAdjacents[i][ adjacents[j] ] = n[i].getWeightEdge(j);
        }
    }

    //faz as ligações entre os nós
    for (int i = 0; i < graph.getNumberOfNodes()-1; i++)
    {
        for (int j = i+1; j < graph.getNumberOfNodes(); j++)
        {
            if(mAdjacents[i][j] > 0.0f)
            {
                ogdf::edge e = g.newEdge(nodes[i], nodes[j]);
                GA.doubleWeight(e) = mAdjacents[i][j];
            }
        }
    }

    return _g;
}

/**
 * Desenha o grafo para arquivo SVG
 */
void DrawGraph::setSVGImage(Graph &graph, Plane &plane,string dateTime, int index,int simulation)
{

    ogdf::Graph g = constructGraphOGDF(graph);//constrói grafo no formato da biblioteca

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

string DrawGraph::getFile()
{
    return this->file;
}

