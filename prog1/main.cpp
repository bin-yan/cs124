// A C / C++ program for Prim's Minimum Spanning Tree (MST) algorithm. 
// The program is for adjacency matrix representation of the graph

#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <time.h>
#include <limits>


class Graph {
    float *edge_weights;
public:
    int num_vertices;
    Graph(int _num_vertices) {
        num_vertices = _num_vertices;
        edge_weights = (float*)malloc(sizeof(float) * (num_vertices - 1) * num_vertices / 2);
        if (edge_weights == NULL) {
            printf("error");
        }
    }

    float &edge (int i, int j) {
        if (i > j) {
            return edge_weights[(i-1) * i / 2 + j];
        } else if (j > i) {
            return edge_weights[(j-1) * j / 2 + i];
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
int minKey(float key[], bool mstSet[], int V)
{
    // Initialize min value
    float min = INT_MAX, min_index;

    for (int v = 0; v < V; v++) {
        if (mstSet[v] == false && key[v] < min) {
            min = key[v], min_index = v;
        }
    }

    return min_index;
}

// A utility function to print the constructed MST stored in parent[]
int printMST(int parent[], Graph *graph)
{
    int V = graph->num_vertices;
    printf("Edge   Weight\n");
    for (int i = 1; i < V; i++)
        printf("%d - %d    %f \n", parent[i], i, graph->edge(i, parent[i]));
}

// Function to construct and print MST for a graph represented using adjacency
// matrix representation
void primMST(Graph *graph)
{
    int V = graph->num_vertices;
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
        int u = minKey(key, mstSet, V);
        //printf("%d\n", u);

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

    printf("%f", graph->edge(V-1,V-2));

    // print the constructed MST
    //printMST(parent, graph);
}



Graph *constructGraph(int V)
{
    int u, v;
    Graph *graph = new Graph(V);
    //srand((unsigned)time(NULL));
    srand(42);
    for(u = 0; u < V; u++)
    {
        for(v = 0; v < u; v++)
        {
            graph->edge(u,v) = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);

        }
    }
    //graph->printGraph();
    return graph;
}



// driver program to test above function
int main()
{
    int V = 46342;

    Graph *graph =  constructGraph(V);

    // Print the solution
    primMST(graph);
    printf("done, %d", V);

    return 0;
}
