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

Dijkstra::Dijkstra(){}

Dijkstra::~Dijkstra(){}

/**
 * Insere caminho mínimo ao inverso
 */
vector<int> Dijkstra::shortestPath(int target)
{
    vector<int> path;

    int u = target;

    path.push_back( target );

    while( this->parent[u] != -1 )
    {
        path.push_back( this->parent[u] );
        u = this->parent[u];
    }

    reverse( path.begin(),path.end() );//inverte caminho

     for (int i = 0; i < (int)path.size(); i++)
     {
         cout<<" "<<path[i];
     }
     cout<<endl;
    return path;
}

/**
 * Implementação baseada no livro The Algorithm Design Manual -- Skiena
 */
int Dijkstra::execute( Graph graph, int source, int target)
{
    // cout<<"Dijkstra\n";
    Node p;                     //vetor temporário
    vector<bool> inTree;        //O nó já esta na árvore?
    vector<double> distance;    //armazena distância para source
    int v;                      //nó atual
    int w;                      //candidato a próximo nó
    int n;                      //número de nós adjacentes
    double weight;              //peso da aresta
    double dist;                //melhor distância atual para o nó de partida


    inTree = vector<bool> ( graph.getNumberOfNodes(), false);
    distance = vector<double> ( graph.getNumberOfNodes(), std::numeric_limits<double>::max() );
    this->parent = vector<int> ( graph.getNumberOfNodes(), -1);

    v = source;
    distance[v] = 0.0f;

    while( inTree[target] == false && inTree[v] == false)
    {
        inTree[v] = true;

        p = graph.getNodeAtPosition(v);

        n = p.getDegree();

        if (n == 0)
        {
            return -std::numeric_limits<double>::max() ;
        }

        int iterator = 0;

        while( iterator < n )
        {
            w = p.getAdjacentNode(iterator);
            weight = p.getWeightEdge(iterator); //obtêm peso da aresta

            /**
             * Verificação de caminho
             */
            if ( ( distance[w] > ( distance[v] + weight ) ) && weight > -1) 
            {
                distance[w] = distance[v] + weight;
                this->parent[w] = v;
            }

            iterator++;
        }

        v = 0;

        dist = std::numeric_limits<double>::max();

        for (int i = 0; i < graph.getNumberOfNodes(); i++)
        {
            if ( ( inTree[i] == false ) && ( dist > distance[i] ) )
            {
                dist = distance[i];
                v = i;
            }
        }

        if (v == target)
        {
            break;
        }
    }

    return distance[target];//retorna distância
}
