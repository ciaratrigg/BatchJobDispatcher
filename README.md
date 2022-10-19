# BatchJobDispatcher

This repository contains the series of assignments completed to build a Batch Job Dispatcher comparable to Unix's cron utility program. The final implementation of the job scheduler uses POSIX threads to concurrently run jobs, even as the user continues to enter them. Each job given to the job scheduler is stored in a queue (implemented as a singly linked list) until it executes. Upon execution, the contents of each job is printed and then it is deleted from the queue. 
