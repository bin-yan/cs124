#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <math.h>
#include <random>
#include <fstream>
#include <cstdint>
#include <sstream>


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
    int64_t B[n], C[n];

    int64_t residue, residue_current, residue_check;
    int j, k;
    float prob;

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

        prob = rand()/RAND_MAX;

        if(residue_current < residue) {
            residue = residue_current;
            copySmallArray(p_current, p, n);
        } else if (prob <= (float)exp(-(residue_current - residue)/coolingFactor(i))){
            residue = residue_current;
            copySmallArray(p_current, p, n);
        } else {
            copySmallArray(p, p_current, n);
        }

        if(residue < residue_check) {
            residue_check = residue;
        }

        //cout<<residue_current<<","<<residue<<endl;

    }

    return residue_check;

}

void generateRandomSequence(int64_t A[], int n) {
    for (int i=0; i < n; i++) {
        //A[i] = rand() % 100 + 1;
        A[i] = ((RAND_MAX + (long)1)*(long)rand() + rand()) % (long)pow(10,12) + 1;
    }
}

void readFile(string filename, int64_t A[], int n) {
    string line;
    std::fstream fs;
    fs.open (filename, std::fstream::in);

    int64_t a;

    for (int i=0; i < n; i++) {
            getline(fs, line);
            istringstream iss(line);
            iss >> a;
            A[i] = a;
    }


    fs.close();
}

void experimentVersion(){

    int n = 100, iterations = 30000, experiments = 50;
    int64_t	A[n], KK[n], RR[n], HC[n], SA[n];
    int64_t residue_KK[experiments], residue_RR[experiments], residue_HC[experiments], residue_SA[experiments];
    int64_t residue_RR_prepartition[experiments], residue_HC_prepartition[experiments], residue_SA_prepartition[experiments];

    for(int exp = 0; exp < experiments; exp++) {

        generateRandomSequence(A, n);

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

        residue_SA_prepartition[exp] = simulatedAnnealing_prepartition(A, n, iterations);


    }

    // print results
    cout << "Karmarkar-Karp: Mean , Std | " << meanArray(residue_KK, experiments) << "," << stdArray(residue_KK, experiments) << endl;
    cout << "Standard representation: repeated random: Mean , Std | " << meanArray(residue_RR, experiments) << "," << stdArray(residue_RR, experiments) << endl;
    cout << "Standard representation: hill climbing: Mean , Std | " << meanArray(residue_HC, experiments) << "," << stdArray(residue_HC, experiments) << endl;
    cout << "Standard representation: simulated annealing: Mean , Std | " << meanArray(residue_SA, experiments) << "," << stdArray(residue_SA, experiments) << endl;

    cout << "Prepartitioning: repeated random: Mean , Std | " << meanArray(residue_RR_prepartition, experiments) << "," << stdArray(residue_RR_prepartition, experiments) << endl;
    cout << "Prepartitioning: hill climbing: Mean , Std | " << meanArray(residue_HC_prepartition, experiments) << "," << stdArray(residue_HC_prepartition, experiments) << endl;
    cout << "Prepartitioning: simulated annealing: Mean , Std | " << meanArray(residue_SA_prepartition, experiments) << "," << stdArray(residue_SA_prepartition, experiments) << endl;

    ofstream myfile ("/Users/byan/Desktop/results.txt");
    if (myfile.is_open())
    {
        for (int i=0; i < experiments; i++)
            myfile << residue_KK[i] <<"," << residue_RR[i] <<"," << residue_HC[i] <<"," << residue_SA[i] <<","
            << residue_RR_prepartition[i] <<"," << residue_HC_prepartition[i] <<"," << residue_SA_prepartition[i] << endl;
        myfile.close();
    }

}

void submittedVersion(string filename){

    int n = 100, iterations = 30000;
    int64_t	A[n], KK[n], RR[n], HC[n], SA[n];
    int64_t residue_KK, residue_RR, residue_HC, residue_SA,
            residue_RR_prepartition, residue_HC_prepartition, residue_SA_prepartition;

    /*
    // generate input file
    generateRandomSequence(A, n);
    ofstream myfile ("/Users/byan/Desktop/input.txt");
    if (myfile.is_open())
    {
        for (int i=0; i < n; i++)
            myfile << A[i] << endl;
        myfile.close();
    }
     */

    //readFile("/Users/byan/Desktop/input.txt", A, n);
    readFile(filename, A, n);

    copyArray(A, KK, n);
    residue_KK = KarmarkarKarp(KK, n);

    copyArray(A, RR, n);
    residue_RR = repeatedRandom(RR, n, iterations);

    copyArray(A, HC, n);
    residue_HC = hillClimbing(HC, n, iterations);

    copyArray(A, SA, n);
    residue_SA = simulatedAnnealing(SA, n, iterations);

    residue_RR_prepartition = repeatedRandom_prepartition(A, n, iterations);
    residue_HC_prepartition = hillClimbing_prepartition(A, n, iterations);
    residue_SA_prepartition = simulatedAnnealing_prepartition(A, n, iterations);

    cout << "Karmarkar-Karp: " << residue_KK << endl;
    cout << "Standard representation: repeated random: " << residue_RR << endl;
    cout << "Standard representation: hill climbing: " << residue_HC << endl;
    cout << "Standard representation: simulated annealing: " << residue_SA << endl;
    cout << "Prepartitioning: repeated random: " << residue_RR_prepartition << endl;
    cout << "Prepartitioning: hill climbimng: " << residue_HC_prepartition << endl;
    cout << "Prepartitioning: simulated annealing: " << residue_SA_prepartition << endl;

}


int main(int argc, char* argv[]) {

    srand((unsigned)time(NULL));

    string filename;
    if (argc==2) {
        char filename_buf[200];
        sscanf(argv[1], "%s", filename_buf);
        filename = string(filename_buf).c_str();

        submittedVersion(filename);
    } else
        experimentVersion();


    return 0;
}