#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <cmath>

using namespace std;

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

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            C[i][j] = A[i][j] + B[i][j];
        }
    }
}

void subtract(vector< vector<int> > &A,
              vector< vector<int> > &B,
              vector< vector<int> > &C, int n) {

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            C[i][j] = A[i][j] - B[i][j];
        }
    }
}



void strassenRecursive(vector< vector<int> > &A,
               vector< vector<int> > &B,
               vector< vector<int> > &C, int n, int threshold) {
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
                p5(n_new,inner), p6(n_new,inner), p7(n_new,inner);

        for (int i = 0; i < n_new; i++) {
            for (int j = 0; j < n_new; j++) {
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

        vector< vector<int> > temp1(n_new,inner);
        subtract(b12, b22, temp1, n_new); // b12 - b22
        strassenRecursive(a11, temp1, p1, n_new, threshold); // p3 = (a11) * (b12 - b22)

        vector< vector<int> > temp2(n_new,inner);
        sum(a11, a12, temp2, n_new); // a11 + a12
        strassenRecursive(temp2, b22, p2, n_new, threshold); // p5 = (a11+a12) * (b22)

        vector< vector<int> > temp3(n_new,inner);
        sum(a21, a22, temp3, n_new); // a21 + a22
        strassenRecursive(temp3, b11, p3, n_new, threshold); // p2 = (a21+a22) * (b11)

        vector< vector<int> > temp4(n_new,inner);
        subtract(b21, b11, temp4, n_new); // b21 - b11
        strassenRecursive(a22, temp4, p4, n_new, threshold); // p4 = (a22) * (b21 - b11)

        vector< vector<int> > temp5(n_new,inner), temp6(n_new,inner);
        sum(a11, a22, temp5, n_new); // a11 + a22
        sum(b11, b22, temp6, n_new); // b11 + b22
        strassenRecursive(temp5, temp6, p5, n_new, threshold); // p1 = (a11+a22) * (b11+b22)

        vector< vector<int> > temp7(n_new,inner), temp8(n_new,inner);
        subtract(a12, a22, temp7, n_new); // a12 - a22
        sum(b21, b22, temp8, n_new); // b21 + b22
        strassenRecursive(temp7, temp8, p6, n_new, threshold); // p7 = (a12-a22) * (b21+b22)

        vector< vector<int> > temp9(n_new,inner), temp10(n_new,inner);
        subtract(a11, a21, temp9, n_new); // a21 - a11
        sum(b11, b12, temp10, n_new); // b11 + b12
        strassenRecursive(temp9, temp10, p7, n_new, threshold); // p6 = (a11-a21) * (b11+b12)

        vector< vector<int> > temp11(n_new,inner), temp12(n_new,inner);
        sum(p5, p4, temp11, n_new); // p5 + p4
        sum(temp11, p6, temp12, n_new); // p5 + p4 + p6
        subtract(temp12, p2, c11, n_new); // c11 = p5 + p4 + p6 - p2

        /*
        vector< vector<int> > temp1(n_new,inner), temp2(n_new,inner);
        sum(a11, a22, temp1, n_new); // a11 + a22
        sum(b11, b22, temp2, n_new); // b11 + b22
        strassenRecursive(temp1, temp2, p1, n_new, threshold); // p1 = (a11+a22) * (b11+b22)

        vector< vector<int> > temp3(n_new,inner);
        sum(a21, a22, temp3, n_new); // a21 + a22
        strassenRecursive(temp3, b11, p2, n_new, threshold); // p2 = (a21+a22) * (b11)

        vector< vector<int> > temp4(n_new,inner);
        subtract(b12, b22, temp4, n_new); // b12 - b22
        strassenRecursive(a11, temp4, p3, n_new, threshold); // p3 = (a11) * (b12 - b22)

        vector< vector<int> > temp5(n_new,inner);
        subtract(b21, b11, temp5, n_new); // b21 - b11
        strassenRecursive(a22, temp5, p4, n_new, threshold); // p4 = (a22) * (b21 - b11)

        vector< vector<int> > temp6(n_new,inner);
        sum(a11, a12, temp6, n_new); // a11 + a12
        strassenRecursive(temp6, b22, p5, n_new, threshold); // p5 = (a11+a12) * (b22)

        vector< vector<int> > temp7(n_new,inner), temp8(n_new,inner);
        subtract(a21, a11, temp7, n_new); // a21 - a11
        sum(b11, b12, temp8, n_new); // b11 + b12
        strassenRecursive(temp7, temp8, p6, n_new, threshold); // p6 = (a21-a11) * (b11+b12)

        vector< vector<int> > temp9(n_new,inner), temp10(n_new,inner);
        subtract(a12, a22, temp9, n_new); // a12 - a22
        sum(b21, b22, temp10, n_new); // b21 + b22
        strassenRecursive(temp9, temp10, p7, n_new, threshold); // p7 = (a12-a22) * (b21+b22)

        sum(p3, p5, c12, n_new); // c12 = p3 + p5
        sum(p2, p4, c21, n_new); // c21 = p2 + p4

        vector< vector<int> > temp11(n_new,inner), temp12(n_new,inner);
        sum(p1, p4, temp11, n_new); // p1 + p4
        sum(temp11, p7, temp12, n_new); // p1 + p4 + p7
        subtract(temp12, p5, c11, n_new); // c11 = p1 + p4 - p5 + p7

        vector< vector<int> > temp13(n_new,inner), temp14(n_new,inner);
        sum(p1, p3, temp13, n_new); // p1 + p3
        sum(temp13, p6, temp14, n_new); // p1 + p3 + p6
        subtract(temp14, p2, c22, n_new); // c22 = p1 + p3 - p2 + p6
         */

        // Grouping the results obtained in a single matrix:
        for (int i = 0; i < n_new ; i++) {
            for (int j = 0 ; j < n_new ; j++) {
                C[i][j] = c11[i][j];
                C[i][j + n_new] = c12[i][j];
                C[i + n_new][j] = c21[i][j];
                C[i + n_new][j + n_new] = c22[i][j];
            }
        }
    }
}

void strassenPadded(vector< vector<int> > &A,
              vector< vector<int> > &B,
              vector< vector<int> > &C, int n, int threshold) {

    int n_pow2 = pow(2, int(ceil(log2(n))));
    vector<int> inner(n_pow2);
    vector< vector<int> > A_pad(n_pow2, inner), B_pad(n_pow2, inner), C_pad(n_pow2, inner);

    for(int i=0; i<n; i++) {
        for (int j=0; j<n; j++) {
            A_pad[i][j] = A[i][j];
            B_pad[i][j] = B[i][j];
        }
    }

    strassenRecursive(A_pad, B_pad, C_pad, n_pow2, threshold);
    for(int i=0; i<n; i++) {
        for (int j=0; j<n; j++) {
            C[i][j] = C_pad[i][j];
        }
    }
}

int main (int argc, char* argv[]) {

    int flag = 0;
    //string filename = "/Users/byan/Documents/2016 Spring/cs124/code/prog2/test.txt";
    //int d = 4;
    string filename;
    int d;

    if (argc==4) {
        printf("test");
        char filename_buf[200];
        sscanf(argv[1], "%d", &flag);
        sscanf(argv[2], "%d", &d);
        sscanf(argv[3], "%s", filename_buf);
        filename = string(filename_buf).c_str();
        printf("%s", filename.c_str());
    }

    int threshold = 1;
    vector<int> inner (d);
    vector< vector<int> > A(d, inner), B(d, inner), C(d, inner);

    readFile (filename, d, A, B);

    strassenPadded(A, B, C, d, threshold);

    printMatrix(A, d);
    printMatrix(B, d);
    //conventional(A, B, C, d);
    printMatrix(C, d);

    return 0;
}
