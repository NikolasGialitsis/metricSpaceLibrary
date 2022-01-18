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

const int MAX_MEMORY_GRAPHS = 2000;
void compute_line_by_line_document_distances(char** doc1, char** doc2, int ndocs, FILE* fpt){
   if(2* ndocs < MAX_MEMORY_GRAPHS){
      make_cache_graphs(doc1, ndocs);
	   make_cache_graphs(doc2, ndocs);
      double dist;
	   for(int i = 1 ; i < ndocs+1; i++){
		   dist = ngg_dissimilarity(i, i+ndocs);
		   fprintf(stderr, "%s\n%s\n%.4f\n\n",doc1[i],doc2[i],dist);
         fprintf(fpt, "%d,%f\n",i,dist);
	   }
      uncache_graphs(0, 2*ndocs);
   }
   else{
      double dist;
	   for(int i = 1 ; i < ndocs+1; i++){
         ngg_construct(0, doc1[i]);
         ngg_construct(1, doc2[i]);
         dist = ngg_dissimilarity(0,1);
		   fprintf(stderr, "%s\n%s\n%.4f\n\n",doc1[i],doc2[i],dist);
         fprintf(fpt, "%d,%f\n",i,dist);
         uncache_graphs(0,2);
	   }
   }

}

char* get_fullpath(char* dirname, char* finame){
      char* fullpath = malloc(sizeof(char)*(strlen(dirname) + strlen(finame)) + 1);
      for(int i = 0 ; i < strlen(dirname) ; i++){
         fullpath[i] = dirname[i];
      }
      for(int j = 0 ; j < strlen(finame) ; j++){
         fullpath[strlen(dirname) + j] = finame[j];
      }
      fullpath[strlen(finame) + strlen(dirname)] = '\0';
      return fullpath;
}

int main (int argc, char **argv) { 

   Index S;
   DIR* d;
   struct dirent *dir;

   if (argc < 3)
   { fprintf (stderr,"Usage: %s index dataset_directory output_directory\n",argv[0]);
   exit(1);
   }

   S = loadIndex ( argv[1]);
   int dn = getCountDB();
   char** reference_entries = getStringsFromDB();
   char* dataset_dir = argv[2];
   char* output_dir = argv[3];
   fprintf(stderr,"index %s\n",argv[1]);
   fprintf(stderr,"dataset dir %s\n",dataset_dir);
   fprintf(stderr,"output dir %s\n",output_dir);

   d = opendir(dataset_dir);
  
   if (d){
      while((dir = readdir(d)) != NULL){

        if(dir->d_type != DT_REG)continue;
         char* fullpath = get_fullpath(dataset_dir, dir->d_name);
         fprintf(stderr, "openning %s\n",fullpath);
         char** noisy_entries = getQueriesFromDB(fullpath);
         char* output_file = get_fullpath(output_dir, dir->d_name);
         FILE* fpt = fopen(output_file, "w+");
         fprintf(fpt,"id,noise\n"); 
         compute_line_by_line_document_distances(noisy_entries, reference_entries, dn, fpt );
         fclose(fpt);
         free(fullpath);
      }
      closedir(d);
   }
   return 0;
 }

