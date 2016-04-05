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
#include "Measure.hpp"
#include "Dijkstra.hpp"

Measure::Measure(){}


Measure::~Measure(){}

void Measure::initialize(Graph &graph,int n, bool bc, bool cc, bool dc, bool ec)
{

    this->numberOfNodes = n;

    ofstream out;

    out.open("out");

    if (bc || cc || ec)
    {

        /**
         * Obtendo medidas de centralidade
         */

        if (bc)
        {
            /**
             * Encontra todos os caminhos mínimos entre pares de nós do grafo
             */
               betweennessCentrality(graph);
        }

        if (cc)
        { 
            closenessCentrality(graph); //centralidade de proximidade
        }

        if (ec)
        {
            efficientCentrality(graph);  //centralidade de eficiência
        }

    }

    if (dc)
    {  
        degreeCentrality(graph);  //centralidade de grau
    }

    vector<Node> nodes = graph.getNodes();
    graph.setNodesMeasures(nodes);

}


void Measure::degreeCentrality(Graph &graph)
{

    int maximumDegree = -1;
    vector<Node> nodes = graph.getNodes();

    for (int i = 0; i < this->numberOfNodes; i++)
    {
        int degree = nodes[i].getDegree();

        nodes[i].setDegreeCentrality(degree);

        double relative = ( (double)degree / (double)(this->numberOfNodes-1) );

        nodes[i].setRelativeDegreeCentrality(relative);

        if (maximumDegree < degree)
        {
            maximumDegree = degree;
        }
    }

    graph.setNodesMeasures(nodes);
}


void Measure::closenessCentrality(Graph &graph)
{
    vector<Node> nodes = graph.getNodes();

    for (int i = 0; i < this->numberOfNodes;i++)
    {
        double sum = 0;
        /**
         * Guarda a soma das distância de i para
         * os demais nós do grafo.
         */
        for (int j = 0; j < this->numberOfNodes; j++)
        {
            Dijkstra d;
            d.execute(graph,i,j);
            vector<int> path = d.shortestPath(j);
            sum += (int)path.size();
        }

        double cc = (double)( 1.0f / sum );

        nodes[i].setClosenessCentrality(cc);
    }

    graph.setNodesMeasures(nodes);
}


void Measure::efficientCentrality(Graph &graph)
{
    vector<Node> nodes = graph.getNodes();

    for (int i = 0; i < this->numberOfNodes;i++)
    {
        int longestPath = 0;

        /**
         * Guarda a soma das distância de i para
         * os demais nós do grafo.
         */
        for (int j = 0; j < this->numberOfNodes; j++)
        {
            Dijkstra d;
            d.execute(graph,i,j);
            vector<int> path = d.shortestPath(j);

            if ((int)path.size() > longestPath)
            {
                longestPath = (int)path.size();

                double auxiliar = (double)path.size();

                double ef = (double) ( 1 / auxiliar ) ;

                nodes[i].setEfficientCentrality(ef);
            }
        }

    }

    graph.setNodesMeasures(nodes);
}

void Measure::betweennessCentrality(Graph &graph)
{
    Brandes brandes (this->numberOfNodes);

    vector<Node> nodes = graph.getNodes();
    vector<double> cb = brandes.betweennessCentrality(nodes);

    for(int w = 0; w < this->numberOfNodes; w++)
    {
        nodes[w].setBetweennessCentrality(cb[w]);
    }

    graph.setNodesMeasures(nodes);
}



