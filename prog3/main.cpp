#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <math.h>
#include <random>


using namespace std;

void printA(int64_t A[], int n){
    for (int i=0; i < n; i++) {
        cout << A[i] << ",";
    }
    cout << endl;
}

void copyArray(int64_t A[], int64_t B[], int n){
    for (int i=0; i < n; i++) {
        B[i] = A[i];
    }
}

int64_t sumArray(int64_t A[], int n){
    int64_t sum = 0;

    for (int i=0; i < n; i++) {
        sum = sum + A[i];
    }

    return sum;
}

void get_max(int64_t A[], int n, int* max, int* nextmax, int* index, int *nextIndex) {

    *index = *nextIndex = -1;
    *max = *nextmax = 0;
    for (int i = 0; i < n; i++) {
        if (A[i] > *max) {
            *nextmax = *max;
            *nextIndex = *index;
            *max = A[i];
            *index = i;
        } else if (A[i] > *nextmax) {
            *nextmax = A[i];
            *nextIndex = i;
        }
    }
}

int KarmarkarKarp(int64_t A[], int n) {
    int residue;

    int	max, nextmax, index, nextIndex;
    while (1) {
        get_max(A, n, &max, &nextmax, &index, &nextIndex);
        if (nextmax == 0)
        {
            residue = max;
            break;
        }
        A[nextIndex] = 0;
        A[index] = max - nextmax;

        //printA(A, n);
    }
    return residue;
}

int64_t repeatedRandom(int64_t A[], int n, int iterations) {
    int64_t B[n];
    copyArray(A,B,n);

    int64_t residue, residue_current;

    residue = abs(sumArray(B, n));

    for (int i = 0; i < iterations; i++) {

        for (int j = 0; j < n; j++) {
            if ((rand() % 2) == 0)
                B[j] = B[j] * -1;
        }

        residue_current = abs(sumArray(B,n));

        if(residue_current < residue) {
            residue = residue_current;
            copyArray(B, A, n);
        }
    }

    return residue;
}

int64_t hillClimbing(int64_t A[], int n, int iterations) {
    int64_t B[n];
    copyArray(A,B,n);

    int64_t residue, residue_current;
    int j, k;

    residue = abs(sumArray(B, n));

    for (int i = 0; i < iterations; i++) {

        j = rand() % n;
        k = rand() % n;

        while(j==k)
            k = rand() % n;

        B[j] = B[j] * -1;
        if ((rand() % 2) == 0) {
            B[k] = B[k] * -1;
        }

        residue_current = abs(sumArray(B,n));

        if(residue_current < residue) {
            residue = residue_current;
            copyArray(B, A, n);
        }

        //cout <<j << "," << k <<"," << residue << "," << residue_current << ",";
        //printA(A,n);

    }

    return residue;
}

float coolingFactor(int i){
    float c = pow(10,10) * pow(0.8, floor(i / 300));
    return c;
}

int64_t simulatedAnnealing(int64_t A[], int n, int iterations) {
    int64_t B[n], C[n];
    copyArray(A,B,n);
    copyArray(A,C,n);

    int64_t residue, residue_current, residue_check;
    int j, k;
    float p;

    residue = abs(sumArray(A, n));
    residue_check = abs(sumArray(A, n));

    for (int i = 0; i < iterations; i++) {

        j = rand() % n;
        k = rand() % n;

        while(j==k)
            k = rand() % n;

        B[j] = B[j] * -1;
        if ((rand() % 2) == 0) {
            B[k] = B[k] * -1;
        }

        residue_current = abs(sumArray(B,n));

        p = rand()/RAND_MAX;

        if(residue_current < residue) {
            residue = residue_current;
            copyArray(B, C, n);
        } else if (p <= (float)exp(-(residue_current - residue)/coolingFactor(i))) {
            //if(i>20000)
            //    cout << (float)exp(-(residue_current - residue)/coolingFactor(i)) <<endl;
            residue = residue_current;
            copyArray(B, C, n);
        }

        if(residue < residue_check) {
            residue_check = residue;
            copyArray(C, A, n);
        }


        //cout <<j << "," << k <<"," << residue << "," << residue_check << ",";
        //printA(A,n);

    }

    return residue_check;
}



int main() {

    // Generate set A with n integers
    srand((unsigned)time(NULL));
    int n = 100;
    int64_t	A[n], KK[n], RR[n], HC[n], SA[n];
    int64_t residue_KK, residue_RR, residue_HC, residue_SA;

    copyArray(A, KK, n);
    copyArray(A, RR, n);
    copyArray(A, HC, n);
    copyArray(A, SA, n);


    for (int i=0; i < n; i++) {
        //A[i] = rand() % 50 + 1;
        A[i] = ((RAND_MAX + (long)1)*(long)rand() + rand()) % (long)pow(10,12) + 1;
    }

    //printA(A, n);

    residue_KK = KarmarkarKarp(KK, n);
    residue_RR = repeatedRandom(RR, n, 25000);
    residue_HC = hillClimbing(HC, n, 25000);
    residue_SA = simulatedAnnealing(SA, n, 25000);

    // print results
    //printA(A, n);
    cout << residue_KK << endl;
    cout << residue_RR << endl;
    cout << residue_HC << endl;
    cout << residue_SA << endl;


    return 0;
}