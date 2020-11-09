#include "./../include/acutest.h"			// Απλή βιβλιοθήκη για unit testing

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
	listNode * node = create_listNode();

	TEST_ASSERT(list != NULL);
	TEST_ASSERT(list->firstNode == NULL);
	TEST_ASSERT(list->lastNode == NULL);
	TEST_ASSERT(list->numOfNodes == 0);

	deleteList(list);
}

void test_insert(void) {
	
	List *list = createList();
	// Θα προσθέτουμε, μέσω της insert, δείκτες ως προς τα στοιχεία του π΄ίνακα
	int N = 1000;
	int* array = malloc(N * sizeof(*array));					

	for (int i = 0; i < N; i++) {

		// list_insert_next(list, LIST_BOF, &array[i]);

		insert_toList(list,(void *)  &array[i]);
		
		// Ελέγχουμε εάν ενημερώθηκε (αυξήθηκε) το μέγεθος της λίστας.
		TEST_ASSERT(list->numOfNodes == (i + 1));	

		// Ελέγχουμε εάν ο πρώτος κόμβος περιέχει σαν τιμή τον δείκτη που μόλις κάναμε insert								
		TEST_ASSERT(list->firstNode->data == &array[i]);	
	}

	// Ελέγχουμε εάν τα στοιχεία έχουν μπει με την αντίστροφη σειρά
	ListNode node = list->firstNode;

	for (int i = N - 1; i >= 0; i--) {
		TEST_ASSERT(node->data == &array[i]);
		node = node->next;
	}

	// Εισαγωγή σε ενδιάμεσο κόμβο: προσθέτουμε το NULL σαν δεύτερο κόμβο
	// ListNode first_node = list->firstNode;

	// list_insert_next(list, first_node, NULL);
	// TEST_ASSERT(list_node_value(list, list_next(list, first_node)) == NULL);

	deleteList(list);
	free(array);
}

// Σύγκριση δύο int pointers
int compare_ints(Pointer a, Pointer b) {
	return *(int*)a - *(int*)b;
}

void test_find() {
	List list = list_create(NULL);
	int N = 1000;
	int* array = malloc(N * sizeof(*array));					

	// Εισάγουμε δοκιμαστικές τιμές στον πίνακα, για να ελέγξουμε την test_find
	for (int i = 0; i < N; i++) {
		array[i] = i;
		insert_toList(list,&array[i]);
	}

	// Εύρεση όλων των στοιχείων
	for (int i = 0; i < N; i++) {
		int* value = findValue(list, &i, compare_ints); 
		TEST_ASSERT(value == &array[i]);
	}

	// Δοκιμάζουμε, για μια τυχαία τιμή που δεν μπορεί πιθανώς να υπάρχει στην λίστα,
	// αν η list_find γυρνάει σωστά NULL pointer
	int not_exists = -1;
	TEST_ASSERT(findValue(list, &not_exists, compare_ints) == NULL);

	deleteList(list);
	free(array);
}

// destroy
//
// Η destroy καλείται σε όλα τα tests, για να βρούμε αν δουλεύει σωστά τρέχουμε
//   make valgrind


// Λίστα με όλα τα tests προς εκτέλεση
TEST_LIST = {
	{ "createList", test_create },
	{ "insert_toList", test_insert },
	{ "findValue", test_find },
	{ NULL, NULL } // τερματίζουμε τη λίστα με NULL
};