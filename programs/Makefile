entityResolution_OBJS = EntityResolution.o utils.o json.o $(LIB)/project.a

entityResolution_ARGS = -jd ./../data/camera_specs -csv ./../data/sigmod_medium_labelled_dataset.csv


include ../common.mk

.PHONY: $(LIB)/project.a
$(LIB)/project.a:
	$(MAKE) -C $(LIB) project.a
