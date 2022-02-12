C_WRAPPER_FOLDER = /home/ngialitsis/search/NGramGraphParallel/c_wrapper
C = gcc

all:  all-strings all-experiments stats

all-strings:  bin/build-mvp-strings bin/query-mvp-strings

all-experiments: bin/noisy_distances bin/query-experiment-a bin/retrieval_error bin/trigen

stats: bin/stats

clean: 
	rm -f bin/* lib/*.o lib/*/*.o lib/*/*/*.o
	
FLAGS = -O9 -Wall
CFLAGS = $(FLAGS) -DCONT -ggdb

# Common files


lib/basicsC.o: src/basics.c
	 $(C) $(CFLAGS) -c -o lib/basicsC.o  src/basics.c

lib/bucketC.o: src/bucket.c
	 $(C) $(CFLAGS) -c -o lib/bucketC.o  src/bucket.c

lib/buildC.o: src/build.c
	 $(C) $(CFLAGS) -c -o lib/buildC.o src/build.c

lib/queryC.o: src/query.c
	 $(C) $(CFLAGS) -c -o lib/queryC.o src/query.c

lib/queryExperimentA.o: src/query_1nn_train.c 
	 $(C) $(CFLAGS) -c -o lib/queryExperimentA.o src/query_1nn_train.c

lib/stats.o: src/distance_matrix_stats.c 
	 $(C) $(CFLAGS) -c -o lib/stats.o src/distance_matrix_stats.c

lib/noisy_distances.o: src/compute_noisy_nn_distances.c 
	$(C) $(CFLAGS) -c -o lib/noisy_distances.o src/compute_noisy_nn_distances.c

lib/retrieval_error.o: src/compute_noisy_retrieval_error.c 
	$(C) $(CFLAGS) -c -o lib/retrieval_error.o src/compute_noisy_retrieval_error.c

lib/trigen.o: src/trigen.c 
	$(C) $(CFLAGS) -c -o lib/trigen.o src/trigen.c


# INDEXES

lib/indexC-mvp.o: src/indexes/mvp/mvp.c
	 $(C) $(CFLAGS) -c -o lib/indexC-mvp.o src/indexes/mvp/mvp.c

######

# SPACES
lib/space-strings.o: src/spaces/strings/objstrings.c $(C_WRAPPER_FOLDER)/Request.c $(C_WRAPPER_FOLDER)/libStringSplitter.so $(C_WRAPPER_FOLDER)/libProximityApproach.so $(C_WRAPPER_FOLDER)/libNGramGraph.so $(C_WRAPPER_FOLDER)/libGraphSimilarity.so
	 $(C) $(CFLAGS) -c -o lib/space-strings.o src/spaces/strings/objstrings.c  -L$(C_WRAPPER_FOLDER) -lC_Interface  -lStringSplitter -lNGramGraph -lProximityApproach -lGraphSimilarity  -lStringSplitter -lNGramGraph -lProximityApproach -lGraphSimilarity

# PROGRAMS

bin/build-mvp-strings: lib/basicsC.o lib/bucketC.o lib/indexC-mvp.o lib/space-strings.o lib/buildC.o 
	$(C) $(CFLAGS) -o bin/build-mvp-strings lib/buildC.o  lib/basicsC.o lib/bucketC.o lib/indexC-mvp.o lib/space-strings.o -L$(C_WRAPPER_FOLDER) -lC_Interface  -lStringSplitter -lNGramGraph -lProximityApproach -lGraphSimilarity  -lStringSplitter -lNGramGraph -lProximityApproach -lGraphSimilarity 	-L$(C_WRAPPER_FOLDER) -lC_Interface  -lStringSplitter -lNGramGraph -lProximityApproach -lGraphSimilarity -lm 

bin/query-mvp-strings: lib/basicsC.o lib/bucketC.o lib/indexC-mvp.o lib/space-strings.o lib/queryC.o
	$(C) $(CFLAGS) -o bin/query-mvp-strings lib/queryC.o  lib/basicsC.o lib/bucketC.o lib/indexC-mvp.o lib/space-strings.o -L$(C_WRAPPER_FOLDER) -lC_Interface  -lStringSplitter -lNGramGraph -lProximityApproach -lGraphSimilarity  -lStringSplitter -lNGramGraph -lProximityApproach -lGraphSimilarity -lm 

bin/query-experiment-a: lib/basicsC.o lib/bucketC.o lib/indexC-mvp.o lib/space-strings.o lib/queryExperimentA.o
	$(C) $(CFLAGS) -o bin/query-experiment-a lib/queryExperimentA.o  lib/basicsC.o lib/bucketC.o lib/indexC-mvp.o lib/space-strings.o -L$(C_WRAPPER_FOLDER) -lC_Interface  -lStringSplitter -lNGramGraph -lProximityApproach -lGraphSimilarity  -lStringSplitter -lNGramGraph -lProximityApproach -lGraphSimilarity -lm 


bin/stats: lib/basicsC.o lib/bucketC.o lib/indexC-mvp.o lib/space-strings.o lib/stats.o
	$(C) $(CFLAGS) -o bin/stats lib/stats.o  lib/basicsC.o lib/bucketC.o lib/indexC-mvp.o lib/space-strings.o -L$(C_WRAPPER_FOLDER) -lC_Interface  -lStringSplitter -lNGramGraph -lProximityApproach -lGraphSimilarity  -lStringSplitter -lNGramGraph -lProximityApproach -lGraphSimilarity -lm 

bin/noisy_distances: lib/basicsC.o lib/bucketC.o lib/indexC-mvp.o lib/space-strings.o lib/noisy_distances.o
	$(C) $(CFLAGS) -o bin/noisy_distances lib/noisy_distances.o  lib/basicsC.o lib/bucketC.o lib/indexC-mvp.o lib/space-strings.o -L$(C_WRAPPER_FOLDER) -lC_Interface  -lStringSplitter -lNGramGraph -lProximityApproach -lGraphSimilarity  -lStringSplitter -lNGramGraph -lProximityApproach -lGraphSimilarity -lm 

bin/retrieval_error: lib/basicsC.o lib/bucketC.o lib/indexC-mvp.o lib/space-strings.o lib/retrieval_error.o
	$(C) $(CFLAGS) -o bin/retrieval_error lib/retrieval_error.o  lib/basicsC.o lib/bucketC.o lib/indexC-mvp.o lib/space-strings.o -L$(C_WRAPPER_FOLDER) -lC_Interface  -lStringSplitter -lNGramGraph -lProximityApproach -lGraphSimilarity  -lStringSplitter -lNGramGraph -lProximityApproach -lGraphSimilarity -lm 

bin/trigen: lib/basicsC.o lib/bucketC.o lib/indexC-mvp.o lib/space-strings.o lib/trigen.o
	$(C) $(CFLAGS) -o bin/trigen lib/trigen.o  lib/basicsC.o lib/bucketC.o lib/indexC-mvp.o lib/space-strings.o -L$(C_WRAPPER_FOLDER) -lC_Interface  -lStringSplitter -lNGramGraph -lProximityApproach -lGraphSimilarity  -lStringSplitter -lNGramGraph -lProximityApproach -lGraphSimilarity -lm 