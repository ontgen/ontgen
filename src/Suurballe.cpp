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
#include "Suurballe.hpp"

Suurballe::Suurballe(){}

Suurballe::~Suurballe(){}

void Suurballe::insertSubtree(Graph &graph, tree<int> &tr, typename tree<int>::iterator &root,vector<int> &nodes, vector<int> &controller, int source)
{
    Node &node = graph.getNodeAtPosition(source);
    vector<int> &adjacents = node.getAdjacentsNodes();

    typename tree<int>::iterator temp;
    typename tree<int>::iterator newRoot;
    
    int newSource = source;

    if (find(this->nodeInTree.begin(),this->nodeInTree.end(),source) == this->nodeInTree.end())
    {
        this->nodeInTree.push_back(source);
    } else {
        return;
    }

    int it = 0;
    int n = (int)adjacents.size();

    while( it <  n)
    {
        if (controller[ adjacents[it] ] == -1)
        {
            temp = tr.append_child( root, adjacents[it] );

            controller[ adjacents[it] ] = adjacents[it];

            if (find(this->nodeInTree.begin(),this->nodeInTree.end(),adjacents[it]) == this->nodeInTree.end())
            {
                this->nodeInTree.push_back(adjacents[it]);
            }

            if( find(nodes.begin(),nodes.end(),adjacents[it]) != nodes.end() )
            {
                newRoot = temp;
                newSource = adjacents[it];
            }
        }

        it++;
    }

    n = ((int)nodes.size())-1;

    if (source == nodes[n])
    {
        return;
    }

    insertSubtree(graph,tr,newRoot,nodes,controller,newSource);
}

tree<int> Suurballe::makeTree(Graph &graph, vector<int> nodes, int source)
{
    vector<Node> auxiliar;
    auxiliar = graph.getNodes();

    tree<int> tr;

    tree<int>::iterator root, top;

    top = root = tr.begin();

    vector<int> controller = vector<int> (this->numberOfNodes,-1);

    controller[source] = source;

     insertSubtree(graph,tr,root,nodes,controller,source);

    for (int i = 0; i < (int) nodes.size(); i++)
    {
        root = tr.insert( root, nodes[i] );
        controller[nodes[i]] = 1;

        Node n = graph.getNodeAtPosition(nodes[i]);
        vector<int> adjacents = n.getAdjacentsNodes();

        for (int k = 0; k < (int) adjacents.size(); k++)
        {
            if (controller[adjacents[k]] == -1 && find(nodes.begin(), nodes.end(), adjacents[k]) == nodes.end()) {
                tr.insert(root, adjacents[k]);
            }
        }
    }

//     cout<<"***"<<endl;
//     kptree::print_tree_bracketed(tr,cout); //imprime árvore
//     cout<<"\n***"<<endl;

    return tr;
}

/**
 * Atualiza peso dos nós da árvore para 0
 * e as demais ligações aplica a função:
 * w'(u,v) = w (w,u) - d(s,v) + d(s,u) 
 */
void Suurballe::updateEdgesWeight(const tree<int>& t, typename tree<int>::iterator iRoot, vector<int> nodes, Graph & graph, int source)
{
    if( t.empty() )
    {
        return;
    }

    if (t.number_of_children(iRoot) == 0)
    {
        return;
    }
    else
    {
        // child1, ..., childn
        // int siblingCount = t.number_of_siblings( t.begin(iRoot) );
        int siblingNum;

        typename tree<int>::sibling_iterator iChildren;
        double weight = 0.0f;//w'(u,v) = w (w,u) - d(s,v) + d(s,u)

        for (iChildren = t.begin(iRoot), siblingNum = 0; iChildren != t.end(iRoot); ++iChildren, ++siblingNum)
        {
            weight = 0.0f;
            int u = *iRoot, v = *iChildren;

             /**
             * Remove arestas do caminho mínimo de ida
             * Deixando somente as arestas de volta
             */
            if ( (nodes[ u ] == u || u == source) && ( nodes[ v ] == v || v == source) )
            {
                graph.setWeightEdgeDirected(u,v,weight);
                graph.setWeightEdgeDirected(v,u,weight);

                graph.removeNode(u,v);//remove o nó do conjunto de adjacente
            }
            else if (find(this->nodeInTree.begin(),this->nodeInTree.end(),u) != this->nodeInTree.end() && find(this->nodeInTree.begin(),this->nodeInTree.end(),v) != this->nodeInTree.end() )
            {
                graph.setWeightEdgeDirected(u,v,weight);

                weight = this->distance[v][u] - this->distance[source][u] + this->distance[source][v];
                graph.setWeightEdgeDirected(v,u,weight);
            }
            else
            {
                weight = this->distance[u][v] - this->distance[source][v] + this->distance[source][u];
              
                graph.setWeightEdgeDirected(u,v,weight);

                u = *iChildren, v = *iRoot;

                weight = this->distance[u][v] - this->distance[source][v] + this->distance[source][u];

                graph.setWeightEdgeDirected(u,v,weight);
            }

            this->treePath[v][u] = 1;
            this->treePath[u][v] = 1;

            updateEdgesWeight(t,iChildren,nodes,graph,source);
        }
    }
}

/**
 * Todas as arestas da árvore receberão peso 0
 * Os demais nós será aplicado a fórmula proposta:
 *      w'(u,v) = w (w,u) - d(s,v) + d(s,u)
 */
void Suurballe::changeEdgesWeights(Graph & graph, tree<int> tr, vector<int> nodes)
{
    int source = *tr.begin();

    /**
     * Percore árvore atualizando o peso das ligações u --> v
     * Se o nodo esta no caminho o peso passa a ser 0 (zero)
     * Do contrário aplica a equação proposta por Suurballe
     * Sendo s o source e u e v nós pertencentes a árvore
     *      w'(u,v) = w (u,v) - d(s,v) + d(s,u)
     */

    // int headCount = tr.number_of_siblings(tr.begin());//número de cabeças da árvore

    typename tree<int>::sibling_iterator iRoot = tr.begin();


    int count = 0;

    vector<int> temp = vector<int> (this->numberOfNodes,-1);

    while( count < (int)nodes.size() )
    {
        temp[ nodes[count] ] = nodes[count];

        count++;
    }

    updateEdgesWeight(tr,iRoot,temp,graph,source);//atualiza peso e remove ligações

    /**
     * Atualiza nós não pertencentes a árvore
     */
    vector<Node> n = graph.getNodes();

    for (int u = 0; u < (int)this->nodeInTree.size(); u++)
    {
        vector<int> adjacents = n[u].getAdjacentsNodes();

        for (int v = 0; v < (int)adjacents.size(); v++)
        {
            double weight = 0.0f;
            int w = adjacents[v];

            if (this->treePath[u][v] == 0)
            {
                weight = this->distance[u][w] - this->distance[source][w] + this->distance[source][u];
                graph.setWeightEdgeDirected(u,w,weight);

                weight = this->distance[w][u] - this->distance[source][u] + this->distance[source][w];
                graph.setWeightEdgeDirected(w,u,weight);
            
                this->treePath[w][u] = 1;
                this->treePath[u][w] = 1;
            }
        }
    }

}


void Suurballe::changeEdgesDistances(Graph &graph, vector<int> nodes, int source)
{
    vector<int> controller = vector<int> (this->numberOfNodes,-1);
    double weight = 0.0f;

    for (int i = 0; i < (int) nodes.size(); i++)
    {
        controller[nodes[i]] = 1;

        Node n = graph.getNodeAtPosition(nodes[i]);
        vector<int> adjacents = n.getAdjacentsNodes();

        for (int k = 0; k < (int) adjacents.size(); k++)
        {
            int w = adjacents[k];

//            if (controller[w] == -1 && find(nodes.begin(), nodes.end(), adjacents[k]) == nodes.end())
//            {

                weight = this->distance[i][w] - this->distance[source][w] + this->distance[source][i];
                graph.setWeightEdgeDirected(i,w,weight);

                weight = this->distance[w][i] - this->distance[source][i] + this->distance[source][w];
                graph.setWeightEdgeDirected(w,i,weight);
//            }

            weight = 0.0f;
        }
    }

    for(int k = 0; k < (int)nodes.size()-1; k += 2)
    {
        graph.setWeightEdgeDirected(nodes[k],nodes[k+1],0.0f);
        graph.setWeightEdgeDirected(nodes[k+1],nodes[k],0.0f);

        graph.removeNode(nodes[k],nodes[k+1]);//remove o nó do conjunto de adjacente
    }
}

void Suurballe::makePathVector(vector<int> &p1,vector<int> &p2, vector<int> &inPath)
{
    for (int u = 0; u < (int)p1.size()-1; u++)
    {
        p2.push_back( p1[u] );
        p2.push_back( p1[u+1] );

        inPath[ p1[u] ] = p1[u];
        inPath[ p1[u+1] ] = p1[u+1];
    }
}

vector<int> Suurballe::disjointPath(int target)
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

    return path;
}

/**
 * armazena nós visitados em visited
 */
bool Suurballe::findPath(Graph &g, vector<Node> nodes,int source, int target)
{
    if( this->visited[source] == 1 || source == target ) return true;

    this->visited[source] = 1;

    vector<int> adjacents = nodes[source].getAdjacentsNodes();

    for( int u = 0; u < this->numberOfNodes; u++)
    {
        //verifica se existe ligação
        if ( find(adjacents.begin(),adjacents.end(),u) != adjacents.end() )
        {
            g.removeNode(source,u);
            this->parent[u] = source;

            if( findPath(g, nodes,u,target) ) return true;
        }
    }

    return false;
}

bool Suurballe::makeSubgraphDisjointPaths(Graph &g, int source, int target)
{
    vector<Node> nodes = g.getNodes();

    //nó origem com mais de uma aresta de saída
    if ((int)nodes[source].getAdjacentsNodes().size() >= 3 )
    {
        return false;
    }

    //nó destino possui aresta de saída
    if ((int)nodes[target].getAdjacentsNodes().size() >= 1 )
    {
        return false;
    }

    int count = 0;

    vector<vector<int>> p;

    while(count < 2)
    {

        this->visited = vector<int> (g.getNumberOfNodes(),0);
        this->parent = vector<int> (g.getNumberOfNodes(),-1);

        findPath(g,nodes,source,target);

        p.push_back( disjointPath(target) );

        nodes = g.getNodes();
        count++;
    }

    nodes = g.getNodes();

    //verifica se os nós não possuem mais nenhuma ligação
    for (int u = 0; u < g.getNumberOfNodes(); u++)
    {
        if ((int)nodes[u].getAdjacentsNodes().size() >= 1 )
        {
            return false;
        }
    }


    return true;
}

void Suurballe::discardCommonEdge(vector<int> &p1, vector<int> &p2, int x, int y)
{
    vector<int> t1, t2;
    int u = 0;

    for (u = 0; u < (int)p1.size(); u += 2)
    {
        if( u == x )
        {
            break;
        }

        t1.push_back(p1[u]);
        t1.push_back(p1[u+1]);
    }

    for (u = 0; u < (int)p2.size(); u += 2)
    {
        if( u == y ) {
            break;
        }
        
        t2.push_back(p2[u]);
        t2.push_back(p2[u+1]);
    }

    for (u = y+2; u < (int)p2.size(); u += 2)
    {
        if (p2[u] == t1[(int)t1.size() - 1] && p2[u+1] == t1[(int)t1.size() - 2])
        {
            t1.erase( t2.begin() + (int)t1.size() - 1 );
            t1.erase( t2.begin() + (int)t1.size() - 1 );
            continue;
        }

        t1.push_back(p2[u]);
        t1.push_back(p2[u+1]);
    }
  
    for (u = x+2; u < (int)p1.size(); u += 2)
    {
        if (p1[u] == t2[(int)t2.size() - 1] && p1[u+1] == t2[(int)t2.size() - 2])
        {
            t2.erase( t2.begin() + (int)t2.size() - 1 );
            t2.erase( t2.begin() + (int)t2.size() - 1 );
            continue;
        }

        t2.push_back(p1[u]);
        t2.push_back(p1[u+1]);
    }

    p1.clear();
    p2.clear();

    for (u = 0; u < (int)t1.size(); u++)
    {
        p1.push_back(t1[u]);
    }

    for (u = 0; u < (int)t2.size(); u++)
    {
        p2.push_back(t2[u]);
    }
}

bool Suurballe::makeDisjointPaths(vector<int> path1, vector<int> path2, Graph &graph)
{

    vector<int> p1,p2;

    vector<int> temp = vector<int> (this->numberOfNodes,-1);

    makePathVector(path1,p1,temp);
    makePathVector(path2,p2,temp);

    /**
     * Remover arestas invertidas
     * Dos caminhos mínimos p1 e p2
     */

    for (int u = 0; u < (int)p1.size()-1; u+=2)
    {
        for (int v = 0; v < (int)p2.size()-1; v+=2)
        {
            //exclui arestas em comum mas invertidas
            if (p1[u] == p2[v+1] && p1[u+1] == p2[v])
            {
                discardCommonEdge(p1,p2,u,v);
            }

        }
    }

    /**
     * Contruir sub-grafo
     */
    vector< vector<int> > subGraph = vector< vector<int> > ( this->numberOfNodes, vector<int> (this->numberOfNodes,0) );

    Graph g;

    g.setNumberOfNodes(this->numberOfNodes);

    g.memsetGraph();

    /**
     * Insere as arestas e seus respectivos pesos
     */
    g.setMaximumDegree(g.getNumberOfNodes()-1);
    g.setMinimumDegree(1);


    for (int u = 0; u < (int)p1.size(); u+=2)
    {
        g.setEdgeDirected(p1[u],p1[u+1]);

    }

    for (int u = 0; u < (int)p2.size(); u+=2)
    {
        g.setEdgeDirected(p2[u],p2[u+1]);

    }

    int source = path1[0];
    int target = path1[ (int)path1.size()-1 ];

    return makeSubgraphDisjointPaths(g,source,target);
}

bool Suurballe::execute(Graph &graph)
{
    if (graph.getNumberOfNodes() <= 2)
    {
        return false;
    }

    bool survivor = false;

    this->numberOfPaths = 0;
    Dijkstra dijkstra;

    this->numberOfNodes = graph.getNumberOfNodes();
    this->distance = vector<vector<int>> (this->numberOfNodes,vector<int>( this->numberOfNodes,0) );

    int n = 0;
    
    /**
     * Para cada par de nós (u,v)
     * Obtêm caminho mínimo
     */
    for (int u = 0; u < this->numberOfNodes-1; u++)
    {
        for(int v = u+1; v < this->numberOfNodes; v++)
        {
            this->distance[u][v] = this->distance[v][u] = dijkstra.execute(graph,u,v);

            this->path.push_back( dijkstra.shortestPath(v) );
            
            //não têm caminho entre u e v
            if ( (int)path[n].size() <= 1 || this->distance[u][v] == std::numeric_limits<double>::max())
            {
                return survivor;
            }

            this->numberOfPaths++;
            n++;
        }
    }
//    cout<<"-----------------------------\n";
    int iterator = 0;
    double dist = 0;

    for (int u = 0; u < this->numberOfNodes-1; u++)
    {
        for (int v = u+1; v < this->numberOfNodes; v++)
        {
            Graph auxiliar = graph;

            /**
             * mudança de peso nas arestas
             * Monta árvore a partir do nó u
             */
            // cout<<"----------------------------\n"<<endl;
            // cout<<"U "<<u<<" V "<<v<<endl;
            this->treePath = vector<vector<int>> (this->numberOfNodes,vector<int>(this->numberOfNodes,0)); 
//            tree<int> tr = makeTree(auxiliar,this->path[iterator], u);

//            if((int)tr.size() <= 0) return false;

            // cout<<"------------------------ "<<u<<" "<<v<<"------------------"<<endl;
//            changeEdgesWeights(auxiliar, tr, this->path[iterator]);
            changeEdgesDistances(auxiliar,this->path[iterator], u);

            // for(int w = 0; w < auxiliar.getNumberOfNodes();w++) auxiliar.printAdjacents(w);
            dist =  dijkstra.execute(auxiliar,u,v);
            // cout<<"----------------------------------------------------------"<<endl;

            //verifica se encontrou caminho
            if ( dist == std::numeric_limits<double>::max() )
            {
                return survivor;
            }

            vector<int> newPath = dijkstra.shortestPath(v);

            //não encontrou caminho
            if ((int)newPath.size() <= 1)
            {
                return false;
            }

            //verifica se os dois caminhos são aresta disjuntas
            // for(int w = 0; w < (int)newPath.size();w++) cout<<" "<<newPath[w];
            // cout<<endl;
//            for(int w = 0; w < (int)path[iterator].size();w++) cout<<" "<<path[iterator][w];
            survivor = makeDisjointPaths(path[iterator],newPath, graph);
            
            if (survivor == false)
            {
                return false;
            }

            this->nodeInTree.clear();

            iterator++;
            survivor = false;
        }
    }
    survivor = true;
    return true;//conseguiu encontrar 2 caminhos de arestas disjuntos para todos os pares de nós

}
