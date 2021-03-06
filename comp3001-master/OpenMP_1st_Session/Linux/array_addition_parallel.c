/*
------------------DR VASILIOS KELEFOURAS-----------------------------------------------------
------------------COMP3001 ------------------------------------------------------------------
------------------PARALLEL PROGAMMING MODULE-------------------------------------------------
------------------UNIVERSITY OF PLYMOUTH, SCHOOL OF ENGINEERING, COMPUTING AND MATHEMATICS---
*/
//gcc example2.c -o p -O2  -fopenmp


#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <sched.h>
#include <pthread.h>
#include <sys/syscall.h>
#include <sys/mman.h>
#include <omp.h>
#include <stdbool.h> 
 #include <sys/time.h>

#define NUM_THREADS 4
#define N 1000000
#define TIMES 1

int A[N], B[N], D[N];

void init();
void un_opt();
void  version1();
void  version2();
void  version3();
bool compare();

int main(){
  
time_t start1, end1;
struct timeval start2, end2;
double pi1,pi2;

init(); //initialize the arrays

start1 = clock();
gettimeofday(&start2, NULL);

for (int i=0; i<TIMES; i++){ //run this many times to get accurate measurement. The output might be wrong in this case. Debug with TIMES=1.
	//un_opt();
	version1();
	//version2();
	//version3();

}

end1 = clock();
gettimeofday(&end2, NULL);
printf(" clock() method: %ldms\n", (end1 - start1) / (CLOCKS_PER_SEC/1000));
printf(" gettimeofday() method: %ldms\n", (end2.tv_sec - start2.tv_sec) *1000 + (end2.tv_usec - start2.tv_usec)/1000);

if (compare()==true)
	printf("\nResult is ok\n");
else
	printf("\nResult is FALSE\n");

}


void init(){

int i;

for (i=0; i<N; i++){
A[i]=rand()%50;
B[i]=rand()%1000;
D[i]=A[i];
}

}

//this is the serial version of the program you need to parallelize
void un_opt(){

int i;

for (i=0; i<N; i++)
 A[i]=A[i] + B[i];

}


bool compare(){

int i;

for (i=0; i<N; i++)
 D[i]=D[i] + B[i];

for (i=0; i<N; i++){
 //printf(" %d %d - ",D[i],A[i]);
 if (D[i]!=A[i])
  return false;
}

return true;

}


//This implementation does not use omp for construct. It is Hard to write, but important to understand how it works
void  version1(){

#pragma omp parallel
{
int id,i,Nthrds, start, end;

id=omp_get_thread_num();
Nthrds=omp_get_num_threads();

start=id   * N / Nthrds;
end=(id+1) * N / Nthrds;

if (id==Nthrds-1)
 end=N;

for (i=start; i<end; i++)
 A[i]=A[i]+B[i];

}

}


void  version2(){

int i;

#pragma omp parallel
{
#pragma omp for
for (i=0; i<N; i++) //openmp makes the loop control index on a parallel loop private to a thread
 A[i]=A[i]+B[i];

}

}


void  version3(){

int i;

#pragma omp parallel for
for (i=0; i<N; i++) //openmp makes the loop control index on a parallel loop private to a thread
 A[i]=A[i]+B[i];


}





