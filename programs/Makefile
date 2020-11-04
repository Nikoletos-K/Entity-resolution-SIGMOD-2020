entityResolution_OBJS = EntityResolution.o utils.o json.o $(LIB)/project
# io_tests_OBJS = io_test ...

entityResolution_ARGS = -jd ./../data/camera_specs -csv ./../data/sigmod_medium_labelled_dataset.csv


include ../common.mk

.PHONY: $(LIB)/project
$(LIB)/project:
	$(MAKE) -C $(LIB) project
