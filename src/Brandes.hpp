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
#ifndef BRANDES_H
#define BRANDES_H

#include "Graph.hpp"
using namespace std;

 class Brandes
 {
 public:

 	Brandes(int);
 	~Brandes();

 	vector< vector<int> > getShortestPath();


 	void insertPaths(vector<Node> &,int,int,int); 
 	void addNode(vector<Node> &, vector< vector<int> > &, int, int, int);
 	int addPaths(vector<Node> &,vector< vector<int> > &,int,int,int);
    double minimumDistance(vector<double> &, vector<double> &, vector<int> &,int);
    void execute(vector< vector<double> >, int ,vector<Node> &);
    vector<double> betweennessCentrality(vector<Node> &);
 	void printShortestPaths();

private:

	int nNodes;							//número de vértices no grafo
	vector<int> nodeAdjacent;			//vetor de nó adjacente
	vector<int> nodeAuxiliar;			//auxiliar  
	vector< vector<int> > shortestPath;	//armazena os caminhos minimos de uma origem i à um destino j	

};
#endif
