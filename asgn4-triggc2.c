/* Program:     Job Queue for Batch Job Dispatcher
   Author:      Ciara Trigg
   Date:        September 23, 2022
   File name:   asgn4-triggc2.c
   Compile:     gcc -o asgn4-triggc2 asgn4-triggc2.c
   Run:         ./asgn4-triggc2
   
   The program accepts an arbitrary number of jobs, puts 
   them into a singly-linked list in ascending order, then 
   prints them. It is an extension of Project 3. 
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

struct JOB {
   char *command[5]; 
   long submitTime;       
   long startTime;        
   struct JOB *next;     
};
typedef struct JOB Job;

struct LIST {
   int numOfJobs;        
   Job *firstJob;        
   Job *lastJob;         
};
typedef struct LIST List;

List* createList();
Job* createJob();
void insertJob(List *list, Job *jobPtr);
Job *deleteFirstJob(List *list);
void appendJob(List *list, Job *jobPtr);
void printJob(Job *curJob);
void printJobs(List *list);
long getTime(); 
void sepParameters(Job *job, int numOfWords);
void newFirstNode(List *list, Job *jobPtr);
void insertAscending(List *list, Job *jobPtr, int startTime);
void insertInEmpty(List *list, Job *jobPtr);
void freeJob(Job *jobPtr);

int main(){
   List *list = createList();
   char input;
   while(input != '!'){
      scanf("%c", &input);
      if(input == '+'){
         Job* jobPtr = createJob();
         insertJob(list, jobPtr);
         list->numOfJobs = list->numOfJobs + 1; 
      }
      else if(input == '-'){
         Job* deletedJob = deleteFirstJob(list);
         freeJob(deletedJob);
      }
   }
   printJobs(list);
   return 0; 
}

List* createList(){
   List* list = (List*)malloc(sizeof(List));
   list->firstJob = NULL;
   list->lastJob = NULL;
   return list;
}

Job* createJob(){
   Job* jobPtr = (Job*)malloc(sizeof(Job));
   int numWords;
   scanf("%d", &numWords);
   jobPtr->command[numWords] = NULL;
   sepParameters(jobPtr, numWords);
   scanf("%ld", &jobPtr->startTime);
   jobPtr->submitTime = getTime();
   return jobPtr;
}

void insertJob(List *list, Job *jobPtr){
   long startTime = jobPtr->submitTime + jobPtr->startTime; 
   if(list->numOfJobs == 0){
      insertInEmpty(list, jobPtr);
   }
   else if(startTime <= (list->firstJob->submitTime + list->firstJob->startTime)){
      newFirstNode(list, jobPtr);
   }
   else if(startTime >= (list->lastJob->submitTime + list->lastJob->startTime)){
      appendJob(list, jobPtr);
   }
   else{
      insertAscending(list, jobPtr, startTime);
   }
}

Job *deleteFirstJob(List *list){
   if(list->firstJob == NULL){
      printf("No Job Deleted\n");
      return NULL;
   }
   else{
      Job* jobPtr = list->firstJob;
      printf("Job Deleted: \n");
      printJob(jobPtr);
      list->numOfJobs = list->numOfJobs - 1;
      list->firstJob = jobPtr->next;
      return jobPtr; 
   }
}

void appendJob(List *list, Job *jobPtr){
   if(list->firstJob == NULL){
      list->firstJob = jobPtr;
      list->lastJob = jobPtr;
   }
   else{
      list->lastJob->next = jobPtr;
      list->lastJob = jobPtr;
   }
}

void printJob(Job *curJob){
   printf("command: ");
   int parameter = 0;
   while(curJob->command[parameter] != NULL){
      printf("%s ", curJob->command[parameter]);
      parameter++;
   } 
   printf("\nsubmit time: %ld\n", curJob->submitTime);
   printf("start time: %ld\n", curJob->startTime);
}

void printJobs(List *list){
   int jobNum;
   if(list->numOfJobs != 0){
      Job *curJob = list->firstJob;
      printf("# of jobs: %d \n", list->numOfJobs);
      for(jobNum = 1; jobNum <= list->numOfJobs; jobNum++){
         printf("Job %d: \n", jobNum);
         printJob(curJob);
         curJob = curJob->next;
      }
   }
   else{
      printf("Empty List");
   }
}

long getTime(){
   long curTime = time(NULL);
   return curTime;
}

void sepParameters(Job *job, int numOfWords){
   for(int curWord = 0; curWord < numOfWords; curWord++){
      char* parameter = (char*)malloc(25);
      scanf("%s", parameter); 
      job->command[curWord] = parameter;
   } 
}

void newFirstNode(List *list, Job *jobPtr){
   Job* tempJob = (Job*)malloc(sizeof(Job));
   tempJob = list->firstJob;
   list->firstJob = jobPtr;
   jobPtr->next = tempJob; 
}

void insertAscending(List *list, Job *jobPtr, int startTime){
   Job* curJob = (Job*)malloc(sizeof(Job));
   Job* tempJob = (Job*)malloc(sizeof(Job));
   curJob = list->firstJob;
   while(startTime >= (curJob->next->submitTime + curJob->next->startTime)){
      curJob = curJob->next;
   }
   if(startTime == (curJob->next->submitTime + curJob->next->startTime)&&
      curJob->submitTime < jobPtr->submitTime){
      tempJob = curJob->next->next; 
      curJob->next = jobPtr;
      jobPtr->next = tempJob; 
   }
   else{
      tempJob = curJob->next;
      curJob->next = jobPtr;
      jobPtr->next = tempJob;  
   }
}

void insertInEmpty(List *list, Job *jobPtr){
   list->firstJob = jobPtr;
   list->lastJob = jobPtr;
}

void freeJob(Job *jobPtr){
   int cur = 0;
   while(jobPtr->command[cur] != NULL){
      free(jobPtr->command[cur]);
      cur++;
   }
   free(jobPtr);
}
