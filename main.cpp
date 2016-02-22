// A C / C++ program for Prim's Minimum Spanning Tree (MST) algorithm. 
// The program is for adjacency matrix representation of the graph

#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <time.h>
#include <limits>


class Graph {
    //unsigned char *edge_weights;
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

    ~Graph(){
        free(edge_weights);
    }


    float getEdgeWeights (long long int i, long long int j) {
        if (i > j) {
            return edge_weights[(i-1) * i / 2 + j];
        } else if (j > i) {
            return edge_weights[(j-1) * j / 2 + i];
        } else {
            return std::numeric_limits<float>::infinity();
        }

    }

    void setEdgeWeights (long long int i, long long int j, float weight) {
        if (i > j) {
            edge_weights[(i-1) * i / 2 + j] = weight;
        } else if (j > i) {
            edge_weights[(j-1) * j / 2 + i] = weight;
        }
    }


    void printGraph() {
        for (int i =0; i < num_vertices; i++) {
            for (int j=0; j < i; j++)
                printf("%d, %d, %f \n", i, j, getEdgeWeights(i,j));
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
        printf("%d - %d    %f \n", parent[i], i, graph->getEdgeWeights(i, parent[i]));
}

float getMSTsize(int parent[], Graph *graph)
{
    float size = 0;
    int V = graph->num_vertices;
    for (int i = 1; i < V; i++) {
        size = size + graph->getEdgeWeights(i, parent[i]);
    }

    return size;

}

// Function to construct and print MST for a graph represented using adjacency
// matrix representation
float primMST(Graph *graph)
{
    int V = graph->num_vertices;
    int parent[V]; // Array to store constructed MST
    float key[V];   // Key values used to pick minimum weight edge in cut
    bool mstSet[V];  // To represent set of vertices not yet included in MST

    // Initialize all keys as INFINITE
    for (int i = 0; i < V; i++)
        key[i] = std::numeric_limits<float>::infinity(), mstSet[i] = false;

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

            if (graph->getEdgeWeights(u,v) && mstSet[v] == false && graph->getEdgeWeights(u,v) < key[v])
                // actually edge might be 0.
                parent[v]  = u, key[v] = graph->getEdgeWeights(u,v);
    }

    return getMSTsize(parent, graph);

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
            float weight = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
            graph->setEdgeWeights(u, v, weight);

        }
    }
    //graph->printGraph();
    return graph;
}



// driver program to test above function
int main(int argc, char *argv[])
{

    int flag = 0;
    int numpoints = 0;
    int numtrials = 0;
    int dimension = 0;

    if (argc==5) {
        sscanf(argv[1], "%d", &flag);
        sscanf(argv[2], "%d", &numpoints);
        sscanf(argv[3], "%d", &numtrials);
        sscanf(argv[4], "%d", &dimension);
    }

    float average = 0;

    Graph *graph;
    for (int i=0; i<numtrials; i++) {
        graph =  constructGraph(numpoints);
        average = average + primMST(graph);
        delete graph;
    }

    average = average / numtrials;


    // Print the solution

    printf("%f %d %d %d\n", average, numpoints, numtrials, dimension);

    return 0;
}
