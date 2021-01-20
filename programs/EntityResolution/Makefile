# entityResolutionClique_OBJS = 				\
# 	EntityResolution_Cliques.o 				\
# 	utils.o 							\
# 	CamSpec.o 							\
# 	Clique.o 							\
# 	Vectorization.o 					\
# 	PairsImplementation.o 				\
# 	dictionary.o  						\
# 	$(LIB)/project.a

entityResolutionPairs_OBJS = 				\
	EntityResolution_Pairs.o 				\
	utils.o 							\
	CamSpec.o 							\
	Clique.o 							\
	Vectorization.o 					\
	PairsImplementation.o 				\
	dictionary.o  						\
	$(LIB)/project.a

# entityResolutionClique_ARGS = -jd ./../../data/camera_specs -csv ./../../data/sigmod_medium_labelled_dataset.csv
# entityResolutionClique_ARGS = -jd ./../../data/camera_specs -csv ./../../data/sigmod_large_labelled_dataset.csv


entityResolutionPairs_ARGS = -jd ./../../data/camera_specs -csv ./../../data/sigmod_medium_labelled_dataset.csv
# entityResolutionPairs_ARGS = -jd ./../../data/camera_specs -csv ./../../data/sigmod_large_labelled_dataset.csv


include ../../common.mk

.PHONY: $(LIB)/project.a
$(LIB)/project.a:
	$(MAKE) -C $(LIB) project.a
