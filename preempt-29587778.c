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

void rr(proc_data_t processes[],int number)
{	
	char *color1 = "black";//background color of interface
	char *color2 = "blue";//foreground color of interface
	initInterface(color1,color2);//initialising the interface for output
 	if (sizeof(proc_data_t) == 0)
    	{
        return 0;
    	} 
	char okay[100]= "That's alright";
	int totalTime = 0;
	proc_data_t current_process;
	   // Appending the row as given order in the file of the process name
   for(int i = 0; i < number_of_process-1; i++){
       appendRow(process_array[i].processName);
    
    }
 
   // General Sorting Funciton to sort it according to there entry time 
   sort(array, number_of_process-1);
   // calculating the totaltime and storing the remaining time in the process
   for(int i=0 ; i< number_of_process-1; i++){
        totalTime = totalTime + array[i].serviceTime;
        array[i].remaningTime = array[i].serviceTime;
        array[i].state = BLANK;
        array[i].sort_index = i;
        //array[i].finishTime = 0;
   
   }
   // intial process should be in ready state and printing for the arrival
   array[0].state = READY;
   printf("  %s entered the system at %d</n>",array[0].processName, array[0].entryTime);
   
  // first item in the queue state
   push(array[0]);
   // updated one
   appendBlank(array[0].index,array[0].entryTime); 
   // time starts at zero
   // updated one 
   int time = array[0].entryTime;
   // time_bar for the display
   int quantum = 2;
   int count = 0;
   while(totalTime != 0){
        // Checking for the size of the queue and for the newly entered for the quantum count 
        if(size() > 0 && count == 0){
             current_process = pop();
             current_process.state = RUNNING;
             if (count == 0){
                 printf("  %s is in running state (for the given quantun)</n>",current_process.processName);
             }
        } 
        else{   
                current_process.state = DEADEND;
               
        }
       
        
       // printf(" %d</n>", totalTime);
       // printf("total Time: %d</n>", totalTime);
       // Putting the current process into the Running state
        if (current_process.remaningTime == 1){
                // totalTime deduction from the current Time
                totalTime = totalTime - current_process.remaningTime;
               // time_bar = current_process.remaningTime;
                appendBar(current_process.index,current_process.remaningTime, colour2, current_process.processName, 0);
                current_process.remaningTime = 0;
                current_process.turnAroundTime = array[current_process.sort_index].waitingTime + current_process.serviceTime; 
                
                // updating the state of the process into the EXIT status
                current_process.state = EXIT;
               // printf("process_EXIT: %s Process State: %d</n>", current_process.processName,current_process.state);
                //current_process.finishTime = time;
                  //printf("waitingTime in the exit state: %d</n>",   array[current_process.sort_index].waitingTime);
                  printf("Process Name: %s Completed. Turnaround time: %d seconds  Total wait time: %d seconds </n>", current_process.processName, current_process.turnAroundTime, array[current_process.sort_index].waitingTime);
                array[current_process.sort_index] = current_process;
              
                // making the quatum count equals to zero
                count = 0;
              
               // slice = true;
                
        }
        // Running status for the process
        else if(current_process.remaningTime > 1){
             appendBar(current_process.index, 1, colour2, current_process.processName, 0);
             current_process.state = RUNNING;
             current_process.remaningTime = current_process.remaningTime - 1;
            // printf("Process Name: %s in the runnnig state</n>", current_process.processName);
             // total time deduction by the 1 secs
             totalTime = totalTime - 1;
             //time_bar = 1;
               count++;
             array[current_process.sort_index] = current_process;
           
          
        }
      
        // Checking for the next process, to update the status
        for(int k = 0; k < number_of_process-1; k++){
                      // Checking the entry time for the rest left process
                  if(array[k].entryTime ==  time && array[k].state == BLANK){ 
                             printf("%s entered the system in %d seconds</n>", array[k].processName, array[k].entryTime);
                             // Appending the blank till the EntryTime
                             appendBlank(array[k].index,time);  // Here index refers th
                             // Updating the status of the Proacess with the READY STATUS
                             array[k].state = READY;
                             // if next queue is nothing so next one should be first entry one
                             
                             push(array[k]);
                  }
                         
                    
                  if (array[k].state == READY && current_process.state != DEADEND){
                       // If the left process is in the READY status then another dotted bar should be displayed on the display according to the row
                       appendBar(array[k].index,1, colour2, array[k].processName, 1);
                       // Adding the waiting time for that particular process
                     
                       
                     
                  }
                   
                      
          }
         // printf("size(): %d</n>", size());
         // Updating the waiting time in the for queued item by one 
          for(int l = front; l < size()+front; l++){
                // if it is in the system then add in the queue only
                
               queue[l].waitingTime++;
                // update it in as well as in the array
              array[queue[l].sort_index].waitingTime++;
               // printf("processName: %s waitingTime: %d</n>", array[queue[l].sort_index].processName,  array[queue[l].sort_index].waitingTime);
              
          }
              
            
          // checking for the running state and time quatum to select another process
          if (current_process.state == RUNNING && count == quantum){
                   //printf("ready state to check the queue push process: %s Process State: %d</n>", current_process.processName,current_process.state);
              // putting back into the READY state
              current_process.state = READY;
             
             // updating the array with the current_process changes 
              array[current_process.sort_index] = current_process;
             // pushing back into the queue
              push(current_process);
             // array[current_process.sort_index].waitingTime++;
              // quantum initialzing becomes zero
              count = 0;
           }
           
          // time counter increases
          time ++;
          // sleep for a second
          sleep(1);
    
   	}
   
	// display feature to wait
	waitExit();
	}
	
