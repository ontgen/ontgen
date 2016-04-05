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
#include <stack>
#include <queue>

Brandes::Brandes(int n)
{
  this->nNodes = n;

  this->shortestPath = vector<vector<int>> (this->nNodes,vector<int>(this->nNodes,0));
  this->nodeAuxiliar = vector<int> (this->nNodes,0);
}

Brandes::~Brandes(){}


vector< vector<int> > Brandes::getShortestPath()
{
  return this->shortestPath;
}

double Brandes::minimumDistance(vector<double> &distance, vector<double> &sptSet, vector<int> &array,int source)
{

  double min = std::numeric_limits<double>::max(),count = 0;
  int min_index = -1;

  for (int v = 0; v < this->nNodes; v++)
  {

    if (sptSet[v] == 0 && distance[v] < min)
    {
      if (distance[v] == 0 && v != source)
      {
        distance[v] = std::numeric_limits<double>::max();
        continue;
      }
      else
      {
        min = distance[v];
        min_index = v;
      }
    }
  }

  for (int v = 0; v < this->nNodes; v++) {

    if (sptSet[v] == 0 && distance[v] <= min)
    {
      min = distance[v];
      nodeAdjacent[count] = v;
      count++;
    }
  }

  for (int v = 0; v < count; v++)
  {
    array.push_back( min_index );
  }

  return count;
}


/**
 * Adiciona os nodes do caminho na estrutura node
 */
void Brandes::addNode(vector<Node> &nodes, vector<vector<int>> & path,int numNodes, int nPaths,int source)
{

  for (int i = 0; i < nPaths; i++)
  {
    if (path[i][numNodes-1] > -1)
    {
      nodes[source].incrementPaths(1);//incrementa o número de caminhos mínimos
    }

    for (int j = 0; j <= numNodes; j++)
    {
      if (path[i][numNodes-j] > -1)
      {

        int temp = path[ i ][ numNodes-j ]+0;

        nodes[source].addNodePath( temp );//adiciona nodo ao caminho
      }
    }
  }
}

/**
 * Adiciona todos os caminhos minimos distintos na matriz caminho
 * @param graph com uma matriz adjacente, e o source
 */
int Brandes::addPaths(vector<Node> & nodes,vector<vector<int>> &path,int adjacent,int source,int target)
{
  // cout<<"source "<<source<<" target "<<target<<endl;
  int i = 0, j = 0,nodesPath = 0,minimum = 0,temp = 1;
  int k = nodes[source].getNumberOfPaths();

  for(temp = 1; temp  < nodes[source].getNumberOfPaths(); temp++)
  {

    int count = nodes[source].getNumberOfNodesFromPath(k-temp); //número de nodos no caminho
    int auxiliar = count-1;

    if (nodes[source].returnNode(k-temp,auxiliar) == adjacent && k-temp >= 0)
    {
      while( temp < nodes[source].getNumberOfPaths() && k-temp >= 0)
      {

        nodesPath =  nodes[source].getNumberOfNodesFromPath(k-temp);
        minimum = shortestPath[source][target];

        if (nodesPath < minimum-1)
        {
          break;
        }

        if ( nodes[source].returnNode(k-temp,auxiliar) == adjacent && nodesPath == minimum-1)
        {
          path[i][j++] = target;

          // cout<<" "<<path.size()<<endl;
          for (int n = auxiliar; n >= 0; n--)
          {
            path[i][j++] = nodes[source].returnNode(k-temp,n);
            // cout<<" "<<path[i][j-1];
          }
          // cout<<endl;

          i++;
          j = 0;

          if (i >= (int)path.size())
          {

            path.push_back(vector<int>((int)nodes.size()));
          }

        }

        temp++;
      }

      break;
    }
  }

  return i;
}

/**
 * Insere caminhos mínimos encontrados
 */
void Brandes::insertPaths(vector<Node> &nodes,int source,int target,int adjacent)
{
// printf("(%d , %d)\n",source,target );
  vector< vector<int> > path = vector< vector<int> > ( this->nNodes, vector<int>(this->nNodes,-1) );

  if (this->shortestPath[source][target] >= 3)
  {
    int nPaths = addPaths(nodes,path,adjacent,source,target);

    addNode(nodes,path,this->shortestPath[source][target],nPaths,source);
  }
  else
  {
    if (this->shortestPath[source][target] == 2)
    {
      path[0][0] = target+0;
      path[0][1] = adjacent+0;

      addNode(nodes,path,2,this->shortestPath[source][target],source);
    }
    else
    {
      path[0][0] = target+0;

      addNode(nodes,path,1,this->shortestPath[source][target],source);
    }
  }

  path.clear();
}



/**
 * Calcula o menor caminho, de um node até os outros
 * graph com uma matriz adjacente, e o source
 */
void Brandes::execute(vector< vector<double> > graph, int source,vector<Node> &nodes)
{
  vector<double> distance = vector<double> (this->nNodes,std::numeric_limits<double>::max());

  vector<double> sptSet = vector<double> (this->nNodes,0);

  vector< vector<int> > edge = vector< vector<int> > ( this->nNodes, vector<int>(2,-1) );

  distance[source] = 0.0f;

  int k = 0,v = 0,i = 0,t = 0,temp = 0,aux = 0;
  
  for (int count = 0; count < this->nNodes; count++)
  {
    nodeAdjacent = vector<int> (this->nNodes,-1);

    vector<int> u;

    double nMin = minimumDistance(distance,sptSet,u,source);

    for (k = 0; k < nMin; k++)
    {
      sptSet[ u[k] ] = 1.0f;
    }

    int increment = 0;
    int currentTarget = nodeAdjacent[0],lastTarget = nodeAdjacent[0];

    for (k = 0; k < nMin; k++)
    {
      aux = -1;

      for (v = 0; v < this->nNodes; v++) {

        if (v == source)
        {
          continue;
        }

        if (!sptSet[v] && graph[ nodeAdjacent[k] ][ v ] && distance[ nodeAdjacent[k] ] != std::numeric_limits<int>::max() && distance[ nodeAdjacent[k] ] + graph[ nodeAdjacent[k] ][v] <= distance[ v ] && currentTarget != v )
        {
            cout<<" "<<v<<" "<<nodeAdjacent[k]<<endl;
          currentTarget = v;

          aux = distance[v];

          temp = distance[ nodeAdjacent[k] ];

          distance[v] =  temp + graph[ nodeAdjacent[k] ][v];

          this->shortestPath[source][v] = this->shortestPath[v][source] = distance[v];

          if ( shortestPath[source][v] > 0)
          {
            int check = 0;

            for (t = 0; t < this->nNodes; t++)
            {
              if (edge[t][0] == v && edge[t][1] == nodeAdjacent[k])
              {
                check = 1;
                break;
              }
            }

            if (check == 0)
            {
              insertPaths(nodes,source,v,nodeAdjacent[k]);

              edge[increment][0] = v;
              edge[increment][1] = nodeAdjacent[k];
                
                increment++;
            }
          }

          nodeAuxiliar[v] = currentTarget;
        }
      }

      if (currentTarget != lastTarget)
      {
        distance[currentTarget] = aux;
        lastTarget = currentTarget;
      }

    }

    for (i = 0; i < this->nNodes; i++)
    {
      if (nodeAuxiliar[i] > -1)
      {
        distance[ nodeAuxiliar[i] ] = this->shortestPath[source][ nodeAuxiliar[i] ];
      }
    }

    for (v = 0; v < this->nNodes; v++)
    {
      nodeAuxiliar[v] = nodeAdjacent[v];
      nodeAdjacent[v] = -1;
    }

    u.clear();
    nodeAdjacent.clear();
  }

  distance.clear();
  sptSet.clear();
}

/**
 * Imprime todos os caminhos mínimos
 */
void Brandes::printShortestPaths()
{
  int i = 0, j = 0;

  for (i = 0; i < (int)this->shortestPath.size(); i++)
  {
    for (j = 0; j < (int)this->shortestPath[i].size(); j++)
    {
      cout<<this->shortestPath[i][j]<<" ";
    }

    cout<<endl;
  }

  cout<<endl;
}


vector<double> Brandes::betweennessCentrality(vector<Node> &nodes)
{
    int n = (int)nodes.size();

    vector<double> cb = vector<double> (n,0.0f);

   for(int source = 0; source < n; source++)
   {
        stack<int> s;
        vector<double> sigma = vector<double> (n,0.0f);
        vector<double> dist = vector<double> (n,-1.0f);
        vector< pair<int,int> > q;
        vector< vector<int> > p = vector< vector<int> > (n);

        sigma[source] = 1.0f;
        dist[source] = 0.0f;

        pair<int,int> t(source,0);
        q.push_back(t);
        std::push_heap(q.begin(), q.end());

        while(!q.empty())
        {
            pair<int, int> u_pair = q.front();

            int u = u_pair.first;
            pop_heap(q.begin(), q.end());
            q.pop_back();

            s.push(u);

            vector<int> adj = nodes[u].getAdjacentsNodes();
            double du = dist[u];

            for(int i = 0; i < (int)adj.size(); i++)
            {
                 int v = adj[i];
                 double du_we = du + nodes[u].getWeightEdge(i);

                 if(sigma[v] == 0)
                 {
                     pair<int,int> t(v,du_we);
                     q.push_back(t);

                     push_heap(q.begin(), q.end());
                 }
                 else
                 {
                    if (du_we < dist[v])
                    {
                        int qn = (int)q.size();

                        for(int j = 0; j < qn; j++)
                        {
                            if(q[j].first == v)
                            {
                                q[j].second = du_we;
                                make_heap(q.begin(), q.end());
                            }
                        }

                        p[v].clear();
                        sigma[v] = 0;
                    }
                    else
                     if (du_we > dist[v])
                       continue;                 // do not increase number of shortest paths

                 }
                 dist[v] = du_we;
                 sigma[v] += sigma[u];
                 p[v].push_back(u);
            }
        }

        vector<double> delta = vector<double> (n,0.0f);

        while(!s.empty())
        {
            int v = s.top();
            s.pop();

            for(int i = 0; i < (int)p[v].size(); i++)
            {
                int u = p[v][i];

                delta[u]  += sigma[u] * (1.0f + delta[v]) / (double)sigma[v];

                if(v != source)
                {
                    cb[v] = cb[v] + delta[v];
                }
            }
        }
    }

    for(int u = 0; u < (int)cb.size(); u++) cb[u] = cb[u]/2;

    return cb;
}
