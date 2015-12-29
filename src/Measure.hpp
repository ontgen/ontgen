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
#include "Brandes.hpp"

 using namespace std;

 class Measure
 {
 public:

 	Measure();
 	~Measure();


 	double getDegreeCentrality(int);
	double getBetweennessCentrality(int);
	double getClosenessCentrality(int);
	double getEfficientCentrality(int);


	void setDegreeCentrality(int,double);
	void setBetweennessCentrality(int,double);
	void setClosenessCentrality(int,double);
	void setEfficientCentrality(int, double);


	void degreeCentrality(vector<Node> &);
	void betweennessCentrality(vector<Node> &);
	void closenessCentrality(vector<Node> &, vector<vector <int> >);
	void efficientCentrality(vector<Node> &, vector<vector <int> >);	

 	double geodesic(vector<Node>,int);
 	int nodeSearch(vector< vector<int> > &, int, int);
 	void initialize(Graph &,int,bool,bool,bool,bool);
 	int pathsSearch(vector<Node>, int, int,vector< vector<int>> &);

private:

	vector<Node> nodes;
	int numberOfNodes;
	int centralNode;	//nó central em relação a centralidade de intermediação
 };