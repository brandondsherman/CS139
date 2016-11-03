#include <unistd.h>
#include <sys/timeb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

long gRefTime;
int threadCount;
int arraySize;
char algorithmChoice;

void Swap(int& , int&);
int Rand(int, int);
int Partition(int[], int, int);


/*
 * @param struct timeb`
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
void SetAlgorithmChoice(char  passedAlgorithmChoice)
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


void InsertionSort(int data[], int size)
{
  int i, j, temp;
  for(i=1; i<size; i++)
  {
    temp = data[i];
    for(j=i-1; j>=0 && data[j]>temp; j--)
      data[j+1] = data[j];
    data[j+1] = temp;
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
 * @param int&
 * @param int&
*/
void Swap(int& x, int& y)
{
  int temp = x;
  x = y;
  y = temp;
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
 * 
*/
int main(int argc, char *argv[])
{
  if (argc < 3 || argc > 4) 
  {
    printf("Too many or too few arguments passed, please enter arguments in the \n array size (1-100000), number of threads (1-16), the sorting algorithm (I or Q)");
    exit(0);
  }

  //read and set array size
  int tempArraySize = atoi(argv[1]); 
  if (tempArraySize <= 1 || tempArraySize >=100000)
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
  if (tempThreadCount <= 1 || tempThreadCount >=16)
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
  //doesn't work idk why tbh
  char tempSortingAlgorithm = (argv[3][0]); 
  if (tempSortingAlgorithm != 'I'|| 
      tempSortingAlgorithm != 'Q' )
  {
    printf("%c", tempSortingAlgorithm);
    printf("Please make a valid choice for the type of sorting algorithm" );
    exit(0);
  }
  else
  {
    SetAlgorithmChoice(tempSortingAlgorithm);
    printf("%c", algorithmChoice);
  }

}


