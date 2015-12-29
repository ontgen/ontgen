/**
 * Copyright 2015 Silvana Trindade
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 * http://www.apache.org/licenses/LICENSE-2.0
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "Suurballe.hpp"

Suurballe::Suurballe(){}

Suurballe::~Suurballe(){}

void Suurballe::insertSubtree(Graph graph, tree<int> &tr, typename tree<int>::iterator root,vector<int> nodes, vector<int> &controller, int source)
{
    Node node = graph.getNodeAtPosition(source);
    vector<int> adjacents = node.getAdjacentsNodes();
    typename tree<int>::iterator temp;
    typename tree<int>::iterator newRoot;
    int newSource = source;

    if (find(this->nodeInTree.begin(),this->nodeInTree.end(),source) == this->nodeInTree.end())
    {
        this->nodeInTree.push_back(source);
    }

    unsigned int it = 0;

    while( adjacents.size() > it )
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

    int n = nodes.size()-1;

    if (source == nodes[n] )
    {
        return;
    }

    insertSubtree(graph,tr,newRoot,nodes,controller,newSource);
}

tree<int> Suurballe::makeTree(Graph graph, vector<int> nodes, int source)
{
    vector<Node> auxiliar;
    auxiliar = graph.getNodes();

    tree<int> tr;

    tree<int>::iterator root, top, parent;

    top = tr.begin();
    root = tr.insert( top, source );

    vector<int> controller = vector<int> (this->numberOfNodes,-1);

    controller[source] = source;

    insertSubtree(graph,tr,root,nodes,controller,source);

    // cout<<endl;
    // kptree::print_tree_bracketed(tr,cout); //imprime árvore
    // cout<<endl;

    // for (unsigned int i = 0; i < this->nodeInTree.size(); ++i)
    // {
    //     cout<<this->nodeInTree[i]<<" ";
    // }
    // cout<<endl;

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
        double weight = 0.0f ;//w'(u,v) = w (w,u) - d(s,v) + d(s,u)

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

                graph.removeNode(u,v);
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

void Suurballe::makePathVector(vector<int> p1,vector<int> &p2, vector<int> &inPath)
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
    // cout<<"\n-----------D---------------"<<endl;
    
    // cout<<endl;
    for (u = 0; u < (int)p1.size(); u += 2)
    {
        if( u == x )
        {
            break;
        }

        t1.push_back(p1[u]);
        t1.push_back(p1[u+1]);
    }
    // cout<<"\n"<<endl; v = 0;   
    for (u = 0; u < (int)p2.size(); u += 2)
    {
        if( u == y ) {
            break;
        }
        
        t2.push_back(p2[u]);
        t2.push_back(p2[u+1]);
    }

    // cout<<"\nsegunda parte "<<u<<" "<<v<<"\n"<<endl;
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

bool Suurballe::execute(Graph & graph)
{

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
            if ( (int)path[n].size() <= 1)
            {
                cout<<" "<<u<<" "<<v<<endl;
                return survivor;
            }

            this->numberOfPaths++;
            n++;
        }
    }

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
            tree<int> tr = makeTree(auxiliar, this->path[iterator], u);
            // cout<<"------------------------ "<<u<<" "<<v<<"------------------"<<endl;
            changeEdgesWeights(auxiliar, tr, this->path[iterator]);
            // for(int w = 0; w < auxiliar.getNumberOfNodes();w++) auxiliar.printAdjacents(w);
            dist =  dijkstra.execute(auxiliar,u,v);
            // cout<<"----------------------------------------------------------"<<endl;

            //verifica se encontrou caminho
            if ( dist == std::numeric_limits<double>::max() )
            {
                cout<<" "<<u<<" "<<v<<endl;
                return survivor;
            }

            vector<int> newPath = dijkstra.shortestPath(v);
            //não encontrou caminho
            if ((int)newPath.size() <= 1)
            {
                cout<<" "<<u<<" "<<v<<endl;
                return survivor;
            }

            //verifica se os dois caminhos são aresta disjuntas
            // for(int w = 0; w < (int)newPath.size();w++) cout<<" "<<newPath[w];
            // cout<<endl;
            for(int w = 0; w < (int)path[iterator].size();w++) cout<<" "<<path[iterator][w];
            survivor = makeDisjointPaths(path[iterator],newPath, graph);
            
            if (survivor == false)
            {
                cout<<" "<<u<<" "<<v<<endl;
                return survivor;
            }

            // this->nodeInTree.clear();

            iterator++;
            survivor = false;
        }
    }

    survivor = true;//conseguiu encontrar 2 caminhos de arestas disjuntos

    return survivor;
}
