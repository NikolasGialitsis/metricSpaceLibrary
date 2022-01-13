#include <sys/types.h>
#include <sys/stat.h>
#include <sys/times.h>
#include "obj.h"
#include "index.h"
#include <time.h>
#include <unistd.h>
#include "basics.h"
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


   if (argc < 4)
   { fprintf (stderr,"Usage: %s idxfile queriesfile output_file\n",argv[0]);
   exit(1);
   }
   FILE* fpt = fopen(argv[3], "w+");
   fprintf(fpt,"id,BestDistance,visitedLeaves,numDistances\n");
   fprintf (fpt,"%s idxfile queriesfile output_file\n",argv[0]);
   fprintf(fpt,"%s,%s,%s,%s\n",argv[0],argv[1],argv[2],argv[3]);
   
   char* finame = argv[2];
  
   fprintf (stderr,"reading index...\n");
   S = loadIndex ( argv[1]);
   int dn = getCountDB();
   fprintf(fpt,"%d\n", dn);

   fprintf (stderr,"Openning: %s\n",finame);
   char** queries = getQueriesFromDB(finame);
  
   fprintf (stderr,"querying %d elements...\n",dn);

   stat (argv[1],&sdata);
   fprintf (stderr,"read %lli bytes\n",(long long)sdata.st_size);

   numDistances = 0;
   long long totalDistances = 0;
   bool fixed=false;
   int k = 1;
   time_t t;
   t = clock();
   maxLeavesToVisit = 50;
   for (int i = 1 ; i < dn+1; i++){
      
      Obj qry;
      int siz;
      qry = parseobj (queries[i]);
      currVisitedLeaves = 0;
      numDistances = 0;
      numQueries++;
      if (fixed){
         times(&t1);
         siz = search(S,qry,r,true);
         times(&t2);
         fprintf (stderr,"%i objects found\n",siz);
      } else { 
         times(&t1);
         fprintf(stderr,"%s\n", queries[i]);
         r = searchNN (S,qry,k,true);
         siz = k;
         times(&t2);
         fprintf (stderr,"kNNs found at distance %f , visited %d/%d leaves, computed %ld distances\n\n",r,currVisitedLeaves,maxLeavesToVisit,numDistances);
         fprintf(fpt,"%d,%f,%d,%ld\n",i, r, currVisitedLeaves, numDistances);
      }
      totalDistances += numDistances;
   }
   
   
   t = clock() - t;
   double time_taken = ((double)t)/CLOCKS_PER_SEC; 
   fprintf(fpt,"Total Distances: %ld\n", totalDistances);
   fprintf(fpt,"Average distance per query: %f\n", totalDistances/(float)numQueries);
   fprintf(fpt, "Queried %d elements in %f seconds\n", numQueries, time_taken);
   fprintf (stderr,"freeing...\n");
   freeIndex (S,true);
   fprintf (stderr,"done\n");
   fclose(fpt);
   return 0;
 }

