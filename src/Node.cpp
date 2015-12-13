/**
 * Definição de características de um nó
 */

#include "Node.hpp" /*Inclui  a definição da classe Graph*/

using namespace std;

Node::Node()
{
	this->degree = 0;

	this->numberOfPaths = 0;
}

Node::~Node(){}

void Node::setEdgeNode(int target){

	adjacents.push_back(target);
	setDegree();
}

void Node::setDegree(){

	this->degree++;
}

void Node::setDistanceNode(double d){

	this->distance.push_back(d);
}

void Node::setNewDistance(int node, double value)
{
	vector<int>::iterator it;

	it = find(this->adjacents.begin(),this->adjacents.end(),node);

	if (it  != this->adjacents.end())
	{
        this->distance[*it] = value;
	}
}

void Node::setEuclideanDistance(double distance)
{
	this->distanceEuclidean.push_back(distance);
}

void Node::setRegion(int r)
{
	this->region = r;
}

void Node::incrementPaths(int n)
{
	this->numberOfPaths  = this->numberOfPaths + n;
}


void Node::setDegreeCentrality(int value)
{
	this->degreeCentrality = value;
}		

void Node::setBetweennessCentrality(double value)
{

	this->betweenCentrality = value;
}

void Node::setClosenessCentrality(double value)
{
	this->closenessCentrality = value;
}


void Node::setEfficientCentrality(double value)
{
	this->efficientCentrality = value;
}


void Node::setRelativeDegreeCentrality(double value)
{
	this->relativeDegreeCentrality = value;
}


int Node::getDegreeCentrality()
{
	return this->degreeCentrality;
}


double Node::getBetweennessCentrality()
{
	return this->betweenCentrality;
}


double Node::getClosenessCentrality()
{
	return this->closenessCentrality;
}


double Node::getEfficientCentrality()
{
	return this->efficientCentrality;
}

int Node::getDegree()
{
	return this->degree;
}

vector<double> Node::getEuclideanDistance()
{
	return this->distanceEuclidean;
}

vector<int> Node::getAdjacentsNodes() 
{
	return this->adjacents;
}	

int Node::getRegionOfNode()
{
	return this->region;
}

/**
 * Verifica se existe ligação
 * Se sim retorna verdadeiro
 * Do contrário falso
 */
bool Node::getEdgeNode(int target){

	vector<int>::iterator it;
	it = find(this->adjacents.begin(),this->adjacents.end(),target);

	if (it  != this->adjacents.end())
	{
		return true;
	}

	return false;
}


int Node::getNumberOfPaths() 
{
	return this->numberOfPaths;
}

int Node::getNumberOfNodesFromPath(int index) 	
{
	return this->paths[index].size();
}

double Node::getWeightEdge(int position)
{
	return this->distance[position];
}	

int Node::getAdjacentNode(int position)
{
	return this->adjacents[position];
}

void Node::addNodePath(int newNode) 
{
     unsigned int position = this->numberOfPaths;

	if (paths.size() < position)
	{
		vector<int> i;
		paths.push_back(i);
	}

	this->paths[position-1].push_back(newNode);
}

int Node::returnNode(int nPath, int index)
{
	return this->paths[nPath][index];
}


vector<vector<int>> Node::returnPaths() 
{
	return this->paths;
}

void Node::removeNode(int node)
{

    for (int i = 0; i < this->degree; i++)
	{
	
		if (this->adjacents[i] == node)
		{
			this->adjacents.erase(this->adjacents.begin()+i);
			this->degree--;
			break;	
		}	
	}
}

void Node::printAdjacents()
{
	for (unsigned int i = 0; i < this->adjacents.size(); i++)
	{
		cout<<" "<<adjacents[i];
	}
	cout<<endl;
}
