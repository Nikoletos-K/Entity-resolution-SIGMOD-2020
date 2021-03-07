![run-tests](../../workflows/run-tests/badge.svg)
![](https://img.shields.io/badge/Language-%20C-blue)  

---

# Entity resolution system for Sigmod Programming Contest 2020
Entity Resolution is the problem of identifying and matching different manifestations of the same real-world object in a dataset. Ironically, Entity Resolution has many duplicate names in the literature, including Record Linkage, Deduplication, Approximate Match, Entity Clustering, and so on.

For this task we needed to identify which product specifications (in short, specs) from multiple e-commerce websites represent the same real-world product. All specs refer to cameras and include information about the camera model (e.g. Canon EOS 5D Mark II) and, possibly, accessories (e.g. lens kit, bag, tripod). The challenge is to develop an Entity Resolution system for matching the specs with the same camera models with high precision and recall.

Contest website: [ACM SIGMOD Programming Contest 2020](http://www.inf.uniroma3.it/db/sigmod2020contest/task.html)

## System functionality - Abstract
At the beginning of the program, the folder containing specs data is read
 and their information is stored in the appropriate structures.
Then, Dataset W is read and cliques and negative correlations are formed. The vocabulary is then created and the vectors for are calculated
each camera.
The data set is then formed with mixed positives and negatives
correlations and is divided into three parts - train 60%, test 20%, validation 20%. Afterward,
in each iteration the model is trained with the Mini-Batch GD algorithm
of Logistic regression using multi-threading. Model training is valued
through the test set and then the model is retrained with all its pairs
original Dataset that we do not know any correlation for them.
At the end of the iterations, a final estimate of the model is made through the
validation set. 


## System task

Our goal was to find all pairs of product specs in dataset X that match, that is, refer to the same real-world product. Our output is stored in a CSV file containing only the matching spec pairs found by your system. 

Example of output CSV file
```
left_spec_id, right_spec_id
www.ebay.com//10, www.ebay.com//20
www.ebay.com//10, buy.net//100
```

## Initial data

We are provided with a dataset including ~30k specs in JSON format, each spec containing a list of (attribute_name, attribute_value) pairs extracted from a different web page, collected across 24 different web sources. We will refer to this dataset as dataset X.

Each spec is stored as a file, and files are organized into directories, each directory corresponding to a different web source (e.g., www.alibaba.com).
All specs refer to cameras and include information about the camera model (e.g. Canon EOS 5D Mark II) and, possibly, accessories (e.g. lens kit, bag, tripod). Accessories do not contribute to product identification: for instance, a Canon EOS 5D Mark II that is sold as a bundle with a bag represents the same core product as a Canon EOS 5D Mark II that is sold alone.

### Example of product specification in JSON format

```
{
  "<page title>": "Samsung Smart WB50F Digital Camera White Price in India with Offers & Full Specifications | PriceDekho.com",
  "brand": "Samsung",
  "dimension": "101 x 68 x 27.1 mm",
  "display": "LCD 3 Inches",
  "pixels": "Optical Sensor Resolution (in MegaPixel)\n16.2 MP"
  "battery": "Li-Ion"
}
```

We provided also with a labelled dataset in CSV format, containing three columns: "left_spec_id", "right_spec_id" and "label". We will refer to this dataset as dataset W (which includes the previously released labelled dataset, referred to as dataset Y).

- The "spec_id" is a global identifier for a spec and consists of a relative path of the spec file. Note that instead of "/" the spec_id uses a special character "//" and that there is no extension. For instance, the spec_id "www.ebay.com//1000" refers to the 1000.json file inside the www.ebay.com directory. All "spec_id" in the labelled dataset W refer to product specs in dataset X. Thus, the dataset W provides labels for a subset of the product pairs in the Cartersian product of the specs dataset X with itself.

- Each row of the labelled dataset represents a pair of specifications. Label=1 means that the left spec and the right spec refer to the same real-world product (in short, that they are matching). Label=0 means that the left spec and the right spec refer to different real-world products (in short, that they are non-matching).


### Example of labelled dataset in CSV format
```
left_spec_id, right_spec_id, label
www.ebay.com//1, www.ebay.com//2, 1
www.ebay.com//3, buy.net//10, 0
```
## Data structures and algorithms used

-  __Generic list__: To create the clicks, a table of double-linked lists was implemented, where each node of the list was pointed to a camera, which belonged to the clique of the specific position of the table.
- __Hashtable-RBT__ :In cases where data had to be searched, a Hashtable was used where each position led to a Red Black Tree. The nodes of each tree indicated the information they were looking for. This structure was used in the case of vocabulary, where with each new word entry it had to be checked if that word already existed, and in the case of specs, where again it had to be checked if one already existed.
- 


## __Compile & Execution__
In the initial directory type
- ```make```: Simple compile
- ```make run-programs```: Compiles and executes the main program
- ```make run-tests```:  Compiles and executes all the tests
- ```make valgring-programs```:  Compiles and executes the main program with valgrind
- ```make valgring-tests```:  Compiles and executes all the tests with valgrind

### Alternatively 
For Dataset-W ```sigmod_large_labelled_dataset.csv``` in the initial directory
```
cd programs/EntityResolution
./entityResolutionCliques -jd ./../../data/camera_specs -csv ./../../data/sigmod_medium_labelled_dataset.csv
```
or for Dataset-Y ```sigmod_medium_labelled_dataset.csv``` in the initial directory
```
cd programs/EntityResolution
./entityResolutionCliques -jd ./../../data/camera_specs -csv ./../../data/sigmod_medium_labelled_dataset.csv
```

## Environment
- Programing language: __C__
- Operating system: Ubuntu Linux 20.04
- gcc version: 9.3.0
- The experiments were conducted using virtual machines utilizing 4 cores of an Intel Core i7 - 8565U CPU and 8GB of main memory

## Team members

__Myrto Igglezou__ \
__Konstantinos Nikoletos__   

### Δομές που χρησιμοποιήθηκαν:

* Έχει υλοποιηθεί ένα hashtable που κάθε θέση του πίνακα έχει έναν δείκτη σε red black tree. Με το διάβασμα του dataset Χ κάθε πληροφορία ενός spec, αποθηκεύεται 
σε ένα struct και μέσω του hashtable κάθε κόμβος του RBT δείχνει σε ένα τέτοιο struct. Στόχος αυτής της υλοποίησης είναι η ταχύτερη αναζήτηση του spec όταν χρειάζεται.
* Για την διαμόρφωση των κλικών χρησιμοποιήθηκε disjoint set. Κάθε spec αντιστοιχίζεται σε μια θέση του πίνακα του set. Συγκεκριμένα, υλοποιήθηκε ένα struct που αποτελείται απο 3 πίνακες, έναν που κρατά τον "πατέρα" κόμβο κάθε spec, έναν που κράτα το rank και έναν που αντιστοιχίζει κάθε θέση του πίνακα με το struct του.
* Μόλις διαμορφωθούν τα set δημιουργείται μια λίστα στον κάθε πάτερα, στην οποία προστίθονται τα spec που ταιριάζουν με αυτόν, και κατ' επέκταση και όλοι αυτοί οι κόμβοι μεταξύ τους.
* Σε κάθε κλίκα υπάρχει ένας bit-Array που χρησιμέυει στην γρήγορη δημιουργία των αρνητικών συσχετίσεων. Ο πίνακας έχει μέγεθος ίσο με το πλήθος των κλικών και κάθε θέση παίνρει την τιμή 1 αν σχετίζεται αρνητικά με την κλίκα που ανήκει ο κάθε πίνακας.
* Για την διαχείρηση των threads δημιουργήθηκε η δομή της ουράς.

### Άλλες σχεδιαστικές επιλογές:

*  Δημιουργήθηκε συνάρτηση grid-search, η οποία έκανε train και test για διαφορετικές τιμές learning rate, num of epochs και threshold, με σκοπό να βρεθεί ο συνδυασμός με το καλύτερο accuracy. Τα αρχεία με τα αποτελέσματα της συνάρτησης είναι στα αρχεία " " και " " για τα αρχεία medium και large αντίστοιχα.
*  Το vectorization των δεδομένων έγινε με τα εξής βήματα:
    1. Πρώτα,  υπολογίστηκε το μέσο tf-idf για κάθε λέξη.
    2. Έπειτα, επιλέχθηκαν οι 1000 λέξεις με το υψηλότερο μέσο tf-idf.
    3. Μετά, για αυτές τις λέξεις υπολογίστηκε το BoW για κάθε spec με βάση αυτές. Στην συνέχεια, με μικρές προσαρμογές, έγινε το tf-idf vectorization. 
    4. Τέλος, δημιουργείται ένας πίνακας που αποθηκέυει τις μη αρνητικές τιμές του διανύσματος.
*  Για τον πειραματισμό εκτέλεσης του προγράμματος με διαφορετικές παραμέτρους, δημιουργήθηκε ένα script σε unix, που τρέχει το πρόγραμμα με διαφορετικές τιμές στα πεδία:
     1. Learning rate
     2. Threshold
     3. Αριθμός εποχών
     4. Αριθμός Thread
     5. Μέγεθος των batches
*  Για την εκτίμηση του μοντέλου χρησιμοποιήθηκαν οι μετρικές accuracy, f1 , recall και precision.
*  Στο διάβασμα όλων των ζευγαριών, στο τμήμα του retrain, το διάβασμά τους γίνεται ανά 50, ωστέ να αποφευχθεί η υπερβολική χρήση μνήμης.
*  Έχει δημιουργηθεί ένα αρχείο Jupyter notebook, που διαβάζει τα αποτελέσματα των πειραματισμών και δημιουργεί τα γραφήματα. Για να εκτελεστεί το script χρειάζεται η εξής διαδικασία:
   1. $ cd  ./scripts
   2. $ ./gridsearch.sh
     
     

