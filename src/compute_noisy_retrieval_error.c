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
int main (int argc, char **argv) { 

   if (argc < 2)
   { fprintf (stderr,"Usage: %s  results_file reference_file \n",argv[0]);
   exit(1);
   }
   char* results_file = argv[1];
   char* reference_file = argv[2];
   ssize_t read;
   char* row = NULL;
   size_t len = 0;

   fprintf(stderr, "Results file:%s\nReference File:%s\n",results_file,reference_file);
   FILE* fp1 = fopen(results_file, "r");
   FILE* fp2 = fopen(reference_file, "r");
   for(int i = 0 ; i < 4 ; i++){
      getline(&row,&len,fp1);//skip headers
   }
   int n_queries = atoi(row);
   
   getline(&row,&len,fp2);

   float real_dist_sum = 0;
   float nn_dist_sum = 0;
   int misses = 0;
   float total_retrieval_error = 0;
   float outliers = 0;
   for (int i = 0 ; i < n_queries; i++){
        getline(&row, &len, fp1);
        int id = atoi(strtok(row,","));
        float nn_dist = atof(strtok(NULL,","));
        nn_dist_sum += nn_dist;

        getline(&row, &len, fp2);
        int id2 = atoi(strtok(row,","));
        float real_dist = atof(strtok(NULL,","));
        real_dist_sum += real_dist;
        assert(id==id2);
        fprintf(stderr,"%d,%f,%f\n",id, nn_dist, real_dist);
        if(nn_dist < real_dist){
           outliers += 1;
        }
        float retrieval_error = 0;
        if(nn_dist != real_dist){
           misses += 1;
           retrieval_error = fabs(nn_dist-real_dist);
           total_retrieval_error += retrieval_error;
        }
   }
   fprintf(stderr,"misses: %d out of %d (%.2f\%)\navg retrieval error: %f\ntotal retrieval error: %f\n",misses, n_queries, (100.0*misses)/n_queries, (1.0*misses)/n_queries, total_retrieval_error);
   fprintf(stderr,"avg_nn_dist: %f\navg_true_dist: %f\noutliers: %d\n",(1.0*nn_dist_sum)/n_queries, (1.0*real_dist_sum)/n_queries, outliers);
   fclose(fp1);
   fclose(fp2);
   if(row) 
      free(row);
   return 0;
 
 }

