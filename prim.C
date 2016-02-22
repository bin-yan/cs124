// Find a minimal spanning tree using Prim's algorithm.

#include <vector>
#include <algorithm>
#include <limits>
#include <stdio.h>
#include <cmath>

using namespace std;

struct Graph {

    int num_nodes;

    Graph (int _num_nodes) : num_nodes(_num_nodes) {}

    virtual float edge(int i, int j) = 0;

};


// Represent a graph through its adjacency matrix.
struct AdjacencyMatrixGraph : public Graph {
    float *edge_weights;
    
    AdjacencyMatrixGraph (int _num_nodes) : Graph(_num_nodes) {
	edge_weights = (float*) malloc(sizeof(float) * num_nodes * num_nodes);

	/*	for (int i=0; i<_num_nodes; i++) 
	    for (int j=0; j<=i; j++)
	    setEdge(i,j,  numeric_limits<float>::infinity()); */
    }

    long long edge_index(long long i, long long j) {
	if (i<j)
	    return (i*num_nodes + j);
	else
	    return (j*num_nodes + i);
    }
	    
    float edge(int i, int j) {
	return edge_weights[edge_index(i,j)];
    }

    void setEdge(int i, int j, float weight) {
	edge_weights[edge_index(i,j)] = weight;
    }
};

float square(float x) { return x*x; }

// A complete graph on random points in [0,1]^2.
struct EuclideanGraph : public Graph {
    float *coordinates;
    int dimension;

    EuclideanGraph (int _num_nodes, int _dimension) : Graph(_num_nodes), dimension(_dimension) {
	coordinates = (float*)malloc(sizeof(float) * num_nodes * dimension);
	for (int i=0; i<num_nodes*dimension; i++) {
	    coordinates[i] = (float)rand() / RAND_MAX;
	}	
    }

    ~EuclideanGraph() {
	free(coordinates);
    }

    float edge(int i, int j) {
	float sum = 0.0;
	for (int k=0; k<dimension; k++) {
	    sum += square(coordinates[i*dimension + k] - coordinates[j*dimension + k]);
	}
	return sqrt(sum);
    }
};

// A complete graph, with random edge weights in [0,1] computed by a simple hash function.
struct HashGraph : public Graph {
    int seed;
    
    HashGraph (int _num_nodes, int _seed) : Graph(_num_nodes), seed(_seed) {}


    float edge(int i, int j) {
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


	return (float)val / (float)0xFFFFFFFF;
    }
};

// Compute the cost of the minimum spanning tree.
float mst(Graph& g) {
    bool visited[g.num_nodes];
    for (int i=0; i<g.num_nodes; i++)
	visited[i] = 0;

    // This array stores the length of the shortest edge to node i, from any visited node.
    float weights[g.num_nodes];
    for (int i=0; i<g.num_nodes; i++)
	weights[i] = numeric_limits<float>::infinity();

    float total_weight = 0.0;

    // The node we are currently working on. We start with node 0.
    int i = 0;
    for (int iters=0; iters<g.num_nodes-1; iters++) {
	visited[i] = true;

	int next = -1;
	float next_weight = numeric_limits<float>::infinity();
	for (int j=0; j<g.num_nodes; j++) {
	    if (!visited[j]) {
		weights[j] = min(weights[j], g.edge(i,j));
		if (weights[j] < next_weight) {
		    next = j;
		    next_weight = weights[j];
		}
	    }
	}

	total_weight += next_weight;
	//printf("Add edge to %d of weight %f.\n", next, next_weight);
	i = next;	
    }
    

    return total_weight;
}

void test() {
    AdjacencyMatrixGraph g(5);

   /* Let us create the following graph
          2    3
      (0)--(1)--(2)
       |   / \   |
      6| 8/   \5 |7
       | /     \ |
      (3)-------(4)
            9          */
 
    g.setEdge(0,1,   2);
    g.setEdge(1,2,   3);
    g.setEdge(0,3,   6);
    g.setEdge(1,3,   8);
    g.setEdge(1,4,   5);
    g.setEdge(2,4,   7);
    g.setEdge(3,4,   9);

    mst(g);
}


int main(int argc, char *argv[]) {
    srand(time(NULL));

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

    printf("randmst %d %d %d %d\n", flag, numpoints, numtrials, dimension);

    const int N = 10000;

    //printf("Making a random graph.\n");
    /*
    AdjacencyMatrixGraph g(N);
    for (int i=0; i<N; i++)
	for (int j=0; j<=i; j++) 
	    g.setEdge(i,j,   (float)rand() / RAND_MAX);
    */
    //HashGraph g(N, rand());
 
    //HashGraph g(N, 0);

    EuclideanGraph g(N, 4);

    printf("Running Prim's algorithm.\n");
    printf("Weight of the tree is %f\n", mst(g));

    return 0;
}

