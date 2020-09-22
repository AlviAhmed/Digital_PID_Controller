#include <stdio.h>
#include <math.h>
#include <sys/mman.h>
#include <pthread.h>
#include <malloc.h>
#include <stdlib.h>
//#include "dlab.h"

//pthread_t Control;
//sem_t data_avail;
// Do not change the name of this semaphore
// Declare global variables (common data), for example:
#define MAXS 10000 // Maximum no of samples 

// Increase value of MAXS for more samples 

float theta[MAXS]; // Array for storing motor position
float ref[MAXS]; // Array for storing reference input  

/** Added variables **/ 
double Kp, Fs, run_time, magnitude, frequency, duty_cycle, 
run_time, motor_position; 
/** Added variables **/

/** Added variables for control function**/  
int k, no_of_samples = 0; 
double ek, uk, Kp = 0.0;
/** Added variables for control function**/ 



void Control();


int  main ()  
{    

Kp = 1.0; // Initialize Kp to 1.
run_time = 10.0; // Set the initial run time to 10 seconds.
Fs = 200.0; // Set the initial sampling frequency to 200 Hz. 

/* For the is variable, I put in a random number since 
this is no access to motor due to COVID-19 */
int motor_number = 5;//Set motor_number. // Check your motor module for motor_number. 

    
// Initialize the reference input vector ref[k].   
/* Added Code*/  
    int k = 0; 
    for (k = 0; k < MAXS; k++) 
    {  
        //ref[k] = 55*PI/180.0;
        ref[k] = 0.0; //KEANU, THIS MIGHT BE WRONG!
    } 
/* Added Code*/  


/* Added Code*/   
char selection = 'a'; //initializaing the selection variable
char input[10];
    while (1)  
    { 
        
/* Added Code*/  
//do  
//{ 
        printf("\n r: Run the control algorithm \n");
        printf("p: Change value of Kp \n");
        printf("f: Change value of sample frequency, Fs \n");
        printf("t: Change value of run_time, Tf \n");
        printf("u: Change the type of inputs(Step or Square) \n");
        printf("g: Plot results on screen \n");
        printf("h: Save the plot results in Postscript \n");
        printf("q: exit \n \n"); 
        scanf("%c", &selection);   
        //selection = getchar();
//} 
//while(selection == NULL || selection == " " || selection == "" || selection == '\0');

        switch (selection)  
        { 
        case 'r': 
            printf("r \n"); 
            //sem_init(&data_avail,0,0);  
            /* Added base off of  */ 
            //#define DLAB_SIMULATE 1
            //#define DLAB_HARDWARE 0 
            //extern int Initialize(int, float, int ); 
           //Initialize(DLAB_SIMULATE, Fs, motor_number);
            /* Added base off of  */  
            printf(" \n Creating Pthread \n"); 
            /* if (pthread_create (&control_thread, NULL, &Control, NULL) != 0) */
            /* { */
            /*     printf("Error Creating Control Thread"); */
            /*     exit (-1); */
            /* } */  

			//Joining the control thread
			//pthread_join(control_thread,NULL);
			//Terminate(); //DLAB function
			//sem_destroy(&data_avail);
            break;
        case 'u': 
            printf("u \n");   
            printf(" Choose Step or Square input \n"); 
            scanf("%s", input); 
			if (strcmp(input,"step")==0) //Step
			{
				printf("Enter the Magnitude of Step\n");
				scanf("%lf",&magnitude);
				for(k=0;k<MAXS;k++)
				{
						/* ref[k]=magnitude*PI/180.0; */ 
                    ref[k] = 0.0; //THIS COULD BE WRONG
				}
				
			}
			if (strcmp(input,"square")==0) //Square
			{
				printf("Enter Magnitude: \n");
				scanf("%lf", &magnitude);
				printf("Enter Frequency:\n");
				scanf("%lf", &frequency);
				printf("Enter Duty Cycle:\n");
				scanf("%lf", &duty_cycle);
				//set up {ref[k]} using DLaB function square()
				//Square(ref,MAXS,Fs,magnitude*PI/180,frequency,duty_cycle);
			}
            break;
        case 'p': 
            printf("p \n");   
            printf("Enter a value for Kp \n"); 
            scanf("%lf", &Kp);
            break; 

		case 'f':
            //New sampling frequency
            printf(" Enter a Frequency :\n");
            scanf("%lf", &Fs);
            break;
		case 't':
            //New run time
            printf("Enter Run Time :\n");
            scanf("%lf", &run_time);
            break;
		case 'g': 
            printf("g \n");
            //plot(ref,theta,Fs,(int)(run_time*Fs),SCREEN,"Graph","time","y-axis");
            break;
		case 'h':
            printf("h \n");
            //plot(ref,theta,Fs,(int)(run_time*Fs),PS,"Graph","time","y-axis");
            break; 
        case 'q': 
            printf("q \n");  
            exit(0);
            break;
        default: 
    //        printf("Invalid Selection \n");
            break;
        } 
    }
}
 

void Control() 
{ 
	k=0;
	no_of_samples = (int)(run_time*Fs);
	while(k < no_of_samples)
	{
//		sem_wait(&data_avail);
		//motor_position = EtoR(ReadEncoder());

		//calculate tracking error
		ek=ref[k] - motor_position;
		
		//calculate control value:
		uk = Kp*ek;
		
		//DtoA(VtoD(uk));
		theta[k] = motor_position;
		k++;
	}
	//Exiting
	pthread_exit(NULL);

}
