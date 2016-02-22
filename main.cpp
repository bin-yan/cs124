// A C / C++ program for Prim's Minimum Spanning Tree (MST) algorithm. 
// The program is for adjacency matrix representation of the graph

#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <time.h>
#include <limits>

// Number of vertices in the graph
#define V 4

class Graph {
    int num_vertices;
    float *edge_weights;
public:
    Graph(int _num_vertices) {
        num_vertices = _num_vertices;
        edge_weights = (float*)malloc(sizeof(float) * (num_vertices - 1) * num_vertices / 2);
    }

    float &edge (int i, int j) {
        if (i > j) {
            return edge_weights[(i-1)/2 * i + j];
        } else if (j > i) {
            return edge_weights[(j-1)/2 * j + i];
        } else {
            float m = INT_MAX;
            return m;
        }

    }

    void printGraph() {
        for (int i =0; i < num_vertices; i++) {
            for (int j=0; j < i; j++)
                printf("%d, %d, %f \n", i, j, edge(i,j));
        }

    }
};

// A utility function to find the vertex with minimum key value, from
// the set of vertices not yet included in MST
int minKey(float key[], bool mstSet[])
{
    // Initialize min value
    int min = INT_MAX, min_index;

    for (int v = 0; v < V; v++) {
        if (mstSet[v] == false && key[v] < min)
            min = key[v], min_index = v;
    }

    return min_index;
}

// A utility function to print the constructed MST stored in parent[]
int printMST(int parent[], int n, Graph *graph)
{
    printf("Edge   Weight\n");
    for (int i = 1; i < V; i++)
        printf("%d - %d    %f \n", parent[i], i, graph->edge(i, parent[i]));
}

// Function to construct and print MST for a graph represented using adjacency
// matrix representation
void primMST(Graph *graph)
{
    int parent[V]; // Array to store constructed MST
    float key[V];   // Key values used to pick minimum weight edge in cut
    bool mstSet[V];  // To represent set of vertices not yet included in MST

    // Initialize all keys as INFINITE
    for (int i = 0; i < V; i++)
        key[i] = INT_MAX, mstSet[i] = false;

    // Always include first 1st vertex in MST.
    key[0] = 0;     // Make key 0 so that this vertex is picked as first vertex
    parent[0] = -1; // First node is always root of MST

    // The MST will have V vertices
    for (int count = 0; count < V-1; count++)
    {
        // Pick thd minimum key vertex from the set of vertices
        // not yet included in MST
        int u = minKey(key, mstSet);

        // Add the picked vertex to the MST Set
        mstSet[u] = true;

        // Update key value and parent index of the adjacent vertices of
        // the picked vertex. Consider only those vertices which are not yet
        // included in MST
        for (int v = 0; v < V; v++)

            if (graph->edge(u,v) && mstSet[v] == false && graph->edge(u,v) < key[v])
                // actually edge might be 0.
                parent[v]  = u, key[v] = graph->edge(u,v);
    }

    // print the constructed MST
    printMST(parent, V, graph);
}



Graph *constructGraph()
{
    int u, v;
    Graph *graph = new Graph(V);
    //srand((unsigned)time(NULL));
    for(u = 0; u < V; u++)
    {
        for(v = 0; v < u; v++)
        {
            graph->edge(u,v) = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
        }
    }
    graph->printGraph();
    return graph;
}


// driver program to test above function
int main()
{
    /* Let us create the following graph
           2    3
       (0)--(1)--(2)
        |   / \   |
       6| 8/   \5 |7
        | /     \ |
       (3)-------(4)
             9          */

    Graph *graph =  constructGraph();

    // Print the solution
    primMST(graph);

    return 0;
}
