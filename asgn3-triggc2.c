/* Program:     Singly-linked List II
   Author:      Ciara Trigg
   Date:        September 19, 2022
   File name:   asgn3-triggc2.c
   Compile:     gcc -o asgn3-triggc2 asgn3-triggc2.c
   Run:         ./asgn3-triggc2
   
   The program accepts an arbitrary number of jobs, puts 
   them into a singly-linked list in a LIFO order, then 
   prints them. It is an extension of Project 2. 
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
void appendJob(List *list, Job *jobPtr);
void printJobs(List *list);
void print(Job *curJob);
long getTime(); 
void freeJobs(char *command, int numJobs);

int main(){
   long start = getTime();
   List *jobs = createList(); 
   while(
   for(int curJob = 1; curJob <= jobs->numOfJobs; curJob++){
      Job* newJob = createJob();
      appendJob(jobs, newJob);
   }
   printf("program started at: %ld\n", start); 
   printJobs(jobs);
   return 0;
}

List* createList(){
   List* list = (List*)malloc(sizeof(List));
   printf("Please enter a number of jobs: ");
   list->firstJob = NULL; 
   list->lastJob = NULL;
   scanf("%d", &(list->numOfJobs));
   return list;
}

Job* createJob(){
   Job* job = (Job*)malloc(sizeof(Job));
   int numOfWords;
   printf("Enter the number of words, the job name, and the start time: ");
   scanf("%d", &numOfWords);
   job->command[numOfWords] = NULL;
   for(int curWord = 0; curWord < numOfWords; curWord++){
      char* parameter = (char*)malloc(25);
      scanf("%s", parameter); 
      job->command[curWord] = parameter;
   }
   scanf("%ld", &job->startTime);
   job->submitTime = getTime();
   return job;
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

void insertJob(List *list, Job *jobPtr){
   
}

Job *deleteFirstJob(List *list){

}

void printJobs(List *list){
   int jobNum;
   if(list->numOfJobs != 0){
      Job *curJob = list->firstJob;
      printf("# of jobs: %d \n", list->numOfJobs);
      for(jobNum = 1; jobNum <= list->numOfJobs; jobNum++){
         printf("Job %d: \n", jobNum);
         print(curJob);
         curJob = curJob->next;
      }
   }
   else{
      printf("Empty List");
   }
}

void print(Job *curJob){
   printf("command: ");
   int parameter = 0;
   while(curJob->command[parameter] != NULL){
      printf("%s ", curJob->command[parameter]);
      parameter++;
   } 
   printf("\nsubmit time: %ld\n", curJob->submitTime);
   printf("start time: %ld\n", curJob->startTime);

}

long getTime(){
   long curTime = time(NULL); 
   return curTime;
}
