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
#include "Graph.hpp" /*Inclui  a definição da classe Graph*/

using namespace std;

Graph::Graph() {

	setNumberOfNodes(0);					//número de nós
	setAverageDegree(0);		//grau máximo da topologia
	setMinimumDegree(0);		//grau minimo da topologia
	setMaximumDegree(0.0);		//grau médio da topologia
	setMinimumDistanceOfNode(0);//distância mínima entre um par de nodos
	
	this->nEdges = 0;
}

Graph::~Graph() {}


void Graph::setNodesMeasures(vector<Node> &newNodes)
{
	this->nodes = newNodes;
}

void Graph::setEdge(int u,int v)
{
	if (getDegree(u) < this->maxDegree  && getDegree(v) < this->maxDegree && u != v)
	{
		this->nodes[v].setEdgeNode(u);
		this->nodes[u].setEdgeNode(v);

		this->nEdges++;
	}
}

/**
 * Atribui distancia euclidiana entre dois nós
 */
void Graph::setEuclideanDistance(int u,int v,double euclidean)
{
	nodes[u].setEuclideanDistance(euclidean);
	nodes[v].setEuclideanDistance(euclidean);
}

/**
 * Atribui a distancia eucliana 
 */
void Graph::setDistancePairOfNodes(int u,int v,double distance)
{
	nodes[u].setDistanceNode(distance);
	nodes[v].setDistanceNode(distance);
}


void Graph::setEdgeDirected(int u,int v)
{
	if (getDegree(u) < this->maxDegree  && getDegree(v) < this->maxDegree && u != v)
	{
		this->nodes[u].setEdgeNode(v);

		this->nEdges++;
	}
}

void Graph::setNumberOfNodes(int n)
{
	this->nNodes = n;
	memsetGraph();
}


void Graph::setMaximumAverageDegree(double avg)
{
	this->avgMaximumDegree = avg;
}

void Graph::setAverageDegree(double avg) 
{
	this->avgDegree = avg;
	this->nEdges = 0;
}

void Graph::setMinimumDegree(int min)
{
	this->minDegree = min;
}

void Graph::setMaximumDegree(int max)
{
	this->maxDegree = max;
}

void Graph::setMinimumDistanceOfNode(int min)
{
	this->minDistance = min;
}

void Graph::setWeightEdgeDirected(int u,int v, double value)
{
	this->nodes[u].setNewDistance(v,value);//peso 1 caso ocorra ligação
}

void Graph::setWeight(int u,int v, double value)
{
	this->nodes[u].setNewDistance(v,value);//peso 1 caso ocorra ligação 
	this->nodes[v].setNewDistance(u,value);//peso 1 caso ocorra ligação 
}


void Graph::setRegionOfNode(int region, int node)
{
	this->nodes[node].setRegion(region);
}

vector<Node> & Graph::getNodes()
{
	return this->nodes;
}

/**
 * Verifica a existência de ligações
 */
bool Graph::getEdge(int u,int v)
{
	if(u == v)
	{
		return true;
	}

	return this->nodes[u].getEdgeNode(v);
}

int Graph::getDegree(int node)
{
	int degree = this->nodes[node].getDegree();

	return degree;
}


int Graph::getNumberOfEdges()
{
	return this->nEdges;
}

int Graph::getNumberOfNodes()
{
	return this->nNodes;
}

int Graph::getMaximumNumberOfEdges() 
{
	int maximumDegree = ( ( this->maxDegree * this->nNodes )/ 2 ); //máximo de ligações que terá a topologia

	try 
	{
        limitDegree();
	}
	catch(const char *error) 
	{
		cout<<error<<endl;
	}

	return maximumDegree;
}

int Graph::getMinimumNumberOfEdges()
{

	return (this->minDegree*this->nNodes)/2;
}

double Graph::getAverageDegree() 
{
	int sum = 0;

	for (int u = 0; u < getNumberOfNodes(); u++)
	{
		sum = sum + nodes[u].getDegree();
	}

	return ( sum/getNumberOfNodes() );
}

double Graph::getMaximumAverageDegree()
{
	return this->avgMaximumDegree;
}

int Graph::getMinimumDegree()
{
	return this->minDegree;
}

int Graph::getMaximumDegree()
{
	return this->maxDegree;
}

int Graph::getMinimumDistanceOfNode()
{
	return this->minDistance;
}

Node & Graph::getNodeAtPosition(int index)
{
    Node &node = nodes.at(index);
    return node;
}

int Graph::getRegionOfNode(int u)
{
	return this->nodes[u].getRegionOfNode();
}

/**
 * Inicializa grafo de ligações com todas as posições em 0
 */
void Graph::memsetGraph() 
{
	this->nodes = vector<Node> (this->nNodes);
}


/**
 * Verifica se o número máximo do grau 
 * no pior caso onde todos os grafos 
 * possuem o mesmo grau ele não ultrapassa o limite
 * Se passar lança uma exception.
 */
void Graph::limitDegree() 
{
	if ( this->maxDegree >= this->nNodes  )	
	{
		// throw "2 ≤ Maximum Degree ≤ [N*(N-1)]/2";
		throw "limitDegree";
	}
}

void Graph::removeNode(int u,int v)
{
	this->nodes[u].removeNode(v);
}

void Graph::printAdjacents(int node)
{
	this->nodes[node].printAdjacents();
}
