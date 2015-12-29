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
#ifndef DIJKSTRA_H
#define DIJKSTRA_H

#include "Node.hpp"

using namespace std;

class Graph
{
public:

	Graph();
	~Graph();

	/**
	 * Setters
	 */
	void setNumberOfNodes(int);
	void setEdge(int,int); 				//para grafo não dirigido
	void setEdgeDirected(int,int);		//para grafo dirigido
	void setMinimumDegree(int);
	void setMaximumDegree(int);
	void setAverageDegree(double);
	void setMaximumAverageDegree(double);//número médio do grau máximo da rede
	void removeEgde(int,int);
	void setWeight(int,int,double);
	void setWeightEdgeDirected(int, int, double);
	void setMinimumDistanceOfNode(int);
	void setDistancePairOfNodes(int,int,double);
	void setEuclideanDistance(int,int,double);
	void setNodesMeasures(vector<Node> &);
	void setRegionOfNode(int, int);		//insere a região que o nó pertence

	/**
	 * Getters
	 */
	int getDegree(int); 		//retorna o grau do nó passado como parâmetro do método
	bool getEdge(int,int); 		//verifica a existência de uma ligação entre dois vértices passados como parâmetros
	int getNumberOfEdges();		//número máximo de arestas do grafo
	int getMinimumDegree();		//número mínimo do grau
	int getMaximumDegree();		//número máximo do grau
	int getNumberOfNodes();		//número de nós do grafo
	int getRegionOfNode(int);
	vector<Node> getNodes();	//retorna um vetor de nós do grafo
	double getAverageDegree();	//retorna grau médio do grafo
	double getMaximumAverageDegree();
    Node & getNodeAtPosition(int);
	int getMinimumDistanceOfNode();
	int getMinimumNumberOfEdges();
	int getMaximumNumberOfEdges();

	void memsetGraph();
	void limitDegree(); 
	void removeNode(int,int);
	void printAdjacents(int);

private:

	int nEdges; 					//número de edges da topologia
	int nNodes;						//número de nós da topologia
	int maxDegree;					//grau máximo da topologia
	int minDegree;					//grau minimo da topologia
	double avgMaximumDegree;		//grau máximo médio previsto para a topologia
	double avgDegree;				//grau médio da topologia
	int minDistance; 				//distancia minima entre um par de vertices/nós
	vector <Node> nodes;			//vetor de informações de um nó do grafo
};

#endif
