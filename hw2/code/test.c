
/*
Christopher De Leon
CSC 139
Assignment 1
*/
//#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <sys/timeb.h>
#include <sys/types.h>
#include <stdio.h>
#include <pthread.h>

//using namespace std;

int arraySize;
int numThreads;
int *mainArray;
int *threadedArray;
int *finalArray;
int value;
char* sortAlg;
long gRefTime;
int indices[16][2];
pthread_attr_t attr;
pthread_t threads[16];
int thread_args[16];


void calculateIndices();
int* combineArrays(int arr[], int finalArr[]);
void insertionSort(int arr[], int arraySize);
void *intertionSortThreaded(void *index);
void *quickSortThreaded(void *index);
void QuickSort(int arr[], int left, int right);
int Partition(int data[], int p, int r);
void Swap(int x, int y);
int Rand(int x, int y);
void validateArray(int arr[]);
void printArray(int arr[]);
long GetMilliSecondTime(struct timeb timeBuf);
long GetCurrentTime(void);
void SetTime(void);
long GetTime(void);

int main(int argc, char* argv[]){
  /*validate if there are not enough arguments*/
  if (argc < 4){
    printf("Not enough arguments. Usage: %c , Array size, Number of threads, Sorting algorithm (I or Q)\n", argv[0]);
    return -1;
  }
  /*validate if there are too many arguments*/
  else if (argc > 4){
    printf("Too many arguments. Usage: %s , Array size, Number of threads, Sorting algorithm (I or Q)\n", argv[0]);
    return -1;
  }
  /*validate the array size*/
  arraySize = atoi(argv[1]);
  if (arraySize > 100000000 || arraySize < 1){
    printf("Not a valid array size. Must be between 1 to 100,000,000\n");
    
    return -1;
  }
  /*validate the number of threads*/
  numThreads = atoi(argv[2]);
  if (numThreads > 16 || numThreads < 1){
    printf("Not a valid number of threads. Must be between 1 to 16\n");
      return -1;
  }
  /*validate sorting algorithm options*/
  sortAlg = argv[3];
  if ((sortAlg.compare("i") != 0) && (sortAlg.compare("I") != 0) && (sortAlg.compare("q") != 0) && (sortAlg.compare("Q") != 0)){
    printf("Not a valid sorting algorithm option. Must be either I or Q\n");
      return -1;
  }


  /*declares the sizes of the arrays*/
  mainArray = new int[arraySize];
  threadedArray = new int[arraySize];
  finalArray = new int[arraySize];
  
  calculateIndices();
  

  /*fills two arrays with random numbers*/
  for (int i = 0; i <= arraySize; i++){
    int rnd = rand() % arraySize;
    mainArray[i] = rnd;
    threadedArray[i] = rnd;
  }
  /*figures if instertion sort or quicksort is used*/
  if (sortAlg.compare("i") == 0 || sortAlg.compare("I") == 0){
    SetTime();
    for (int index = 0; index < numThreads; index++){
      insertionSort(mainArray, index);
    }
    combineArrays(mainArray, finalArray);
    gRefTime = GetTime();
  }
  else if (sortAlg.compare("q") == 0 || sortAlg.compare("Q") == 0){
    SetTime();
    QuickSort(mainArray, 0, arraySize);
    for (int index = 0; index < numThreads; index++){
      QuickSort(mainArray, indices[index][0], indices[index][1]);
    }
    combineArrays(mainArray, finalArray);
    gRefTime = GetTime();
  }
  printf("The sequential sorting time is: %ld\n", gRefTime);
  validateArray(finalArray);
  //printArray(finalArray);
  
  calculateIndices();
  
  SetTime();
  for (int index = 0; index < numThreads; index++){
    int rc;
    /* get the default attributes */
    pthread_attr_init(&attr);
    
    thread_args[index] = index;
    if (sortAlg.compare("i") == 0 || sortAlg.compare("I") == 0){
      rc = pthread_create(&threads[index], &attr, intertionSortThreaded, (void *) index);
    }else if (sortAlg.compare("q") == 0 || sortAlg.compare("Q") == 0){
      rc = pthread_create(&threads[index], &attr, quickSortThreaded, (void *) index);
    }
    
    if (rc){
      printf("Error: unable to create thread, %d\n", rc);
    }
  }
  for (int index = 0; index < numThreads; index++){
    pthread_join(threads[index],NULL);
  }
  
  /* for (int t = 0; t < numThreads; t++){
    for (int i = indices[t][0]; i <= indices[t][1]; i++){
      printf("value:%d, thread:%d, element #: %d\n", threadedArray[i], t, i);
    }
  }  */
  
  combineArrays(threadedArray,finalArray);
  gRefTime = GetTime();
  printf("The threaded sorting time is: %ld\n", gRefTime);
  validateArray(finalArray);
  //printArray(finalArray);

}
/*combines the sorted parts of an array into another array fully sorted*/
int* combineArrays(int arr[], int finalArr[]){
  int lowest;
  int incIndice = 0;
  for (int i = 0; i < arraySize; i++){
    lowest = -1;
    for (int j = 0; j < numThreads; j++){
      if (indices[j][0] <= indices[j][1]){
        if (lowest < 0)
        {
          lowest = arr[indices[j][0]];
        }
        if (lowest >= arr[indices[j][0]]){
          lowest = arr[indices[j][0]];
          incIndice = j;
        }
      }
    }
    indices[incIndice][0] += 1;
    finalArr[i] = lowest;
  } 
  return finalArr;
}

/*calculate indices*/
void calculateIndices(){
  value = arraySize/numThreads;
  for (int i = 0; i < numThreads; i++){
    indices[i][0] = i*value; //left side
    indices[i][1] = (i+1)*value-1; //right side
  }
}

/*insertion sort algorithm*/
void insertionSort(int arr[], int index){
  int j, temp, left, right;
  left = indices[index][0] ;
  right = indices[index][1];
  for (int i = left; i <= right; i++){
    j = i;
    while (j > left && arr[j] < arr[j - 1]){
      temp = arr[j];
      arr[j] = arr[j - 1];
      arr[j - 1] = temp;
      j--;
    }
  }
}

/*insertion sort threaded algorithm*/
void *intertionSortThreaded(void *index){
  int j, temp, left, right;
  left = indices[(int)index][0] ;
  right = indices[(int)index][1];
  for (int i = left; i <= right; i++){
    j = i;
    while (j > left && threadedArray[j] < threadedArray[j - 1]){
      temp = threadedArray[j];
      threadedArray[j] = threadedArray[j - 1];
      threadedArray[j - 1] = temp;
      j--;
    }
  }
  pthread_exit(0);
}

/*quick sort threaded algorithm*/
void *quickSortThreaded(void *index){
  int left, right;
  left = indices[(int)index][0];
  right = indices[(int)index][1];
  
  QuickSort(threadedArray, left, right);

  pthread_exit(0);
}

/*quick sort algorithm*/
void QuickSort(int data[], int p, int r){
     int q;

     if(p >= r) return;
     q = Partition(data, p, r);
     QuickSort(data, p, q-1);
     QuickSort(data, q+1, r);

}

 
/*partitions the array and swaps array elements*/
int Partition(int data[], int p, int r){
     int i, j, x, pi;

     pi = Rand(p, r);
     Swap(data[r], data[pi]);

     x = data[r];
     i = p-1;
     for(j=p; j<r; j++){
    if(data[j] < x){
      i++;
      Swap(data[i], data[j]);
    }
     }
     Swap(data[i+1], data[r]);
     return i+1;
}

/*swaps the values*/
void Swap(int x, int y){
     int temp = x;
     x = y;
     y = temp;
}

/*generates a random piviot point*/
int Rand(int x, int y){
     int range = y-x+1;
     int r = rand() % range;
     r += x;
     return r;
}

void validateArray(int arr[]){
  bool correct;
  /*checks each element of the array and verify if the array is sorted corrected*/
  for (int i = 1; i < arraySize; i++){
    if (arr[i] >= arr[i - 1]){
      correct = true;
    }else{
      printf("Element %d is not correct", i);
      i = arraySize + 1;
      correct = false;
    }
  }
  if (correct){
    printf("The sorting is correct\n");
  }
  else{
    printf("The sorting is incorrect\n");
  }
}

void printArray(int arr[]){
  /*prints out array to see if sorted correctly*/
  for (int i = 0; i < arraySize; i++){
    printf("%d, %d\n", arr[i], i);
  }
}

long GetMilliSecondTime(struct timeb timeBuf)
{
  long mliScndTime;

  mliScndTime = timeBuf.time;
  mliScndTime *= 1000;
  mliScndTime += timeBuf.millitm;
  return mliScndTime;
}

long GetCurrentTime(void)
{
  long crntTime = 0;

  struct timeb timeBuf;
  ftime(&timeBuf);
  crntTime = GetMilliSecondTime(timeBuf);

  return crntTime;
}

void SetTime(void)
{
  gRefTime = GetCurrentTime();
}

long GetTime(void)
{
  long crntTime = GetCurrentTime();

  return (crntTime - gRefTime);
