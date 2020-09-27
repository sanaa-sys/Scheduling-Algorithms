// Created by AYESHA ALI,29587778 on 25/09/2019.
// Last modified:8/10/2019
//This program implements the Round Robin scheduling algorithm using ioL system.
//It reads process data from "process-data.txt" or a file inputted by user and
//stores process name, process entry and service time in PCB structure
//With the process data it implements Round Robin algorithm

//Header files needed to implement the code
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/file.h>
#include <sys/wait.h>
#include <stdlib.h>
#include "process-visualiser.h"//Header to use ioL services
#define max 10


typedef enum {
	READY, RUNNING, EXIT//States in which processes exist
} process_state_t ;//instance of enum


typedef struct {
	char process_name[11];
	int entryTime;
	int serviceTime;
	int remainingTime;
	int index;
	process_state_t state;//variable of enum

} pcb_t;//instance of PCB instance
pcb_t queue[10];//using queue for round robin implementation
pcb_t processes[10];//array of PCB structure which can store max. 10 processes
//implementation of queue
int front = 0;
int rear = -1;
int count = 0;
int number;
_Bool isFull() {
//returns True if queue is full
   return count == max;
}
_Bool isEmpty() {
//returns True if queue is empty
   return count == 0;
}
void insert(pcb_t process) {
//inserts process in queue
   if(!isFull()) {
	
      if(rear == max-1) {
         rear = -1;            
      }       

      queue[++rear] = process;
      count++;
   }
}
pcb_t removeData() {
//returns deleted element from queue
pcb_t data = processes[front++];
	
   if(front == max) {
      front = 0;
   }
	
   count--;
   return data;  
  
}
void sort(pcb_t processes[],int number);//function declaration for sort()
void rr(pcb_t processes[],int number);//function declaration for rr()



int main(int argc, char *argv[])
{	
	
	char buffer[100];//stores file content
	char *filename;//stores filename
	//Assigning filename from which process data has to be read from
	
	//Reading from file and storing data in PCB structure
    FILE *fileid = fopen("process.txt","r");//Opens filename in read mode with a file pointer
	if (fileid < 0)//if fileid is -1,it means the file hasn't been opened
    	{   
        	perror("Cant open file");//write string openError to error stream
        }
   	
   	
	fscanf(fileid,"%s %d %d",processes[0].process_name,&processes[0].entryTime,&processes[0].serviceTime);//Reads first process and stores its data in processes array
	processes[0].remainingTime = processes[0].serviceTime;//assigning remaining time == service time
	processes[0].index = 0;//storing index of process
	number=1;//To store number of processes
    while(fgets(buffer,sizeof(buffer),fileid) != NULL)//Reading other lines of file ie other processes
	{	
		
		fscanf(fileid,"%s %d %d",processes[number].process_name,&processes[number].entryTime,&processes[number].serviceTime); //storing data of other processes in processes 
		processes[number].remainingTime = processes[number].serviceTime;//assigning remaining time == service time
		number=number+1;//number of processes incremented
		processes[number].index = number;//storing index of process
	}
	number=number-1;//last value is garbage value 
	
	fclose(fileid);//Close file

	//swap();//function call for sort()
	sort(processes,number);
	rr(processes,number);//function call for fcfs()


}
void sort(pcb_t processes[],int number)
//This function sorts the processes array based on acsending order on entry time using bubble sort
//Arguments:processes array of type pcb_t which has data for all processes and number of integer type which has number of processes
//Return value:None
{
	int j=0,i=0;//counters for loops
	int tmpet=0;//temporary variable to sort entry time of processes
	int tmpst=0;//temporary variable to sort service time of processes
	int tmpin=0;
	char temp[100];//temporary variable to sort process name of processes
	for (i=0;i<number-1;i++)
	{
		for (j=0;j<number-i-1;j++)
		{
			if (processes[j].entryTime > processes[j+1].entryTime)//if entry time of process is greater than that of the process after
			{
				//Sorting entry time
				tmpet = processes[j].entryTime;
				processes[j].entryTime=processes[j+1].entryTime;
				processes[j+1].entryTime=tmpet;
				//Sorting service time
				tmpst = processes[j+1].serviceTime;
				processes[j].serviceTime=processes[j+1].serviceTime;
				processes[j+1].serviceTime=tmpst;
				tmpin = processes[j+1].index;
				processes[j].index=processes[j+1].index;
				processes[j+1].index=tmpin;
				//Sorting process name
				strcpy(temp,processes[j].process_name);
				strcpy(processes[j].process_name,processes[j+1].process_name);
				strcpy(processes[j+1].process_name,temp);
			}
		}
	}
	

}

void rr(pcb_t processes[],int number)
{	
	char *color1 = "black";//background color of interface
	char *color2 = "blue";//foreground color of interface
	initInterface(color1,color2);//initialising the interface for output
 	int Time=processes[0].entryTime;
 	int Total_time = 0;//variable with total time
 	int quantum = 2;
 	int slice = quantum;
 	insert(processes[0]);//adding first process to queue
 	int wait[number];//waiting time of processes
 	int turn[number];//turnaround time of processes
 	pcb_t item;//object of pcb class

 	//insert(dumm);
 	//insert(processes[0]);
 	
 	
 
	
 	for(int i=0;i<number;i++)
 	{
 		Total_time += processes[i].serviceTime;//adding service times to total time
 		int rowIndex=appendRow(processes[i].process_name); //add row for each process
		appendBlank(rowIndex,processes[i].entryTime);//add blank space from 0 until entry time
 	}
		while(Total_time != 0 && Time < 10)//while loop condition
		 
		 	{   //printf(" slice: %d</n>",slice); 
		 	    //printf("Item in queue: %d", count);
		 		if(!isEmpty() && slice ==  quantum){
		 			item = removeData();//executing first process from queue
		 			//printf("item poped: %s</n>", item.process_name);
		 		}
		 		else{
		 			//item = dummy;
		 		}
		 		printf("item process Name: %s</n>", item.process_name);
			
		 		if(item.remainingTime == 1)//if remainingtime < qunatum run process for 1 sec
		 		{
		 			  
		 				printf("%s</n>",item.process_name);
		 				
		 				printf("%s</n>","EXIT");
		 				sleep(item.remainingTime);//execute process for 1 sec
		 				slice ==  quantum;//reset slice value
		 				item.remainingTime = 0;//setting remaining time of process to 0
		 				item.state = EXIT;//setting to exit state
		 				processes[item.index] = item;//update value of processes array in index with item
		 				//printf(" %d</n>", )
		 				printf("Total Time: %d</n>", Total_time);
		 				removeData();//remobe item from queue
		 				Total_time -= item.remainingTime;
		 				
		 		}
		 	   if(item.remainingTime >= 1)//if remainingtime >= 1  the process executes till quantum
		 	   {   
		 			
		 			printf("%s",item.process_name);
		 			printf("%s</n>","RUNNING");
		 			sleep(1);//
		 			item.state = RUNNING;
		 			processes[item.index] = item;
		 			slice--;
		 			item.remainingTime -= 1;//decrementing remaining time with 1
		 			Total_time -= item.remainingTime;
		 		 		
		 		 	
		 	  }
		 	  
		 	  for(int i=0;i<number;i++)
 			  {	
				
					if(Time == processes[i].entryTime && strcmp(processes[i].process_name, item.process_name) != 0)//checking arrival of other processes
					{
						processes[i].state = READY;//setting it in READY state
						//printf(" %s",processes[i].process_name);
						printf("item inserted: %s",processes[i].process_name);
						printf("  %s</n>","ARRIVED");
						insert(processes[i]);//inserting it to queue
						
					}
			   }	
		 	  if(slice == 0 && item.state == RUNNING){//if quantum has ended and process is still running
		 	  		insert(item);//insert item at queue so that it can be executed again
		 	  		slice = quantum;//assign slice to quuantum
		 	  }
		  //printf("Time: %d</n>", Time); 
	      Time++;
	      //printf("process: %s  %d", processes[number-1].process_name, processes[number-1].state);
	      if(processes[number-1].state == EXIT){
	    	//if last process has executed break loop
	      		break;
	      
	      }
	   
		    
				
	   
		 		
		 	}
	
		
		waitExit();
	}
	
