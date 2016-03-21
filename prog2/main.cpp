#include <sstream>
#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>

using namespace std;

int threshold;

void readFile(string filename, int n, vector< vector<int> > &A, vector< vector<int> > &B) {
    string line;
    std::fstream fs;
    fs.open (filename, std::fstream::in);

    int a;

    for (int i=0; i < n; i++) {
        for (int j=0; j < n; j++) {
            getline(fs, line);
            istringstream iss(line);
            iss >> a;
            A[i][j] = a;
        }
    }

    int b;
    for (int i=0; i < n; i++) {
        for (int j=0; j < n; j++) {
            getline(fs, line);
            istringstream iss(line);
            iss >> b;
            B[i][j] = b;
        }
    }

    fs.close();
}

void printMatrix(vector< vector<int> > matrix, int n) {
    for (int i=0; i < n; i++) {
        for (int j=0; j < n; j++) {
            if (j != 0) {
                cout << "\t";
            }
            cout << matrix[i][j];
        }
        cout << endl;
    }
}

void conventional(vector< vector<int> > A,
                  vector< vector<int> > B,
                  vector< vector<int> > &C, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            C[i][j] = 0;
            for (int k = 0; k < n; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

void sum(vector< vector<int> > &A,
         vector< vector<int> > &B,
         vector< vector<int> > &C, int n) {
    int i, j;

    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            C[i][j] = A[i][j] + B[i][j];
        }
    }
}

void subtract(vector< vector<int> > &A,
              vector< vector<int> > &B,
              vector< vector<int> > &C, int n) {
    int i, j;

    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            C[i][j] = A[i][j] - B[i][j];
        }
    }
}

void strassenRecursive(vector< vector<int> > &A,
               vector< vector<int> > &B,
               vector< vector<int> > &C, int n) {
    if (n <= threshold) {
        conventional(A, B, C, n);
        return;
    }

    else {
        int n_new = n/2;
        vector<int> inner (n_new);
        vector< vector<int> >
                a11(n_new,inner), a12(n_new,inner), a21(n_new,inner), a22(n_new,inner),
                b11(n_new,inner), b12(n_new,inner), b21(n_new,inner), b22(n_new,inner),
                c11(n_new,inner), c12(n_new,inner), c21(n_new,inner), c22(n_new,inner),
                p1(n_new,inner), p2(n_new,inner), p3(n_new,inner), p4(n_new,inner),
                p5(n_new,inner), p6(n_new,inner), p7(n_new,inner),
                aResult(n_new,inner), bResult(n_new,inner);

        int i, j;

        for (i = 0; i < n_new; i++) {
            for (j = 0; j < n_new; j++) {
                a11[i][j] = A[i][j];
                a12[i][j] = A[i][j + n_new];
                a21[i][j] = A[i + n_new][j];
                a22[i][j] = A[i + n_new][j + n_new];

                b11[i][j] = B[i][j];
                b12[i][j] = B[i][j + n_new];
                b21[i][j] = B[i + n_new][j];
                b22[i][j] = B[i + n_new][j + n_new];
            }
        }

        sum(a11, a22, aResult, n_new); // a11 + a22
        sum(b11, b22, bResult, n_new); // b11 + b22
        strassenRecursive(aResult, bResult, p1, n_new); // p1 = (a11+a22) * (b11+b22)

        sum(a21, a22, aResult, n_new); // a21 + a22
        strassenRecursive(aResult, b11, p2, n_new); // p2 = (a21+a22) * (b11)

        subtract(b12, b22, bResult, n_new); // b12 - b22
        strassenRecursive(a11, bResult, p3, n_new); // p3 = (a11) * (b12 - b22)

        subtract(b21, b11, bResult, n_new); // b21 - b11
        strassenRecursive(a22, bResult, p4, n_new); // p4 = (a22) * (b21 - b11)

        sum(a11, a12, aResult, n_new); // a11 + a12
        strassenRecursive(aResult, b22, p5, n_new); // p5 = (a11+a12) * (b22)

        subtract(a21, a11, aResult, n_new); // a21 - a11
        sum(b11, b12, bResult, n_new); // b11 + b12
        strassenRecursive(aResult, bResult, p6, n_new); // p6 = (a21-a11) * (b11+b12)

        subtract(a12, a22, aResult, n_new); // a12 - a22
        sum(b21, b22, bResult, n_new); // b21 + b22
        strassenRecursive(aResult, bResult, p7, n_new); // p7 = (a12-a22) * (b21+b22)

        sum(p3, p5, c12, n_new); // c12 = p3 + p5
        sum(p2, p4, c21, n_new); // c21 = p2 + p4

        sum(p1, p4, aResult, n_new); // p1 + p4
        sum(aResult, p7, bResult, n_new); // p1 + p4 + p7
        subtract(bResult, p5, c11, n_new); // c11 = p1 + p4 - p5 + p7

        sum(p1, p3, aResult, n_new); // p1 + p3
        sum(aResult, p6, bResult, n_new); // p1 + p3 + p6
        subtract(bResult, p2, c22, n_new); // c22 = p1 + p3 - p2 + p6

        // Grouping the results obtained in a single matrix:
        for (i = 0; i < n_new ; i++) {
            for (j = 0 ; j < n_new ; j++) {
                C[i][j] = c11[i][j];
                C[i][j + n_new] = c12[i][j];
                C[i + n_new][j] = c21[i][j];
                C[i + n_new][j + n_new] = c22[i][j];
            }
        }
    }
}


int main (int argc, char* argv[]) {

    int flag = 0;
    string filename = "/Users/binyan/Documents/Study/cs124/code/prog2/test.txt";
    int d = 4;

    if (argc==4) {
        char filename_buf[200];
        sscanf(argv[1], "%d", &flag);
        sscanf(argv[2], "%d", &d);
        sscanf(argv[3], "%s", filename_buf);
        filename = string(filename_buf);
    }

    int threshold = 2;
    vector<int> inner (d);
    vector< vector<int> > A(d, inner), B(d, inner), C(d, inner);

    readFile (filename, d, A, B);

    printMatrix(A, d);
    printMatrix(B, d);
    conventional(A, B, C, d);
    printMatrix(C, d);

    return 0;
}
