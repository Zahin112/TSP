#include <bits/stdc++.h>
#include <iostream>
#include <fstream>

using namespace std;

#define N 132
#define INF INT_MAX

class Node
{
public:
    int reduce_matrix[N][N];
    int cost;
    int v;
    int level;//which level on the journey/visited city no.
};

Node* nNode(int par_matrix[N][N],int level, int i, int j)
{
    Node* node = new Node;

    //Node* reducem_matrix=par_matrix;
    memcpy(node->reduce_matrix, par_matrix, sizeof node->reduce_matrix);

    if(level != 0)
    {
        for (int k = 0; k < N; k++)
        {
        node->reduce_matrix[i][k] = INF;
        node->reduce_matrix[k][j] = INF;
        }
    }

    node->reduce_matrix[j][0] = INF;
    node->level = level;
    node->v = j;
    return node;
}

void reduce_row(int reduce_matrix[N][N], int row[N])
{
    fill_n(row, N, INF);

    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            if (reduce_matrix[i][j] < row[i])
                row[i] = reduce_matrix[i][j];

    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            if (reduce_matrix[i][j] != INF && row[i] != INF)
                reduce_matrix[i][j] -= row[i];
}

void reduce_column(int reduce_matrix[N][N], int col[N])
{
    fill_n(col, N, INF);

    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            if (reduce_matrix[i][j] < col[j])
                col[j] = reduce_matrix[i][j];

    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            if (reduce_matrix[i][j] != INF && col[j] != INF)
                reduce_matrix[i][j] -= col[j];
}

int cost_calculation(int reduce_matrix[N][N])
{
    int cost = 0;

    int row[N];
    reduce_row(reduce_matrix, row);

    int col[N];
    reduce_column(reduce_matrix, col);

    for (int i = 0; i < N; i++)
    {
        if(row[i] != INF) cost+=row[i];
        if(col[i] != INF) cost+=col[i];
    }

    return cost;
}

class comp {
public:
    bool operator()(const Node* l, const Node* r) //const
    {
        return l->cost > r->cost;
    }
};

int BNB(int matrix[N][N])
{
    priority_queue<Node*, std::vector<Node*>, comp> pq;

    Node* root = nNode(matrix, 0, -1, 0);
    root->cost = cost_calculation(root->reduce_matrix);
    pq.push(root);
    while (!pq.empty())
    {
        Node* min = pq.top();
        pq.pop();
        int i = min->v;
        cout<<i<<endl;
        if (min->level == N - 1)
        {

            return min->cost;
        }

        for (int j = 0; j < N; j++)
        {
            if (min->reduce_matrix[i][j] != INF)
            {
                Node* child = nNode(min->reduce_matrix, min->level + 1, i, j);
                child->cost = min->cost + min->reduce_matrix[i][j] + cost_calculation(child->reduce_matrix);
                pq.push(child);
            }
        }

        delete min;
    }
}

int Brute(int reduce_matrix[N][N], int num_nodes)
{
    int src = 0,i=0;
    vector<int> nodes;
    for(int i=0;i<num_nodes;i++)
    {
        if(i != src)
            nodes.push_back(i);
    }

    int n = nodes.size();
    int cost = INF;

    while(next_permutation(nodes.begin(),nodes.end()))
    {
        //cout<<i++<<endl;
        int path_weight = 0;

        int j = src;

        for (int i = 0; i < n; i++)
        {
            if(reduce_matrix[j][nodes[i]]!= INF)
            {
              path_weight += reduce_matrix[j][nodes[i]];
              j = nodes[i];
            }
        }
        if(reduce_matrix[j][src]!= INF)
            path_weight += reduce_matrix[j][src];

        cost = min(cost, path_weight);
    }
    return cost;
}


int main()
{
    int matrix[N][N],u,v,w ;
    ifstream inf;
    inf.open("data.txt");

    ///matrix input
    ifstream fp("data.txt");
    if (!fp)
    {
        cout << "Error" << endl;
        return 1;
    }

    for(int i = 0; i < N; i++)
    {
        for(int j = 0; j < N; j++)
        {
            fp >> matrix[i][j];
            if ( matrix[i][j]==-1 )
               matrix[i][j]=INF;

        }
    }
    ///node input

    for(int i = 0; i < N; i++)
    {
        for(int j = 0; j < N; j++)
        {
               matrix[i][j]=INF;
        }
    }
    /*for(int i = 0; i < N; i++)
    {
        for(int j = 0; j < N; j++)
        {
               cout<<matrix[i][j]<<endl;

        }
    }*/


    for(int i = 0; i < N; i++)
    {
        inf >>u>>v>>w;
        //cout<<u <<v <<w <<endl;
        matrix[u][v]=w;
        //matrix[v][u]=w;
        cout<< matrix[u][v] <<endl;
    }
    ///inf >>u>>v>>w;
    ///matrix[u][v]=w;
    ///cout<< matrix[u][v] <<endl;
    /*
    int matrix[N][N] =
    {
        { INF, 20, 30, 10, 10 },
        { 15,  INF, 16,  4,  2 },
        { 3,   5,  INF, 2,  4 },
        { 19,   6,   18,   INF, 3 },
        { 16,   4,   7,   16, INF }
    };*/

    cout << "Cost is " << BNB(matrix);
    cout << "Cost is " << Brute(matrix,N);//10 min e 1025254 hoise 13 factorial r

    return 0;
}
