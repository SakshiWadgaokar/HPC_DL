#include <omp.h>
#include <iostream>
#include <queue>
#include <chrono>

using namespace std;
using namespace std::chrono;

class Node
{
public:
    int value;
    Node* left;
    Node* right;

    Node(int val)
    {
        value = val;
        left = NULL;
        right = NULL;
    }
};

Node* generateTree()
{
    Node* root = new Node(1);

    root->left = new Node(2);
    root->right = new Node(3);

    root->left->left = new Node(4);
    root->left->right = new Node(5);

    root->right->left = new Node(6);
    root->right->right = new Node(7);

    return root;
}

void bfs(Node* root)
{
    if (root == NULL)
        return;

    queue<Node*> q;
    q.push(root);

    while (!q.empty())
    {
        Node* current = q.front();
        q.pop();

        cout << current->value << " ";

        if (current->left != NULL)
            q.push(current->left);

        if (current->right != NULL)
            q.push(current->right);
    }
}

void dfs(Node* root)
{
    if (root == NULL)
        return;

    #pragma omp critical
    cout << root->value << " ";

    #pragma omp parallel sections
    {
        #pragma omp section
        dfs(root->left);

        #pragma omp section
        dfs(root->right);
    }
}

int main()
{
    Node* root = generateTree();

    auto start_bfs = high_resolution_clock::now();

    cout << "BFS Traversal: ";
    bfs(root);

    auto end_bfs = high_resolution_clock::now();

    auto bfs_time =
    duration_cast<microseconds>(end_bfs - start_bfs);

    cout << "\nBFS Execution Time: "
         << bfs_time.count()
         << " microseconds\n";

    auto start_dfs = high_resolution_clock::now();

    cout << "\nDFS Traversal: ";
    dfs(root);

    auto end_dfs = high_resolution_clock::now();

    auto dfs_time =
    duration_cast<microseconds>(end_dfs - start_dfs);

    cout << "\nDFS Execution Time: "
         << dfs_time.count()
         << " microseconds\n";

    return 0;
}