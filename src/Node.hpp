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
#ifndef NODE_H
#define NODE_H
/**
 * Definições de função-membro que aparecem em Nodes.cpp
 */
#include <iostream>
#include <deque>
#include <iterator>
#include <algorithm> //reverse vector
#include <limits> //limites numéricos
#include <stdexcept> // for std::runtime_error
#include <vector>
#include <random>
#include <fstream>//manipulação de arquivos


#include <ogdf/basic/graphics.h>
#include <ogdf/basic/Graph.h>
#include <ogdf/basic/graph_generators.h>
#include <ogdf/layered/DfsAcyclicSubgraph.h>
#include <ogdf/fileformats/GraphIO.h>
#include <ogdf/layered/SugiyamaLayout.h>
#include <ogdf/layered/OptimalRanking.h>
#include <ogdf/layered/MedianHeuristic.h>
#include <ogdf/layered/OptimalHierarchyLayout.h>

using namespace std;

class Node
{
public:

    Node();
    ~Node();

    /**
     * Setters
     */
    void setEdgeNode(int);						//faz uma ligação
    void setDegree();							//aumenta o grau do nó
    void setNewDistance(int, double);				//atualiza peso de uma ligação
    void setDistanceNode(double);				//atribui distância entre dois nós
    void setDegreeCentrality(int);				//atribui o valor da centralidade de grau
    void setBetweennessCentrality(double);			//atribui o valor da centralidade de intermediação
    void setClosenessCentrality(double);		//atribui o valor da centralidade de proximidade
    void setEfficientCentrality(double);		//atribui o valor da centralidade de eficiência
    void setRelativeDegreeCentrality(double);	//atribui o valor da centralidade relativa de grau
    void setEuclideanDistance(double);
    void setRegion(int);                        //armazena a região que o nó pertence
    /**
     * Getters
     */
    int getDegree(); 					//retorna o grau do nó passado como parâmetro do método
    bool getEdgeNode(int); 				//verifica a existência de uma ligação entre dois vértices passados como parâmetros
    int getNumberOfPaths();				//número de caminhos mínimos
    int getNumberOfNodes();				//retorna o número de caminhos mínimos
    int getRegionOfNode();
    int getAdjacentNode(int);			//retorna nó adjacente na posição passada como parâmetro
    double getWeightEdge(int);			//retorna o peso da ligação
    void incrementPaths(int);			//incrementa o número de caminhos minimos
    int getDegreeCentrality();		    //centralidade do grau
    double getBetweennessCentrality();		//centralidade de intermediação
    double getClosenessCentrality();	//centralidade de proximidade
    double getEfficientCentrality();	//centralidade de eficiência
    vector<int> &getAdjacentsNodes();	//retorna vetor de nós adjacentes
    int getNumberOfNodesFromPath(int);	//número de nodos em um caminho minimo
    vector<double>  getEuclideanDistance();//retorna um vetor com as distâncias euclidianas de cada ligação

    void addNodePath(int);					//adiciona nodo ao caminho
    void removeNode(int);					//remove nodo da lista de adjacências
    int returnNode(int,int);				//retorna nodo das posições indicadas localizada na matriz de caminhos mínimos
    vector< vector<int> > returnPaths();	//retorna caminhos mínimos
    void printAdjacents();

private:

    int degree; 					     //número de links da topologia
    vector<int> adjacents;  		     //vetor de adjacências de um nó
    vector<double> distance;			 //vetor de distância das ligações entre nós adjacentes

    /**
     * Variáveis utilizadas para medidas de centralidade
     */
    int index;						//nodo origem
    int numberOfPaths;				//número de caminhos mínimos
    int degreeCentrality;			//centralidade do grau
    double relativeDegreeCentrality;//centralidade relativa de grau
    double betweenCentrality;		//centralidade de intermediação do nó
    double closenessCentrality;		//centralidade de proximidade
    double efficientCentrality;		//centralidade de eficiência
    vector< vector<int> > paths;	//matriz contendo todos caminhos minimos
    int region;                     //região a qual o nó pertence
};

#endif
