#include <unistd.h>
#include <sys/timeb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

long gRefTime;
int threadCount;
int arraySize;
int ind;
char algorithmChoice;
int indicies[16][2];

void InsertionSort(int[], int);

void Swap(int, int);
int Rand(int, int);
int Partition(int[], int, int);
int* Merge(int[], int[]);
void FindIndex(int, int);

/*
 * @param struct timeb
 * @returns long
 */
long GetMilliSecondTime(struct timeb timeBuf)
{
  long mliScndTime;
  mliScndTime = timeBuf.time;
  mliScndTime *= 1000;
  mliScndTime += timeBuf.millitm;
  return mliScndTime;
}

/*
 * @param void
 * @returns long
 */
long GetCurrentTime(void)
{
  long crntTime=0;
  struct timeb timeBuf;
  ftime(&timeBuf);
  crntTime = GetMilliSecondTime(timeBuf);
  return crntTime;
}

/*
 *@param void
*/
void SetTime(void)
{
   gRefTime = GetCurrentTime();
}

/*
 * @param void
 * @returns long
*/
long GetTime(void)
{
   long crntTime = GetCurrentTime();
   return (crntTime - gRefTime);
}

/*
 * @param int  
*/
void SetThreadCount(int passedThreadCount)
{
  threadCount = passedThreadCount;
}

/*
 * @param void
 * @returns int
*/
int GetThreadCount(void)
{
  return threadCount;
}

/*
 * @param int  
*/
void SetArraySize(int passedArraySize)
{
  arraySize = passedArraySize;
}

/*
 * @param void
 * @returns int
*/
int GetArraySize(void)
{
  return arraySize;
}

/*
 * @param char 
*/
void SetAlgorithmChoice(char passedAlgorithmChoice)
{
  algorithmChoice = passedAlgorithmChoice;
}

/*
 * @param void
 * @returns char* 
 */
char GetAlgorithmChoice(void)
{
  return algorithmChoice;
}

/*
 * @param int
 * @param int
 * @return int
 */
int Rand(int x, int y)
{ 
  int range = y-x+1;
  int r = rand() % range;
  r += x;
  return r;
} 

/*
 * @param int&
 * @param int&
*/
void Swap(int x ,int y)
{
  int temp = x;
  x = y;
  y = temp;
}

/*
 * @param int[]
 * @param int
 */
void InsertionSort(int arr[], int ind){
  int j, temp, left, right;
  left = indicies[ind][0] ;
  right = indicies[ind][1];
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



void QuickSort(int data[], int p, int r)
{
  int q;
  if(p >= r) return;
  q = Partition(data, p, r);
  QuickSort(data, p, q-1);
  QuickSort(data, q+1, r);
}

/*
 * @param int[]
 * @param int
 * @param int
 * @return int
 */
int Partition(int data[], int p, int r)
{
  int i, j, x, pi;
  pi = Rand(p, r);
  Swap(data[r], data[pi]);
  x = data[r];
  i = p-1;
  for(j=p; j<r; j++)
  {
    if(data[j] < x)
    {
      i++;
      Swap(data[i], data[j]);
    }
  }
  Swap(data[i+1], data[r]);
  return i+1;
}

/*
 * @param int[]
 * @param int[][] //lol no idea
 * @param int
 * @param int[]
 */
void mergeArrays(int array[], int indicies[][2], int threads, int result[])
{
	for (int i = 0; i < threads - 1; i++) {
			int resPos = 0;
			int lhsPos = 0;
			int lhsEnd = indicies[i][1];
			int rhsPos = indicies[i+1][0];
			int rhsEnd = indicies[i+1][1];

			while (lhsPos <= lhsEnd && rhsPos <= rhsEnd) {
					if (array[lhsPos] <= array[rhsPos]) {
							result[resPos] = array[lhsPos];
							lhsPos++;
					} else {
							result[resPos] = array[rhsPos];
							rhsPos++;
					}
					resPos++;
			}

			while (lhsPos <= lhsEnd) {
					result[resPos] = array[lhsPos];
					lhsPos++;
					resPos++;
			}

			while (rhsPos <= rhsEnd) {
					result[resPos] = array[rhsPos];
					rhsPos++;
					resPos++;
			}
	}
}

/*
 * @param int[]
 * @returns int
 */
int IsSorted(int arr[], int arraySize)
{
	int isSorted = 1;
	for (int i = 0; i < arraySize-1; i++)
	{
		int temp = arr[i] + 1;
		if (arr[i] < temp)
		{
			isSorted = -1;
		}
	}
	return isSorted;

}

/*
 * @param int
 * @param int 
 */
void FindIndex(int arraySize, int numThreads)
{
	ind = arraySize/numThreads;
  for (int i = 0; i < numThreads; i++)
	{
		indicies[i][0] = i*ind;
		indicies[i][1] = (i+1)*ind - 1;
	}	
}

/*
 * @param int[]
 * @param int[]
 * @return int[]
 */
int* Merge(int source[], int dest[])
{
	int low;
	int inc = 0;
	for (int i = 0; i < arraySize; i++)
	{
		low = -1;	
		for (int j = 0; j < threadCount; j++)
		{
			if (indicies[j][0] <= indicies[j][1])
			{
				if (low < 0)
				{
						low = source[indicies[j][0]];
				}
				if (low >= source[indicies[j][0]])
				{
						low = source[indicies[j][0]];
						inc = j; 
				}
			}
		}
	indicies[inc][0] +=1;
	dest[i] = low;
	}
	return dest;
}

int main(int argc, char *argv[])
{
  if (argc < 3 || argc > 4) 
  {
    printf("Too many or too few arguments passed, please enter arguments in the \n array size (1-100000), number of threads (1-16), the sorting algorithm (I or Q)");
    exit(0);
  }
 
  //read and set array size
  int tempArraySize = atoi(argv[1]); 
  if ((tempArraySize <= 1 || tempArraySize >=100000000) && (sizeof(tempArraySize) != sizeof(size_t)))
  {
    printf("Your array size is out of range, exiting");
    exit(0);
  }
  else
  {
    SetArraySize(tempArraySize);
    //printf("%d", arraySize);
  }
  
  //read and set thread count
  int tempThreadCount = atoi(argv[2]); 
  if ((tempThreadCount <= 1 || tempThreadCount >=16) && (sizeof(tempThreadCount) != sizeof(size_t)))
  {
    printf("Your thread count size is out of range, exiting");
    exit(0);
  }
  else
  {
    SetThreadCount(tempThreadCount);
    //printf("%d", threadCount);
  }
  
  //read and set thread count
  char* tempSortingAlgorithm = argv[3];
  char sortc = tempSortingAlgorithm[0];
  
  if ((sortc == 'I') || (sortc == 'Q')) 
  {
    SetAlgorithmChoice(sortc);
    //printf("%c", algorithmChoice);
  }
  else
  {
    printf("Please make a valid choice for the type of sorting algorithm" );
    exit(0);
  }

  //1. Fill the array with random numbers
  int arraySize = GetArraySize();
  
	int singleArray[arraySize];
  int tempArray[arraySize];

  for(int i = 0; i < arraySize; i++)
  {
    singleArray[i] = Rand(0, arraySize);
    tempArray[i] = Rand(0, arraySize);
  }

  //2. Based on the number of threads (T), compute the indicies for dividing the array into
  //T equal parts. For example, if the array size (N) is 1000 and T is 2, the indicies will be 0,
  //499, 500, 999. 
	int numThreads = GetThreadCount();
	
	FindIndex(arraySize, threadCount);
		
	//3. Sort sequentially by applying the sorting algorithm to each part of the array, and
	//then combining the sorted parts into one sorted array using an O(n) algorithm
	char sortingAlgorithm = GetAlgorithmChoice(); 
	
	if (sortingAlgorithm == 'I')
	{
		SetTime();

		for (int i = 0; i < numThreads; i++)
		{
			InsertionSort(singleArray, i);
		}
		
		Merge(singleArray, tempArray);
		gRefTime = GetTime();	
	}
	
	else if (sortingAlgorithm == 'Q')
	{
		SetTime();
		
		QuickSort(singleArray, 0, arraySize);
		
		for (int i = 0; i < numThreads; i++)
		{
			QuickSort(singleArray, indicies[i][0], indicies[i][1]);
		}
		
		Merge(singleArray, tempArray);
		gRefTime = GetTime();	
	}
	
	printf("Single Threaded time is: %ld\n", gRefTime);	
	
	// 4. Apply an O(n) algorithm to check if the array has been sorted correctly, and print a
  // message indicating correct/incorrect sorting
	int sortedVal = IsSorted(singleArray, arraySize);
	char sortedChar;
	if (sortedVal == 1)
	{
		sortedChar = 't';
	}
	else sortedChar = 'f';

	/*
	 *print
  for(int i = 0; i < arraySize; i++)
  {
		printf("%d", singleArray[i]);
		printf("%c", ' ');
  }
	
	*/

	printf("Is the array sorted? t = true/f = false \n");
	printf("%c", sortedChar);

	/*	
	if (sortingAlgorithm == 'I')
	{
		for (int i = 0; i < numThreads-1; i++) {
			int start = i* chunkSize;
			int end = start + chunkSize - 1;
			InsertionSort(&arr[start], end + 1);
		}
		//Last chunk with all the remaining content
		int end = arraySize - 1;
		int start = end + 1;
		InsertionSort(&arr[start], end + 1);   
	}

	//quick sort don't work
	else if (sortingAlgorithm == 'I')
	{
		for (int i = 0; i < numThreads-1; i++) {
			int start = i* chunkSize;
			int end = start + chunkSize - 1;
			QuickSort(arr, start, end + 1);
		}
		//Last chunk with all the remaining content
		//int end = arraySize - 1;
		//int start = end + 1;
		//QuickSort(arr, start, end + 1);   
	}

  for(int i = 0; i < arraySize; i++)
  {
		printf("%d", arr[i]);
		printf("%c", ' ');
  }
  */
  
  int threadedArray[arraySize];
  //int tempArray[arraySize];
  
	//5. refill array with random numbers
	for(int i = 0; i < arraySize; i++)
  {
    threadedArray[i] = Rand(0, arraySize);
  }
	
}
