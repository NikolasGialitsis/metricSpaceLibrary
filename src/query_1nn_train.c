#include <sys/types.h>
#include <sys/stat.h>
#include <sys/times.h>
#include "obj.h"
#include "index.h"
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

   for (int i = 1 ; i < n+1 ; i++){
      Obj qry;
      int siz;

      qry = parseobj (queries[i]);
      numQueries++;
      if (fixed){
         times(&t1);
         siz = search(S,qry,r,true);
         times(&t2);
         fprintf (stderr,"%i objects found\n",siz);
      } else { 
         times(&t1);
         r = searchNN (S,qry,k,false);
         siz = k;
         times(&t2);
         #ifdef CONT
                     fprintf (stderr,"kNNs at distance %f\n",r);
         #else
                     fprintf (stderr,"kNNs at distance %f\n",r);
         #endif
         }
      }
     fprintf(stderr,"Total distances per query: %f\n",
     numDistances/(float)numQueries);
     fprintf (stderr,"freeing...\n");
     freeIndex (S,true);
     fprintf (stderr,"done\n");
	   return 0;
   }

