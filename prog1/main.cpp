<<<<<<< Updated upstream
<<<<<<< HEAD
#include <iostream>

using namespace std;

int main() {
    cout << "Hello, World!" << endl;
    return 0;
}
=======
// A C / C++ program for Prim's Minimum Spanning Tree (MST) algorithm. 
// The program is for adjacency matrix representation of the graph

=======
>>>>>>> Stashed changes
#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <time.h>
#include <limits>
#include <math.h>

struct Graph {

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
    float *edge_weights;

    AdjacencyMatrixGraph (int _num_vertices) : Graph (_num_vertices){
        edge_weights = (float*)malloc(sizeof(float) * (num_vertices - 1) * num_vertices / 2);
        if (edge_weights == NULL) {
            printf("error");
        }

        int u, v;
        for(u = 0; u < num_vertices; u++)
        {
            for(v = 0; v < u; v++)
            {
                float weight = (float)rand() / RAND_MAX;
                setEdgeWeights(u, v, weight);

            }
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


int getNextVertex(float MSTweights[], bool inMST[], int V)
{
    // Initialize min value
    float min_weight = std::numeric_limits<float>::infinity();
    int next_vertex;

    for (int v = 0; v < V; v++) {
        if (!inMST[v]  && MSTweights[v] < min_weight) {
            min_weight = MSTweights[v], next_vertex = v;
        }
    }

    return next_vertex;
}

void printMST(int MSTparents[], float MSTweights[], int V)
{
    printf("The minimum spanning tree\n");
    for (int i = 1; i < V; i++)
        printf("%d - %d    %f \n", MSTparents[i], i, MSTweights[i]);
}


float findPrimMST(Graph *graph)
{
    int V = graph->num_vertices;
    int MSTparents[V];
    float MSTweights[V];
    bool inMST[V];
    float MSTsize = 0;


    for (int i = 0; i < V; i++) {
        MSTweights[i] = std::numeric_limits<float>::infinity();
        inMST[i] = false;
    }

    // vertex 0 as root, include it.
    MSTweights[0] = 0;
    MSTparents[0] = -1;


    for (int count = 0; count < V-1; count++)
    {
        int u = getNextVertex(MSTweights, inMST, V);

        inMST[u] = true;

        for (int v = 0; v < V; v++)

            if (!inMST[v] && MSTweights[v] > graph->getEdgeWeights(u,v)) {
                MSTparents[v]  = u;
                MSTweights[v] = graph->getEdgeWeights(u,v);
            }
    }

    //caluclate MST size
    for (int v = 0; v < V; v++){
        MSTsize = MSTsize + MSTweights[v];
    }

    //printMST(MSTparents, MSTweights, V);

    return MSTsize;
}


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
                graph =  new AdjacencyMatrixGraph(numpoints);

            } else {
                graph = new HashGraph(numpoints, rand());
            }
        } else {
                graph = new EuclideanGraph(numpoints, dimension);
        }


        average = average + findPrimMST(graph);
        delete graph;

    }

    average = average / numtrials;

    // Print the result
    printf("%f %d %d %d\n", average, numpoints, numtrials, dimension);

    return 0;
}
>>>>>>> eabbf05c7193b86a35ec05d13f67d0a389edf480
