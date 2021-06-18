#include <stdio.h>
#include <stdlib.h>


/* Declare global variables & struct to hold process scheduling information */
struct node {
	
	/*these are going to be for every row in the table*/

	int id;
	int arrival;
	int total_CPU;
	//bool done; // cchnaged it to an int
	int done; //can use int if we want. Can think of this as active flag
	int start;
	int end;
	int turnaround;
 	int sort_field; //will either be a copy of arrival field or copy of CPU fileds

}*sched_table = NULL;
typedef struct node table_type;
	int num_processes;

/***************************************************************/
void print_table(table_type* table){

	/* Declare local variables */
	int i;

	//print top row of table ccontaining fild name
	printf("ID\tArrival\tTotal\tStarst\tEnd\tTurnaround \n");
	printf("--------------------------------------------------\n");

	/* Print valid contents of each row in a for-loop */
	for (i=0; i < num_processes; i++)
	{
	    
	    printf("%d\t%d\t%d\t%d\t%d\t%d\t", table[i].id,table[i].arrival,table[i].total_CPU,table[i].start,table[i].end,table[i].turnaround);

	    printf(" \n");

	}
	
	return;
}



/***************************************************************/
void parameter() {

	/* Declare local variables */
	int i;
	int id = 0;
	int arrival_time = 0;
	int  CPU_time;
	//maybe more if needed
	
	/* Prompt for total number of processes */
	printf("Enter total number of processes: ");
	scanf("%d", &num_processes);

	/* Allocate memory for sche*/
	sched_table = (table_type *)calloc(num_processes , sizeof(table_type)); //this is to building the rows * the width. calloc will allowcate memory and set everytihng to zero but we need to have , instead of * inbetween
	
	/* For each process */
	for (i=0; i < num_processes; i++)
	{
		/* Initialize fields appropriately dont need because calloc
		sched_table[i].done = 0;
		sched_table[i].start = 0;
		sched_table[i].end = 0;
		sched_table[i].turnaround = 0;*/

		/* Prompt for process id, arrival time, and total CPU time */

		printf("Enter process id: ");
		scanf("%d", &id);
		sched_table[i].id = id;

		printf("Enter arrival time for process P[%d]: ", id);
		scanf("%d", &arrival_time);
		sched_table[i].arrival = arrival_time;


		printf("Enter total CPU time for process P[%d]: ", id);
		scanf("%d", &CPU_time);
		sched_table[i].total_CPU = CPU_time;
		printf(" \n");

		
	}//end for loop

    /* Print table contents */
    
	print_table(sched_table);
	return;	
}


/***************************************************************/
table_type* sort_table(table_type *table) {
	
	/* Declare local variables */
	//Would let us barrow sort funtion from the internet

	int i,j;
	table_type temp;

	for (i = 0; i <num_processes -1; i++) //only the one with num_porccess-1
	{
		for ( j=i+1; j < num_processes; j++)
		{
		
			if (table[i].sort_field > table[j].sort_field)
			{
			temp = table[i];
			table[i] = table[j];
			table[j] = temp;
			}//end of if
		}// end for j
	}// end for i

	return table;
}


/***************************************************************/
void first_in_first_out() {

	/* Declare local variables */ //this function is for FIFO
	table_type *sorted_table;
    int i;
    
    /* Allocate memory for copy of table for sorting */
	 sorted_table = (table_type *) malloc(num_processes * sizeof(table_type));

	/* For each row in table, set sorting field to arrival field */
	for (i = 0; i <num_processes;i++)
		sched_table[i].sort_field = sched_table[i].arrival;

	/* Call "sort" function on original table and assign to table copy */
	sorted_table = sort_table(sched_table);

	/* Set field values for index 0 of sorted table based on user input values */
	sorted_table[0].start = sorted_table[0].arrival;
	sorted_table[0].end = sorted_table[0].total_CPU + sorted_table[0].start;	
	sorted_table[0].turnaround = sorted_table[0].total_CPU; //changed total_CPU from lower case to upper case
	
	/* For each additional row of sorted table, set fields appropriately */
	for (i = 1; i <num_processes; i++)
	{	
	sorted_table[i].start =  sorted_table[i-1].end; //sorted_table[i].arrival; //	//when did it start? it starts the meomet the previous one finished
	sorted_table[i].end = sorted_table[i].total_CPU + sorted_table[i].start;	
	sorted_table[i].turnaround = sorted_table[i].total_CPU + (sorted_table[i].start - sorted_table[i].arrival);//changed total_CPU from lower case to upper case
	}//end for loop

	/* Print and free sorted table */
	print_table(sorted_table);	
	free(sorted_table);
	
	return;
}


/***************************************************************/
void shortest_job_first() {
	
	/* Declare local variables */	
    table_type *sorted_table;
	int i;
	int num_processesd  = 0;
	int current_cycle = 0;
	int atleast_one;

    /* Allocate memory for copy of table for sorting */
	 sorted_table = (table_type *) malloc(num_processes * sizeof(table_type));
	
	/* For each row in table, set sorting field to total CPU field, initialize other fields appropriately */
		for (i = 0; i <num_processes;i++)
	{
		sched_table[i].sort_field = sched_table[i].total_CPU; //not sure about total_cpu //changed total_CPU from lower case to upper case
		sorted_table[i].done = 0;
	}//end for loop


	/* Call "sort" function on original table and assign to table copy */
	sorted_table = sort_table(sched_table);


	/* Loop until all processes have been scheduled */
	while (num_processesd < num_processes) {
	atleast_one = 0;
		
		printf("%d\n", num_processesd);
		
		/* For each process */
		for (i = 0; i <num_processes; i++){ //		for (i = 0; i <num_processes-1; i++){

		    
			/* If not already scheduled and (arrival time is <= current cycle) */
				if (!sorted_table[i].done && (sorted_table[i].arrival <= current_cycle)) {
				/* Set sorted table fields appropiately */
					sorted_table[i].done = 1;
					sorted_table[i].start = current_cycle; //based on the current cycle would be the starting time
					sorted_table[i].end = sorted_table[i].total_CPU + sorted_table[i].start;	
					sorted_table[i].turnaround = sorted_table[i].total_CPU + (sorted_table[i].start - sorted_table[i].arrival);//changed total_CPU from lower case to upper case
				/* Update current cycle and increment number of processes completed */
				current_cycle = sorted_table[i].end;
				num_processesd++;
				atleast_one  = 1;
				} //end if statement
			}//end for loop
		if (atleast_one == 0)
			current_cycle++;
		
	}//end while loop

	/* Print sorted table */	
	print_table(sorted_table);	
	free(sorted_table);

	return;
}


/***************************************************************/
void quit() {
/* If the table is not NULL, free the memory */
	
	printf("Quitting");
	if (sched_table != NULL) free(sched_table);
  return;
}


/***************************************************************/
int main() {

	/* Declare local variables */
	int choice;
	
	/* Until the user quits, print the menu, prompt for the menu choice, call the appropriate procedure */
while ( choice != 4)
{
    printf("Batch Scheduling\n");
	printf("--------------------------------\n");
	printf("1) Enter parameters \n");
	printf("2) Schedule processes with FIFO algorithm \n");
	printf("3) Schedule processes with SJF algorithm \n");
	printf("4) Quit program and free memory \n\n");
	
	printf("Enter selection: ");
	scanf("%d", &choice);

	
	if (choice == 1)
	{
	   parameter();
	}
	
	else if (choice == 2)
	{
	   first_in_first_out();
	}
	
	else if (choice == 3)
	{
	   shortest_job_first();
	}
	else if (choice == 4)
	{
	   quit();
	}
	
} //end while loop

  return 1;
}




