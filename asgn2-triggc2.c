/* Program:     Singly-linked List in C
   Author:      Ciara Trigg
   Date:        September 12, 2022
   File name:   asgn2-triggc2.c
   Compile:     gcc -o asgn2-triggc2 asgn2-triggc2.c
   Run:         ./asgn2-triggc2
   
   The program accepts an arbitrary number of jobs, puts 
   them into a singly-linked list in a LIFO order, then 
   prints them. 
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

struct JOB {
   char programName[25]; 
   int submitTime;       
   int startTime;        
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


int main(){
   List *jobs = createList();
   for(int curJob = 1; curJob <= jobs->numOfJobs; curJob++){
      Job* newJob = createJob();
      appendJob(jobs, newJob);
   }
   printJobs(jobs);
   return 0;
}

List* createList(){
   List* list = (List*)malloc(sizeof(List));
   printf("Please enter a number of jobs: ");
   list->firstJob = NULL; 
   scanf("%d", &(list->numOfJobs));
   return list;
}

Job* createJob(){
   Job* job = (Job*)malloc(sizeof(job));
   printf("Enter the job name, submit time, and start time: ");
   scanf("%s%d%d", job->programName, &job->submitTime, &job->startTime);
   job->next = NULL;
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
      list->lastJob->next = NULL;
   }
}

void printJobs(List *list){
   int jobNum = 1;
   if(list->numOfJobs != 0){
      Job *curJob = list->firstJob;
      printf("# of jobs: %d \n", list->numOfJobs);
      for(jobNum = 1; jobNum <= list->numOfJobs; jobNum++){
         printf("Job %d: \n", jobNum);
         printf("program name: %s\n", curJob->programName);
         printf("submit time: %d\n", curJob->submitTime);
         printf("start time: %d\n", curJob->startTime);
         curJob = curJob->next;
      }
   }
}
