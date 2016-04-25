#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <math.h>
#include <random>
#include <fstream>
#include <cstdint>


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

void copySmallArray(int A[], int B[], int n){
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

int64_t meanArray(int64_t A[], int n){
    int64_t mean;
    mean = sumArray(A, n)/n;
    return mean;
}

int64_t stdArray(int64_t A[], int n){
    int64_t sum_std = 0;
    int64_t mean = meanArray(A, n);

    for(int i=0; i<n; i++) {
        sum_std= sum_std + (A[i]-mean) * (A[i]-mean);
        //cout << sum_std <<",";
    }


    return sqrt(sum_std/n);
}

void get_max(int64_t A[], int n, int64_t* max, int64_t* nextmax, int* index, int *nextIndex) {

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

int64_t KarmarkarKarp(int64_t A[], int n) {
    int64_t residue, max, nextmax;
    int index, nextIndex;
    while (1) {
        get_max(A, n, &max, &nextmax, &index, &nextIndex);
        if (nextmax == 0)
        {
            residue = max;
            break;
        }
        A[nextIndex] = 0;
        A[index] = max - nextmax;
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
        } else {
            copyArray(A, B, n);
        }

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
            residue = residue_current;
            copyArray(B, C, n);
        } else {
            copyArray(C, B, n);
        }

        if(residue < residue_check) {
            residue_check = residue;
            copyArray(C, A, n);
        }

    }

    return residue_check;
}

void generateRandomState(int p[], int n) {
    for (int i=0; i < n; i++) {
        p[i] = rand() % n;
    }
}

void generateNewSequence(int64_t A[], int64_t B[], int p[], int n){
    for (int i=0; i < n; i++)
        B[i] = 0;

    for (int j=0; j < n; j++)
        B[p[j]] = B[p[j]] + A[j];
}

int64_t KarmarkarKarpResidue(int64_t A[], int n) {
    int64_t C[n];
    copyArray(A, C, n);
    int64_t residue;
    residue = KarmarkarKarp(C,n);

    return residue;
}

int64_t repeatedRandom_prepartition(int64_t A[], int n, int iterations) {
    int64_t B[n];

    int64_t residue, residue_current;

    int p[n];
    generateRandomState(p,n);
    generateNewSequence(A, B, p, n);
    residue = KarmarkarKarpResidue(B,n);

    for (int i = 0; i < iterations; i++) {
        generateRandomState(p,n);
        generateNewSequence(A, B, p, n);
        residue_current = KarmarkarKarpResidue(B,n);

        if(residue_current < residue) {
            residue = residue_current;
        }
    }

    return residue;
}

int64_t hillClimbing_prepartition(int64_t A[], int n, int iterations) {
    int64_t B[n];

    int64_t residue, residue_current;
    int j, k;

    int p[n], p_current[n];
    generateRandomState(p,n);
    copySmallArray(p,p_current,n);
    generateNewSequence(A, B, p, n);
    residue = KarmarkarKarpResidue(B,n);

    for (int i = 0; i < iterations; i++) {

        j = rand() % n;
        k = rand() % n;

        while(p_current[j]==k)
            k = rand() % n;

        p_current[j] = k;

        generateNewSequence(A, B, p_current, n);
        residue_current = KarmarkarKarpResidue(B,n);

        if(residue_current < residue) {
            residue = residue_current;
            copySmallArray(p_current, p, n);
        } else {
            copySmallArray(p, p_current, n);
        }

        //cout<<residue_current<<","<<residue<<endl;

    }

    return residue;
}

int64_t simulatedAnnealing_prepartition(int64_t A[], int n, int iterations) {
    int64_t B[n];

    int64_t residue, residue_current;
    int j, k;

    int p[n], p_current[n];
    generateRandomState(p,n);
    copySmallArray(p,p_current,n);
    generateNewSequence(A, B, p, n);
    residue = KarmarkarKarpResidue(B,n);

    for (int i = 0; i < iterations; i++) {

        j = rand() % n;
        k = rand() % n;

        while(p_current[j]==k)
            k = rand() % n;

        p_current[j] = k;

        generateNewSequence(A, B, p_current, n);
        residue_current = KarmarkarKarpResidue(B,n);

        if(residue_current < residue) {
            residue = residue_current;
            copySmallArray(p_current, p, n);
        } else {
            copySmallArray(p, p_current, n);
        }

        //cout<<residue_current<<","<<residue<<endl;

    }

    return residue;

}



int main() {

    // Generate set A with n integers
    srand((unsigned)time(NULL));
    int n = 100, iterations = 1000, experiments = 1;
    int64_t	A[n], KK[n], RR[n], HC[n], SA[n];
    int64_t residue_KK[experiments], residue_RR[experiments], residue_HC[experiments], residue_SA[experiments];
    int64_t residue_RR_prepartition[experiments], residue_HC_prepartition[experiments], residue_SA_prepartition[experiments];

    for(int exp = 0; exp < experiments; exp++) {

        for (int i=0; i < n; i++) {
            //A[i] = rand() % 100 + 1;
            A[i] = ((RAND_MAX + (long)1)*(long)rand() + rand()) % (long)pow(10,12) + 1;
        }


        //printA(A, n);
        copyArray(A, KK, n);
        residue_KK[exp] = KarmarkarKarp(KK, n);

        copyArray(A, RR, n);
        residue_RR[exp] = repeatedRandom(RR, n, iterations);

        copyArray(A, HC, n);
        residue_HC[exp] = hillClimbing(HC, n, iterations);

        copyArray(A, SA, n);
        residue_SA[exp] = simulatedAnnealing(SA, n, iterations);

        residue_RR_prepartition[exp] = repeatedRandom_prepartition(A, n, iterations);
        residue_HC_prepartition[exp] = hillClimbing_prepartition(A, n, iterations);

    }

    // print results
    /*
    cout << meanArray(residue_KK, experiments) << "," << stdArray(residue_KK, experiments) << endl;
    cout << meanArray(residue_RR, experiments) << "," << stdArray(residue_RR, experiments) << endl;
    cout << meanArray(residue_HC, experiments) << "," << stdArray(residue_HC, experiments) << endl;
    cout << meanArray(residue_SA, experiments) << "," << stdArray(residue_SA, experiments) << endl;

    cout << meanArray(residue_RR_prepartition, experiments) << "," << stdArray(residue_RR_prepartition, experiments) << endl;
    cout << meanArray(residue_HC_prepartition, experiments) << "," << stdArray(residue_HC_prepartition, experiments) << endl;
*/

    ofstream myfile ("/Users/byan/Desktop/results.txt");
    if (myfile.is_open())
    {
        for (int i=0; i < experiments; i++)
            myfile << residue_KK[i] <<"," << residue_RR[i] <<"," << residue_HC[i] <<"," << residue_SA[i]<< endl;
        myfile.close();
    }

    return 0;
}