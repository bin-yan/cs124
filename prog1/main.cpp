// A C / C++ program for Prim's Minimum Spanning Tree (MST) algorithm. 
// The program is for adjacency matrix representation of the graph

#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <time.h>
#include <limits>
#include <math.h>


struct Graph {
    //unsigned char *edge_weights;
    int num_vertices;

    Graph(int _num_vertices) {
        num_vertices = _num_vertices;

    }


    virtual float getEdgeWeights (int i, int j) = 0;

    virtual ~Graph(){};


    void printGraph() {
        for (int i =0; i < num_vertices; i++) {
            for (int j=0; j < i; j++)
                printf("%d, %d, %f \n", i, j, getEdgeWeights(i,j));
        }

    }
};

struct AdjacencyMatrixGraph : Graph{
    //unsigned char *edge_weights;
    float *edge_weights;


    AdjacencyMatrixGraph (int _num_vertices) : Graph (_num_vertices){

        edge_weights = (float*)malloc(sizeof(float) * (num_vertices - 1) * num_vertices / 2);
        if (edge_weights == NULL) {
            printf("error");
        }
    }

    ~AdjacencyMatrixGraph(){
        free(edge_weights);
    }


    float getEdgeWeights (int i, int j) {
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
};

struct HashGraph : public Graph {
    int seed;

    HashGraph (int _num_vertices, int _seed) : Graph(_num_vertices), seed(_seed) {}

    float getEdgeWeights(int i, int j) {
        if (i<j) return getEdgeWeights(j,i);

        unsigned int val = seed;

        val +=i;
        val *= 1103515245;
        val += (val << 10);
        val ^= (val >> 6);
        val += (val << 3);
        val ^= (val >> 11);
        val += (val << 15);

        val +=j;
        val *= 1103515245;
        val += (val << 10);
        val ^= (val >> 6);
        val += (val << 3);
        val ^= (val >> 11);
        val += (val << 15);


        return (float)(val & 0xFFFFFFFF) / (float)0xFFFFFFFF;
    }
};

struct EuclideanGraph : public Graph {
    float *coordinates;
    int dimension;

    EuclideanGraph (int _num_vertices, int _dimension) : Graph(_num_vertices), dimension(_dimension) {
        coordinates = (float*)malloc(sizeof(float) * num_vertices * dimension);
        for (int i=0; i<num_vertices*dimension; i++) {
            coordinates[i] = (float)rand() / RAND_MAX;
        }
    }

    ~EuclideanGraph() {
        free(coordinates);
    }

    float getEdgeWeights(int i, int j) {
        float sum = 0;
        for (int k=0; k<dimension; k++) {
            sum = sum + (float)pow((coordinates[i*dimension + k] - coordinates[j*dimension + k]), 2);
        }
        return sqrt(sum);
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
    AdjacencyMatrixGraph *graph = new AdjacencyMatrixGraph(V);
    //srand(42);
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

    srand((unsigned)time(NULL));

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
        if(dimension==1) {
            if(numpoints <= 33000) {
                graph =  constructGraph(numpoints);

            } else {
                graph = new HashGraph(numpoints, rand());
                //printf("%f", graph->getEdgeWeights(60000,60001));
            }
        } else {
                graph = new EuclideanGraph(numpoints, dimension);
        }


        average = average + primMST(graph);
        delete graph;

    }

    average = average / numtrials;


    // Print the solution

    printf("%f %d %d %d\n", average, numpoints, numtrials, dimension);

    return 0;
}
