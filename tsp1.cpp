#include <bits/stdc++.h>
#include <iostream>

using namespace std;

#define N 5
#define INF INT_MAX

class Node
{
public:
    vector<pair<int, int>> path;//calculate current path
    int matrix_reduced[N][N];
    int cost;
    int vertex;
    int level;//which level on the journey/visited city no.
};

Node* newNode(int par_matrix[N][N], vector<pair<int, int>> path,int level, int i, int j)
{
    Node* node = new Node;
    node->path = path;
    if (level != 0)
        node->path.push_back(make_pair(i, j));
    memcpy(node->matrix_reduced, par_matrix, sizeof node->matrix_reduced);
    //node->matrix_reduced = par_matrix;

    if(level != 0)
    {
        for (int k = 0; k < N; k++)
        {
        node->matrix_reduced[i][k] = INF;
        node->matrix_reduced[k][j] = INF;
        }
    }

    node->matrix_reduced[j][0] = INF;
    node->level = level;
    node->vertex = j;
    return node;
}

void reduce_row(int matrix_reduced[N][N], int row[N])
{
    fill_n(row, N, INF);

    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            if (matrix_reduced[i][j] < row[i])
                row[i] = matrix_reduced[i][j];

    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            if (matrix_reduced[i][j] != INF && row[i] != INF)
                matrix_reduced[i][j] -= row[i];
}

void reduce_column(int matrix_reduced[N][N], int col[N])
{
    fill_n(col, N, INF);

    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            if (matrix_reduced[i][j] < col[j])
                col[j] = matrix_reduced[i][j];

    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            if (matrix_reduced[i][j] != INF && col[j] != INF)
                matrix_reduced[i][j] -= col[j];
}

int cost_calculation(int matrix_reduced[N][N])
{
    int cost = 0;

    int row[N];
    reduce_row(matrix_reduced, row);

    int col[N];
    reduce_column(matrix_reduced, col);

    for (int i = 0; i < N; i++)
    {
        if(row[i] != INT_MAX) cost+=row[i];
        if(col[i] != INT_MAX) cost+=col[i];
    }

    return cost;
}

class comp {
public:
    bool operator()( Node* lhs,  Node* rhs) //const
    {
        return lhs->cost > rhs->cost;
    }
};

int BNB(int matrix[N][N])
{
    priority_queue<Node*, std::vector<Node*>, comp> pq;
    vector<pair<int, int>> v;
    Node* root = newNode(matrix, v, 0, -1, 0);
    root->cost = cost_calculation(root->matrix_reduced);
    pq.push(root);
    while (!pq.empty())
    {
        Node* min = pq.top();
        pq.pop();
        int i = min->vertex;
        if (min->level == N - 1)
        {
            min->path.push_back(make_pair(i, 0));
            return min->cost;
        }

        for (int j = 0; j < N; j++)
        {
            if (min->matrix_reduced[i][j] != INF)
            {
                Node* child = newNode(min->matrix_reduced, min->path, min->level + 1, i, j);
                child->cost = min->cost + min->matrix_reduced[i][j] + cost_calculation(child->matrix_reduced);
                pq.push(child);
            }
        }

        delete min;
    }
}

int main()
{
    int matrix[N][N] =
    {
        { INF, 20, 30, 10, 10 },
        { 15,  INF, 16,  4,  2 },
        { 3,   5,  INF, 2,  4 },
        { 19,   6,   18,   INF, 3 },
        { 16,   4,   7,   16, INF }
    };

    cout << "Cost is " << BNB(matrix);

    return 0;
}

