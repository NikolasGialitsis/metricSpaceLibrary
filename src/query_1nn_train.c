#include <sys/types.h>
#include <sys/stat.h>
#include <sys/times.h>
#include "obj.h"
#include "index.h"
#include <time.h>
#include <unistd.h>

typedef struct sPalDB
   { char *pals;  /* words all together */
     char **ptrs;  /* pointers to each word */
     int npals;	  /* number of words (there is space for 1 more) */
     int csize;   /* allocated c size */
     int *c;
   } PalDB;


static PalDB DB;

#define db(p) (DB.ptrs[(int)p])


extern long long numDistances;
int main (int argc, char **argv) { 
   Index S;
   Tdist r;
   struct stat sdata;
   struct tms t1,t2;
   int numQueries = 0;


   if (argc < 3)
   { fprintf (stderr,"Usage: %s idxfile queriesfile\n",argv[0]);
   exit(1);
   }
   char* finame = argv[2];
   fprintf (stderr,"Openning: %s\n",finame);
   int n = openDB(finame);
   char** queries = getQueriesFromDB();
   fprintf (stderr,"querying %d elements...\n",n);


   fprintf (stderr,"reading index...\n");
   S = loadIndex ( argv[1]);
   stat (argv[1],&sdata);
   fprintf (stderr,"read %lli bytes\n",(long long)sdata.st_size);

   numDistances = 0;
   
   bool fixed=false;
   int k = 1;
   int numDeviations = 0;
   double sumDeviations = 0;
   double avgDeviations = 0;
   time_t t;
   t = clock();
   int report_after_n_steps = 500;
   for (int i = 1 ; i < n+1; i++){
      
      Obj qry;
      int siz;
      if(i % report_after_n_steps == 0){
         printf("computed %d distances\n",i);
         fprintf(stderr,"Total distances per query: %f\n", numDistances/(float)numQueries);
         fprintf(stderr,"Num Deviations: %d\n", numDeviations);
         fprintf(stderr,"Sum Deviations: %f\n", sumDeviations);
         if(sumDeviations > 0){
            fprintf(stderr,"Avg Deviations: %f\n", numDeviations/(float)sumDeviations);
         }
      }
      qry = parseobj (queries[i]);
      currVisitedLeaves = 0;
      numQueries++;
      if (fixed){
         times(&t1);
         siz = search(S,qry,r,true);
         times(&t2);
         fprintf (stderr,"%i objects found\n",siz);
      } else { 
         times(&t1);
         r = searchNN (S,qry,k,false);
         if(r > 0){
            numDeviations++;
            sumDeviations += r;
            fprintf (stderr,"%d: dist = %f\n",i, r);  
         }
         siz = k;
         times(&t2);
        
      }
      fprintf("query \"%s\" visited %d leaves in MVT\n",currVisitedLeaves);
   }
   
   t = clock() - t;
   double time_taken = ((double)t)/CLOCKS_PER_SEC; 
   fprintf(stderr,"Total distances per query: %f\n", numDistances/(float)numQueries);
   fprintf(stderr,"Num Deviations: %d\n", numDeviations);
   fprintf(stderr,"Sum Deviations: %f\n", sumDeviations);
   if(sumDeviations > 0){
      fprintf(stderr,"Avg Deviations: %f\n", numDeviations/(float)sumDeviations);
   }
   printf("Queried %d elements in %f seconds\n", n, time_taken);
   fprintf (stderr,"freeing...\n");
   freeIndex (S,true);
   fprintf (stderr,"done\n");
   return 0;
 }

