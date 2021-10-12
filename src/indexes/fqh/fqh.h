/**
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.
**/


#include "../../index.h"
#include "../../bucket.h"

typedef struct sfqnode
   { union
        { struct
             { void *bucket;
               int size;
             } hoja;
          struct
             { void *children;
             } interno;
        } u;
   } fqvpnode;

typedef struct
   { Tdist dist;  /* [dist,dist sgte) */
     fqvpnode child;
   } Tchild;

typedef struct
   { Obj query;
     Tdist dist;
   } query;

typedef struct
   { query *queries;
     int height;
     int arity, n;
     fqvpnode node;
     char *descr;
   } fqvpt;

#define child(node,i) (((Tchild*)((node)->u.interno.children))[i])

typedef struct
   { Obj obj;
     Tdist dist;
   } Tod;

