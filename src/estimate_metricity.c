#include <sys/types.h>
#include <sys/stat.h>
#include <sys/times.h>
#include "obj.h"
#include "index.h"
#include <time.h>
#include <unistd.h>
#include "basics.h"
#include <dirent.h>
#include "/home/ngialitsis/search/NGramGraphParallel/c_wrapper/C_Interface.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "math.h"

typedef struct triplet{
    int x;
    int y;
    int z;
} triplet;

unsigned long long int factorial(int x){
    unsigned long long int fact = 1;
    for(int i = 1; i <= x; i++){
        fact *= i;
    }
    return fact;
}
unsigned long long int n_combinations_without_replacement(int n, int k){
    if (n == 500 && k == 3){
        return 20708500;
    }
    return factorial(n)/(factorial(k)*factorial(n-k));
}


triplet* generate_triplets(int* elements, int sample_size){
    unsigned long long int n_triplets = n_combinations_without_replacement(sample_size, 3);
    assert(n_triplets >= sample_size);
    // fprintf(stderr, "%llu triplets\n", n_triplets);
    triplet* triplets = malloc(sizeof(triplet)*n_triplets);
    unsigned long long  int num_triplets = 0;
    for(int i = 0 ; i < sample_size; i++){
        for(int j = i+1; j< sample_size; j++){
            for(int k = j+1; k < sample_size; k++){
                triplets[num_triplets].x = elements[i] + 1;
                triplets[num_triplets].y = elements[j] + 1;
                triplets[num_triplets].z = elements[k] + 1;
                num_triplets+=1;
            }
        }
    }
    // print_triplets(triplets, n_triplets);
    assert(n_triplets == num_triplets);
    return triplets;
}


void print_triplets(triplet* triplets, int n){
    for(int i = 0 ;  i < n ; i+=2000){
        fprintf(stderr, "(%d, %d, %d)\n",triplets[i].x, triplets[i].y, triplets[i].z);
    }
}



double fractional_power_t_base(double x, double w){
    if (w > 0){
        return pow(x,(1.0/(1+w)));
    }
    else{
        return pow(x,1-w);
    }
}

double get_t_base_modified_distance(int first_id, int second_id, double w){
    double dist = get_precomputed_distance_if_exists(first_id,second_id);
    return fractional_power_t_base( dist , w);
}


int is_euclidean(int x, int y, int z, double w){
    double dxy = get_t_base_modified_distance(x,y,w);
    double dyz = get_t_base_modified_distance(y,z,w);
    double dxz = get_t_base_modified_distance(x,z,w);  
    
    if ((dxy > dyz + dxz) ||
        (dyz > dxy + dxz) ||
        (dxz > dxy + dyz)){
            return 0;
    }
    return 1;
}

double get_non_triangles(char** objs, triplet* sample, int n_triplets, double w){
    
    double T_error = 0; 
    int nontriangles = 0;
    for(int i = 0 ; i < n_triplets; i++){
        if(!is_euclidean(sample[i].x,sample[i].y,sample[i].z, w)){
            nontriangles++;
        }
    }
    // fprintf(stderr, "nontriangles : %d\n",nontriangles);
    T_error = (nontriangles*1.0)/n_triplets;
    // fprintf(stderr, "w: %f T-error: %f\n",w, T_error);
    return nontriangles;
    
}



int main (int argc, char **argv) { 
    Index S;
    
    if (argc < 2){
        fprintf (stderr,"Usage: %s idxfile output_file\n",argv[0]);
        exit(1);
    }
    S = loadIndex (argv[1]);
    int dn = getCountDB();
    char** dbitems = getStringsFromDB();
    int sample_size = 500;
    int w = 0;
    double theta = 0.02;
    int n_iter = 1000;

    int* range = malloc(sizeof(int)*50000);
    for (int i = 0; i < 50000; i++)
    {  
        range[i] = i;
    } 
    int seed_of_seeds = 777;
    int* seeds = get_random_sample_indices(50000, n_iter, seed_of_seeds);

    for (int i = 0; i < n_iter; i++)
    {   
        fprintf(stderr, "iteration %d\n",i);
        int* random_indices = get_random_sample_indices(dn, sample_size, seeds[i]);
        triplet* sample = generate_triplets(random_indices, sample_size);
        
        unsigned long long int n_triplets = n_combinations_without_replacement(sample_size, 3);
        int nontriangles = get_non_triangles(dbitems, sample, n_triplets, w);
        // fprintf(stderr, "iteration %d triangle error %.2f",i,error);
        free(sample);
        free(random_indices);
        if (nontriangles > 0){
            fprintf(stderr, "is not metric\n");
            return 0;
        }
    }
    fprintf(stderr, "is metric\n");
 }

