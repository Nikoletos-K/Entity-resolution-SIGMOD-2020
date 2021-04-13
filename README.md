![run-tests](../../workflows/run-tests/badge.svg) \
![](https://img.shields.io/badge/Language-%20C-blue)  
![](https://img.shields.io/badge/Made%20with-Markdown-1f425f)
![](https://img.shields.io/pypi/l/ansicolortags)
![](https://img.shields.io/badge/Made%20with-Bash-1f425f)

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

Our goal was to find all pairs of product specs in dataset X that match, is or refer to the same real-world product. Our output is stored in a CSV file containing only the matching spec pairs found by your system. 

Example of output CSV file
```
left_spec_id, right_spec_id
www.ebay.com//10, www.ebay.com//20
www.ebay.com//10, buy.net//100
```

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
./entityResolutionPairs -jd ./../../data/camera_specs -csv ./../../data/sigmod_large_labelled_dataset.csv
```
or for Dataset-Y ```sigmod_medium_labelled_dataset.csv``` in the initial directory
```
cd programs/EntityResolution
./entityResolutionPairs -jd ./../../data/camera_specs -csv ./../../data/sigmod_medium_labelled_dataset.csv
```

#### Command line execution with given parameters
For example, cmd for large dataset is:

```
./entityResolutionPairs -jd ./../../data/camera_specs 
                        -csv ./../../data/sigmod_large_labelled_dataset.csv 
                        -lr learning_rate 
                        -sv step_value 
                        -et euclid_threshold 
                        -rt retrain_threshold 
                        -ep num_of_epochs 
                        -rl retrain_loops 
                        -bs batch_size 
                        -nt number_of_threads
```
Available options:

- ```-lr```: Learning rate 
- ```-sv```: Step value for the retrain phase [0.001, 0.5]
- ```-et```: Euclid threshold, for the Logistic Regression converge  
- ```-rt```: Retrain threshold 
- ```-ep```: Models max number of running epochs 
- ```-rl```: Max number of loops for the retrain process 
- ```-bs```: Models batch size 
- ```-nt```: Number of threads running in the thread pool




## Input data

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
- __Bitarray__: Bitarray was used to store the relationships between clicks but also between specs. Each click has a table of bits, the size of the number of clicks. The click has a negative correlation with the clickers in the positions with value 1. More , in the implementation of the third part of the work we needed to know the pairs that have been created, so that in the retrain process they are not trained themselves. This information is captured via a bitArray in each spec size equal to the number of all specs. Each spec stores in the table the positions corresponding to specs that have either a positive or a negative correlation.
- __Disjoint set__: The Disjoint set data structure was used to create clicks. It is essentially an array of integers, where each position represents a spec and the value of this position stores the "father" of the spec, ie elements with the same "father" belong and in the same clique. After reading the Dataset W, any positive correlation implies that the two elements belong to the same clique. Then the table is updated.

## Unit testing
For the testing of the structures but also of the models that we have created, we used the library ```acutest.h```.


More information about the [acutest library](https://github.com/mity/acutest)

## Multi-threading implementation

For the faster execution of the program, it has been paralleled in two points of the program. But before the operation is mentioned in these points, it will
become a concise explanation of the implementation of multithreading. The function is in the folder ```/modules/JobScheduler``` and consists of:
- A scheduler, which actually accepts jobs and assigns them to threads, uses a priority queue to temporarily store the jobs. We have a thread pool and a continuous flow of independent jobs. When a task is created, it enters the scheduler's priority queue and waits to be executed. The work is performed in the order they were created (first-in-first-out - FIFO). Each thread waits in line until it is assigned a task and, after performing it, returns to the queue to undertake a new task. For the proper operation of a timer, it is necessary to use semaphores in the queue, in order to block the threads there, and the critical section, so that tasks can be properly imported and exported from the queue.

- The implementation of the scheduler has been done with the Queue structure

We have created two timers, one that parallels the calculation of gradients in batches (the function will be explained in the next chapter
she) and one that creates parallel pairs to become the retrain.

We have created two timers, one that parallels the calculation of gradients in batches (this function will be explained in the next chapter) and one that creates parallel pairs to retrain.

Benefits of this option:
1. The number of threads is not abused (as they are reused) and we take advantage of all the acceleration they can offer.
2. Independent tasks such as those mentioned above are parallelized resulting in acceleration. 

## Machine learning model - Logistic Regression

### Gradient descent algorithms - GD
We tried three versions of this model:

#### Full-Batch GD
In this implementation every calculation of some derivatives and the gradient in general required access to the entire Dataset, which we observed that:

- Improved and accelerated the convergence process in seasons (ie fewer seasons for convergence), presenting smoother transitions, however it proved to be a bad tactic due to the very high volume of repetitions in each season.

- Brought a huge increase in the training time of the model. The implementation of Full-Batch Gradient Descent was our first attempt, which we rejected as it was not requested, but also because, based on our experiments, it took too long to execute. 

#### Stochastic GD
This algorithm was implemented as part of the second implementation and in short, the model accepts the vector data and for each of them finds the loss and the gradient and updates the stored weights. For this algorithm we observe the following:

- It needs more epochs than the other to converge, but the convergence effort is not smooth due to the constant change of weights.
- Significant reduction of the model training time. 


#### Mini-Batch GD
Mini-Batch GD is our final algorithm, which has been implemented and exists in the final implementation of our system. In short, this algorithm is a hybrid of the previous two as it updates the algorithm weights for each data set. The 516, 10264 and 2056 elements are selected as the beam size, based on which our experiments were performed. This algorithm offers us the best results and for which we make the following observations:
- Shows a smoother convergence behavior than the stochastic algorithm.
- Execution time is not long.

### Re-trainning to strong probabilities
The retraining of the model is achieved through a loop that lasts either certain repetitions or stops for specific values of a threshold. \
At first, the model is trained through the Mini batch GD algorithm of logistic regression using multi-threading. Then, again using multi-threading, all the pairs that had not been determined positive or negative correlation are traversed and given to the model. If a strong positive or negative probability arises, the new pair is stored in a table. This table is then sorted by quick sort from highest probability to lowest. Through the resolve-transitivity-issues function some pairs are integrated into the training set


## Model performance

Metrics: __Accuracy,Precision,Recall,F1-Score__

#### Best results for medium labelled dataset ***without re-train***

| Learning rate | Threshold euclid | Epochs | Batch size | Threads | Test Accuracy | Test Recall | Test Precision | Test F1 | Validation Accuracy | Validation Recall  | Validation Precision | Validation F1 | Time CPU | Time Real |
| ---- | ---- | ---- | ---- | ---- | ---- | ---- | ---- | ---- | ---- | ----  | ---- | ---- | ---- | ---- |
| 0.001 | 0.00010 | 50 | 2056 | 10 | 92.69 | 8.50  | 70.93 | 15.17 | 92.74 | 8.36  | 75.00 | 15.04 | 1.41 | 1.52 |
| 0.001 | 0.00001 | 50 | 2056 | 10 | 92.69 | 8.50  | 70.93 | 15.17 | 92.74 | 8.36  | 75.00 | 15.04 | 1.41 | 1.54 |
| 0.001 | 0.00100 | 50 | 2056 | 10 | 92.69 | 8.50  | 70.93 | 15.17 | 92.74 | 8.36  | 75.00 | 15.04 | 1.39 | 1.53 |
| 0.100 | 0.10000 | 50 | 2056 | 10 | 92.45 | 16.02 | 53.00 | 24.60 | 92.68 | 17.41 | 58.14 | 26.80 | 1.40 | 1.50 |
| 0.100 | 0.00100 | 50 | 2056 | 10 | 92.45 | 16.02 | 53.00 | 24.60 | 92.68 | 17.41 | 58.14 | 26.80 | 1.35 | 1.44 |

#### Best results for large labelled dataset ***without re-train***

| Learning rate | Threshold euclid | Epochs | Batch size | Threads | Test Accuracy | Test Recall | Test Precision | Test F1 | Validation Accuracy | Validation Recall  | Validation Precision | Validation F1 | Time CPU | Time Real |
| ---- | ---- | ---- | ---- | ---- | ---- | ---- | ---- | ---- | ---- | ----  | ---- | ---- | ---- | ---- |
| 0.001| 0.00001 | 50 | 512 | 20 | 88.52 | 12.06 | 75.64 | 20.81 | 88.61 | 12.60 | 77.26 | 21.66 | 9.85  | 10.48 |
| 0.001| 0.00010 | 50 | 512 | 20 | 88.52 | 12.06 | 75.64 | 20.81 | 88.61 | 12.60 | 77.26 | 21.66 | 9.86  | 13.65 |
| 0.001| 0.00100 | 50 | 512 | 20 | 88.52 | 12.06 | 75.64 | 20.81 | 88.61 | 12.60 | 77.26 | 21.66 | 9.86  | 10.50 |
| 0.001| 0.10000 | 50 | 512 | 20 | 88.49 | 11.50 | 76.44 | 19.99 | 88.56 | 11.97 | 77.68 | 20.74 | 9.24  | 9.85  |
| 0.010| 0.00001 | 50 | 512 | 10 | 88.42 | 9.79  | 80.02 | 17.45 | 88.44 | 9.99  | 80.34 | 17.77 | 10.00 | 10.62 |


#### Best results for large labelled dataset ***with re-train***

| Learning rate | Threshold euclid | Retrain threshold | Epochs | Batch size | Threads | Validation Accuracy | Validation Recall  | Validation Precision | Validation F1 | Time CPU | Time Real |
| ---- | ---- | ---- | ---- | ---- | ---- | ---- | ---- | ---- | ---- | ----  | ----  |
| 0.00100| 0.0001   | 0.020000 | 50 | 2056 | 20 | 92.80 | 14.62 | 64.02 | 23.81 | 5.61  | 10.20 |
| 0.001000| 0.000100 | 0.020000 | 50 | 1024 | 20 | 92.37 | 3.34  | 57.14 | 6.32  | 13.45 | 18.61 |
| 0.001000| 0.000100 | 0.010000 | 50 | 1024 | 20 | 92.37 | 3.34  | 57.14 | 6.32  | 13.57 | 18.81 |
| 0.10000| 0.000    | 0.020    | 5  | 1024 | 20 | 92.31 | 0.00  | 0.00  | 0.00  | 1.85  | 5.33  |




#### Scores per retrain epoch

![](./images/performance.png?raw=true "Plots")




## Performance remarks for medium dataset

### Remarks based on the diagrams
- The red curve (Accuracy) that mainly remains stationary, which means that the retrain ultimately does not significantly increase accuracy. (eg Plot 3)
- In other cases, however, due to incorrect predictions that changed the shape of the cliques, the red curve (Accuracy) shows large falls and rises, resulting in the model becoming worse than it was in the beginning. (eg Plot 11,8)
- The Recall curve (blue) has the opposite convergence than the Accuracy curve (red), something to be expected as when the model better predicts the same pairs, it loses accuracy from the different objects (eg PLot 3,5,6)
- The Recall, Precision, F1 curves remain lower than the red one as there is a weakness in predicting the same pairs.

### Comments based on the scoreboards:
- Only in one case the model predicts all zeros and some aces (92.80% accuracy)
- The batch with size 2056 again offers the best results as well as without retrain. 


## Alternative implementation
In the second part of the implementation of the system we were called to implement the categorization tactic in an alternative way. More specifically:

- Each clique is trained separately with its own objects and therefore a categorization model is created for each clique.
- Then a set of objects from all cliques is formed and each object is tested in each clique. The clique that will give the highest probability will be the one that the object belongs to. This implementation was rejected due to its very low performance, but and why in the end it was not in demand. 

## Implementation techniques for increasing systems performance
In the various stages of the work it was observed that the following design options improved the execution time of the program, without straining the memory:

- __Bit-array__: In order to avoid the storage of unnecessary information but also the continuous access of memory, the bit Array was used where possible, eg storage of positive and negative correlations.

- __Disjoint set__: A disjoint set was used to form the clicks, as opposed to the use of lists and indicators. The space reserved was smaller and the calculations were significantly faster.

- __Dictionary__: In order to avoid multiple json accesses, all the necessary information is collected and all possible calculations are done on the first reading of the files.

- __Sparse Matrix__: For optimal storage of vectors only the non-negative positions of the vector are stored. In this way the reading, but also their storage becomes faster.

- __Red Black Tree__: In cases where it was necessary to search for data, a red black tree was used, since for the search, in the worst case it has complexity O (logn), while on the contrary other structures have O (n).
 

## Shell script - **Bash** 
Implemented a shell script that executes the system for multiple variables. We took advantage of this test in order to make a gridsearch. \
Execution:
```
cd ./scripts
./gridsearch.sh
```

## Python notebook
We created a notebook, in order to interpreter system results. 

## Environment
- Programing language: __C__
- Operating system: __Ubuntu Linux 20.04__
- gcc version: __9.3.0__
- The experiments were conducted using virtual machines utilizing 4 cores of an __Intel Core i7 - 8565U CPU__ and 8GB of main memory


---

© Myrto Iglezou && Konstantinos Nikoletos 


     
     

