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
#include "Dijkstra.hpp"
#include "tree.hh"
#include "tree_util.hh"

class Suurballe
{
public:

	Suurballe();
	~Suurballe();

	bool execute(Graph &);								//encontra caminhos disjuntos 
	bool findPath(Graph &, vector<Node>, int, int);
	tree<int> makeTree(Graph &, vector<int>, int);						//insere nós em uma árvore binária
	bool makeDisjointPaths(vector<int>, vector<int>, Graph &);
	void changeEdgesWeights(Graph &, tree<int>, vector<int>);			//atualiza peso dos nós da árvore
	void makePathVector(vector<int>,vector<int> &,vector<int> &inPath);
    void insertSubtree(Graph &, tree<int> &, typename tree<int>::iterator &, vector<int> &, vector<int> &, int);
	void updateEdgesWeight(const tree<int>& , typename tree<int>::iterator,vector<int>, Graph &, int); 
	vector<int> disjointPath(int);
	bool makeSubgraphDisjointPaths(Graph &, int, int);
	vector<double> averageHops();										//calculo o número médio de saltos para o caminho principal e o de backup
	void discardCommonEdge(vector<int> &, vector<int> &, int, int);									//calculo o número médio de saltos para o caminho principal e o de backup

private:

	int numberOfNodes;
	vector<int> visited;
	vector<int> parent;
	int numberOfPaths;
	vector<int> nodeInTree;
	vector< vector<int> > path; 				//armazena caminho mínimo 
	vector< double> hopWorking;		//número de saltos(i,j) do principal
	vector< double> hopBackup;			//número de saltos(i,j) de backup
	vector< vector<int> > distance;				//armazena distância entre um par de nós d(u,v)
	vector< vector<int> > treePath;				//matriz que armazena enlaces da árvore de u a v
};
