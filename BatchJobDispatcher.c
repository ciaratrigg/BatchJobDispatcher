/* Program:     Batch Job Dispatcher - Threads
   Author:      Ciara Trigg
   Date:        September 23, 2022
   File name:   asgn6-triggc2.c
   Compile:     gcc -lpthread asgn6-triggc2.c
   Run:         ./a.out
   
   Batch Job Dispatcher allows the user to submit jobs 
   (programs along with command line parameters) and specify a time, 
   at which the job should be started to execute. This is similar to 
   the cron utility program in Unix operating systems which allows 
   the user to schedule jobs. This implementation of the Batch Job 
   Dispatcher uses POSIX threads so that jobs can run concurrently 
   and the user can continue entering new jobs even while others are 
   executing. 
   
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>

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
void *dispatcherThread(void *ptr);
void *schedulerThread(void *ptr);
void *executerThread(void *ptr);


int main(){
   List *list = createList();
   pthread_t dispatch, schedule;
   
   pthread_create(&dispatch, NULL, schedulerThread, (void*)list);
   pthread_create(&schedule, NULL, dispatcherThread, (void*)list);
   pthread_join(schedule, NULL);
   pthread_join(dispatch, NULL);
   printJobs(list);
   
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
      list->firstJob = jobPtr->next;
      list->numOfJobs = list->numOfJobs - 1;
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

void *schedulerThread(void *ptr){
   List *list = ptr;
   char input; 
   while(1){
      scanf("%c", &input);
      if(input == '+'){
         Job* jobPtr = createJob();
         insertJob(list, jobPtr);
         list->numOfJobs = list->numOfJobs + 1; 
      }
      else if(input == 'p'){
         printJobs(list);
      }
      else if(input == '-'){
         Job* deletedJob = deleteFirstJob(list);
         freeJob(deletedJob);
      }
   }
}

void *dispatcherThread(void *ptr){
   long curTime;
   List *list = ptr;
   Job *nextJob = NULL;
   while(1){
      pthread_t fork;
      if(list->numOfJobs > 0){
         curTime = getTime();
         nextJob = list->firstJob;
         if(curTime >= nextJob->submitTime + nextJob->startTime){
            Job *temp = deleteFirstJob(list);
            pthread_create(&fork, NULL, executerThread, (void*)temp);
            pthread_join(fork, NULL);
         }
         else {
            sleep(1);
         }
      }
      else {
         sleep(1);
      }
   }
   return NULL;
}

void *executerThread(void *ptr){
   char *argv[4];
   int status = 999, child_pid;
   Job *jobPtr = ptr; 
   for(int i = 0; i <= 4; i++){
      if(jobPtr->command[i] != NULL){
         argv[i] = jobPtr->command[i];
      }
      else{
         argv[i] = NULL;
      }
   }
   child_pid = fork();
   if(child_pid == 0){
      execvp(argv[0], argv);
      exit(1);
   }
   else{
      waitpid(child_pid, &status, WEXITED);
   }
   free(jobPtr);
   return NULL; 
}
