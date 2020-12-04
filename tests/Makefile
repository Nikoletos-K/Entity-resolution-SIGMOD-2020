# Κάνοντας compile το <foo>_test.c με μια υλοποίηση <foo>.c του
# συγκεκριμένου τύπου, παράγουμε ένα tets για την υλοποίηση αυτή.

RBTree_test_OBJS = RBTree_test.o $(MODULES)/RBTree/RBTree.o
HashTable_test_OBJS = HashTable_test.o $(MODULES)/HashTable/HashTable.o $(MODULES)/RBTree/RBTree.o
DisJointSet_test_OBJS = DisJointSet_test.o $(MODULES)/DisJointSet/DisJointSet.o
List_test_OBJS = List_test.o $(MODULES)/List/list.o
Json_test_OBJS = 																									\
	Json_test.o ./../programs/EntityResolution/json.o ./../programs/EntityResolution/utils.o 		  		 \
	$(MODULES)/BitArray/bitArray.o 				 															\
	$(MODULES)/List/list.o 		 																		\
	$(MODULES)/DisJointSet/DisJointSet.o 																			\
	$(MODULES)/HashTable/HashTable.o  																		\
	$(MODULES)/RBTree/RBTree.o

# Ο βασικός κορμός του Makefile
include ../common.mk
