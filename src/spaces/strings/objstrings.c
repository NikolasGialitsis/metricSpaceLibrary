
#include "../../obj.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include "/home/ngialitsis/search/NGramGraphParallel/c_wrapper/C_Interface.h"
#include <time.h>
#include <unistd.h>

typedef struct sPalDB
   { char *pals;  /* words all together */
     char **ptrs;  /* pointers to each word */
     int npals;	  /* number of words (there is space for 1 more) */
     int csize;   /* allocated c size */
     int *c;
   } PalDB;

static int never = 1;
static PalDB DB;

#define db(p) (DB.ptrs[(int)p])

Tdist distanceInter (Obj obj1, Obj obj2){
  Tdist dist = get_precomputed_distance_if_exists(obj1, obj2);
  if(dist >= 0) return dist;
  ngg_construct(1, db(obj2));
  dist = ngg_dissimilarity(0,1);
  printf("D(%.100s,%.100s)=%f\n",db(obj1),db(obj2),dist);
  uncache_graphs(1,1);
  return dist;
}


Obj parseobj (char *s)
   { 
     char *str = db(NewObj);
     if (str != NULL){
       printf("emptying string %s\n",str);
       free(str);
     }
    
     ngg_construct(NewObj, s);
     str = malloc (strlen(s)+1);
     strcpy (str,s);
     db(NewObj) = str;
     printf("parsed %s\n\n",db(NewObj));
     return NewObj; 
   }

void printobj (Obj obj)

   { printf ("%s\n",db(obj));
   }

int openDB (char *name)

   { char *ptr,*top;
     FILE *f;
     struct stat sdata;
	   unsigned long dn;
     closeDB ();
     f = fopen (name,"r");
     stat (name,&sdata);
     DB.pals = malloc (sdata.st_size);
     fread (DB.pals,sdata.st_size,1,f);
     fclose (f);
     ptr = DB.pals; top = ptr + sdata.st_size; 
     dn = 0;
     while (ptr < top) //count newlines (num of input sequences)
	{ while (*ptr != '\n') ptr++;
	  dn++; *ptr++ = 0;//change newline to string termination character
	}
      
     DB.ptrs = malloc ((dn+1)*sizeof(char*)); //storage for one additional ptr in DB
     dn = 0; ptr = DB.pals; 
     DB.ptrs[0] = NULL; //notice: skip extra ptr
     while (ptr < top)//Add ptrs to start of sequences to DB 
	{ DB.ptrs[++dn] = ptr;
	  while (*ptr++);
        }
     DB.npals = dn;

    
    if(access( objects_storage_file, R_OK ) == 0) {
      time_t t;
      t = clock();
      decerialize(objects_storage_file);
      t = clock() - t;
      double time_taken = ((double)t)/CLOCKS_PER_SEC;
      printf("cereal took %f seconds to deserialize distance matrix\n", time_taken);    
    } else {
      printf("could not find objects file %s\n",objects_storage_file);    
      exit(0);
      time_t t;
      t = clock();
      printf("DB.npals %d\n",DB.npals);
      DistMat* DM = ngg_compute_distance_matrix(DB.ptrs+1,DB.npals);
      t = clock() - t;
      double time_taken = ((double)t)/CLOCKS_PER_SEC; 
      printf("distance matrix took %f seconds to build \n", time_taken);
      cerealize(DM, objects_storage_file);

      t = clock() - t;
      time_taken = ((double)t)/CLOCKS_PER_SEC;
      printf("cereal took %f seconds to serialize distance matrix\n", time_taken);  
    }
    return DB.npals;
   }

void closeDB (void)

   { if (never) { never=0; DB.pals = NULL; }
     if (DB.pals == NULL) return;
     free (DB.pals); 
     free (DB.ptrs);
     free (DB.c); DB.csize = -1;
     DB.pals = NULL;
   }
