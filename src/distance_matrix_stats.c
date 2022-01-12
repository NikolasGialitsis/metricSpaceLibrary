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


int main (int argc, char **argv) { 

   Index S;
   DIR* d;
   struct dirent *dir;

   if (argc < 2)
   { fprintf (stderr,"Usage: %s dmat_directory\n",argv[0]);
   exit(1);
   }
   char* finame = argv[1];
   printf("dirname %s\n",finame);
   d = opendir(finame);
   if (d){
      while((dir = readdir(d)) != NULL){

        if(dir->d_type != DT_REG)continue;
         char* fullpath = malloc(sizeof(char)*(strlen(finame) + strlen(dir->d_name)) + 1);
         for(int i = 0 ; i < strlen(finame) ; i++){
            fullpath[i] = finame[i];
         }
         for(int j = 0 ; j < strlen(dir->d_name) ; j++){
            fullpath[strlen(finame) + j] = dir->d_name[j];
         }
         fullpath[strlen(finame) + strlen(dir->d_name)] = '\0';
         printf("openning %s\n",fullpath);
         DistMat* DM = decerialize(fullpath, 1);
         print_stats_distance_matrix(DM);
         free(fullpath);
      }
      closedir(d);
   }
   return 0;
 }

