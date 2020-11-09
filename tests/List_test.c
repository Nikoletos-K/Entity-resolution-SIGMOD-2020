#include "./../include/acutest.h"			

#include "./../iclude/list.h"


void test_create(void) {

	List *list = createList();

	TEST_ASSERT(list != NULL);
	TEST_ASSERT(list->firstNode == NULL);
	TEST_ASSERT(list->lastNode == NULL);
	TEST_ASSERT(list->numOfNodes == 0);

	deleteList(list);
}

void test_createNode(void) {

	int data = 10; 
	listNode * node = create_listNode((void*) &data);

	TEST_ASSERT(node != NULL);

	free(node);
}

void test_insert(void) {
	
	List *list = createList();
	
	int N = 1000;
	int* array = malloc(N * sizeof(*array));					

	for (int i = 0; i < N; i++) {

		insert_toList(list,(void *)  &array[i]);
		
		TEST_ASSERT(list->numOfNodes == (i + 1));	

		TEST_ASSERT(list->firstNode->data == &array[i]);	
	}

	// Ελέγχουμε εάν τα στοιχεία έχουν μπει με την αντίστροφη σειρά
	ListNode node = list->firstNode;

	for (int i = N - 1; i >= 0; i--) {
		TEST_ASSERT(node->data == &array[i]);
		node = node->next;
	}

	deleteList(list);
	free(array);
}

int compare_ints(Pointer a, Pointer b) {
	return *(int*)a - *(int*)b;
}

void test_find() {
	List *list = createList();
	int N = 1000;
	int* array = malloc(N * sizeof(*array));					

	for (int i = 0; i < N; i++) {
		array[i] = i;
		insert_toList(list,&array[i]);
	}

	// Εύρεση όλων των στοιχείων
	for (int i = 0; i < N; i++) {
		int* value = findValue(list, &i, compare_ints); 
		TEST_ASSERT(value == &array[i]);
	}

	int not_exists = -1;
	TEST_ASSERT(findValue(list,(void*) &not_exists, compare_ints) == NULL);

	deleteList(list);
	free(array);
}

void test_one_node_list(){
	List *list = createList();

	int data = 10;
	insert_toList(list,(void*)data);
	TEST_ASSERT(list->numOfNodes==1);
	TEST_ASSERT(oneNodeList(list) == 1);

	int data2 = 12;
	insert_toList(list,(void*)data2);
	TEST_ASSERT(oneNodeList(list) != 1);
}

TEST_LIST = {
	{ "createList", test_create },
	{ "insert_toList", test_insert },
	{ "findValue", test_find },
	{ "oneNodeList",test_one_node_list}
	{ NULL, NULL } 
};