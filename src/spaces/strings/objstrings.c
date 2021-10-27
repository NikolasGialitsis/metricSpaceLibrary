
#include "../../obj.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include "/home/ngialitsis/search/NGramGraphParallel/c_wrapper/C_Interface.h"

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

	/* edit distance */




static int ed (char *p1, char *p2)

    { 
	    
      register int pc,nc,j,i; 
      register char cc; 
      register int m = strlen(p1);
      int *c;
      if (DB.csize < m) 
	 { DB.csize = m; 
	   DB.c = realloc (DB.c,(m+1)*sizeof(int)); 
	 }
      c = DB.c;
      nc = m;
      p1--;
      for (j=0;j<=m;j++) c[j] = j;
      for (i=0;(cc=p2[i]);i++)
        { pc = i; nc = i+1; 
          for (j=1;j<=m;j++) 
            { if (c[j] < nc) nc = c[j];
              pc += (cc != p1[j]);
              if (pc <= nc) nc = pc; else nc++; 
              pc = c[j]; c[j] = nc; 
            } 
        } 
      return nc;
    }

// Tdist distanceInter (Obj obj1, Obj obj2)

//    { return ed (db(obj1),db(obj2));
//    }

Tdist distanceInter (Obj obj1, Obj obj2){
  Tdist dist =  ngg_dissimilarity(obj1,obj2);
  printf("Dist(%d,%d) : %.5f\n\n",(int) obj1, (int)obj2, dist);
  return dist;
}


Obj parseobj (char *s)
   { 
     char *str = db(NewObj);
     if (str != NULL) free(str);
     ngg_construct(NewObj, s);
     str = malloc (strlen(s)+1);
     strcpy (str,s);
     db(NewObj) = str;
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
     while (ptr < top) 
	{ while (*ptr != '\n') ptr++;
	  dn++; *ptr++ = 0;
	}
    
     DB.ptrs = malloc ((dn+1)*sizeof(char*));
     dn = 0; ptr = DB.pals;
     DB.ptrs[0] = NULL;
     while (ptr < top) 
	{ DB.ptrs[++dn] = ptr;
	  while (*ptr++);
        }
     DB.npals = dn;
     printf("DB.npals %d\n",DB.npals);
     ngg_construct_graph_database(DB.ptrs, DB.npals);
     printf("constructed graph database\n");
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
