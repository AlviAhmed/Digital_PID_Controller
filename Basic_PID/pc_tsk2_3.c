#include <stdio.h>
#include "dlab_def.h"
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <math.h>

#define MAXS 6000
#define PI M_PI
float Kp, run_time, Fs, theta[MAXS], ref[MAXS], magnitude;  

/* Added for PID  */ 
float Ti, Td, N; 
float Fsq, duty_cycle;         /* Input frequency for a square wave and duty cycle*/
/* Added for PID  */ 

int motor_num, no_of_samples;
char selection;
sem_t data_avail;
pthread_mutex_t completion=PTHREAD_MUTEX_INITIALIZER;

void *Control(void *arg){
    int k=0;
    no_of_samples=(int)(run_time*Fs); 
    float Ts = 1/Fs;
    float motor_position;
    float ek = 0;   //tracking error
    float uk = 0;  //calculate control value 
    
    /* Added for PID -> dev for derivative and int for integral */ 
    float ek_prev = 0, ukint = 0, ukint_prev = 0, ukdev = 0, ukdev_prev = 0, uksum = 0; 
    /* Added for PID */
    
while(k<no_of_samples){
        sem_wait(&data_avail);
        motor_position=EtoR(ReadEncoder());
        ek=ref[k]-motor_position;
        uk=Kp*ek; 
        ukint = (ukint_prev) + ((Kp/Ti)*ek_prev*Ts); /* creating u(kt) of integral term */ 
        ukdev = ((Td)/(N*Ts+Td)*ukdev_prev)+((Kp*Td*N)/(N*Ts+Td)*(ek-ek_prev)); /* creating u(kt) of the current derivative term */ 
        uksum = uk + ukint + ukdev;
        DtoA(VtoD(uksum));
        theta[k]=motor_position;  

        /* Updating all the previous values with the current ones */
        ek_prev = ek;
        ukint_prev = ukint; 
        ukdev_prev = ukdev;
        k++;


        //sem_post(&data_avail);
	//pthread_mutex_unlock(&completion);
	if(k==no_of_samples){
	    pthread_mutex_unlock(&completion);
    	}
    }
    pthread_exit(NULL);
}

int main()
{
    pthread_t control;
    char user_input[8];
    Kp=58.2; //initializing Kp to 58.2
    run_time=10.0; //initial run time is 10.0
    Fs=200.0; //initial frequecy is 200Hz 
    motor_num=1; //motor number is 1
    magnitude=5*M_PI/18; // default angle given in lab manual (50 deg) 

    /*Init for PID  */  
    Fsq = 0.5; 
    duty_cycle = 50; 
    Ti = 0.0375;
    Td = 0.009375; 
    N = 20; 
    /* For this demo want to enage square therefore making it equal to string square manually */ 
    strcpy(user_input,"square");
    /*Init for PID  */


    // Initialize the reference input vector ref[k].    
    no_of_samples=(int)(run_time*Fs);
    int i;
    if(strcmp(user_input,"step")==0){ //if input is step, initialized ref as normally would
        for(i=0; i<no_of_samples;i++){
            ref[i]=magnitude;               
        }  
    } 
    if (strcmp(user_input, "square") == 0){ 
        Square(ref, no_of_samples, Fs, magnitude, Fsq, duty_cycle);
    }
    // Initialize the reference input vector ref[k].   

    while(1){
       
    	pthread_mutex_lock(&completion);
        printf("\t Feel free to enter a command\n");
        printf("r: Run the control algorithm \n");
        printf("p: Change value of Kp \n");
        printf("f: Change value of sample frequency, Fs \n");
        printf("t: Change value of run_time, Tf \n");
        printf("u: Change the type of inputs(Step or Square) \n");   
        printf("i: Change value of Ti\n");
        printf("d: Change value of Td\n");
        printf("n: Change value of N\n");
        printf("g: Plot results on screen \n");
        printf("h: Save the plot results in Postscript \n");
        printf("q: exit \n \n"); 
        scanf(" %c",&selection);
        switch(selection){
        case 'r':   
            printf("\n Case r \n");
            // Initialize the reference input vector ref[k].    
            no_of_samples=(int)(run_time*Fs);
            int i;
            if(strcmp(user_input,"step")==0){ //if input is step, initialized ref as normally would
                for(i=0; i<no_of_samples;i++){
                    ref[i]=magnitude;               
                }  
            } 
            if (strcmp(user_input, "square") == 0){ 
                Square(ref, no_of_samples, Fs, magnitude, Fsq, duty_cycle);
            }
            // Initialize the reference input vector ref[k].   
            sem_init(&data_avail,0,1);
            printf("\n Initializing Motor \n");
            Initialize(Fs,motor_num); 
            printf("\n Creating Thread \n \n");
            pthread_create(&control,NULL,&Control,NULL);
            pthread_join(control,NULL);
            Terminate();
            sem_destroy(&data_avail);
            break;
        case 'u':
            printf("\n Case U \n");   
            printf(" Type either step or square to choose input \n \n"); 
            scanf("%s",user_input);
            if(strcmp(user_input,"step")==0){
				printf("Enter the Magnitude of Step\n");
                scanf("%f", &magnitude);
                magnitude=magnitude*M_PI/180;//deg to rad 
                printf("New Step Magnitude = %f \n", magnitude);
            } 
            if (strcmp(user_input,"square")==0) //Square
			{
				printf("Enter Magnitude: \n");
				scanf("%f", &magnitude);
                magnitude=magnitude*M_PI/180;//deg to rad 
                printf("New Square Magnitude = %f \n", magnitude); 

                printf("Enter Square Frequency\n");
				scanf("%f", &Fsq);
                printf("New Square Frequency = %f \n", Fsq ); 
                
                printf("Enter Duty Cycle:\n");
				scanf("%f", &duty_cycle);
                printf("New Square DutyCycle = %f \n", duty_cycle); 
                Square(ref, no_of_samples, Fs, magnitude, Fsq, duty_cycle);
			}
            pthread_mutex_unlock(&completion);
            break; 
		case 'i':  
            printf(" \n Case I \n");
		    printf("Enter Ti \n");
            scanf("%f",&Ti);
            pthread_mutex_unlock(&completion);
            break;
		case 'd':
            printf(" \n Case D \n");
		    printf("Enter Td\n");
            scanf("%f",&Td);
            pthread_mutex_unlock(&completion);
            break;
		case 'n':
            printf(" \n Case N \n");
		    printf("Enter N \n");
            scanf("%f",&N);
            pthread_mutex_unlock(&completion);
            break;
        case 'p':
            printf("\n Case P \n");   
            printf("Enter new value for Kp \n"); 
            scanf("%f",&Kp); 
            printf("New Kp value = %f \n", Kp);
            pthread_mutex_unlock(&completion);
            break;
        case 'g':
            printf("\n Case G \n");
            plot(ref,theta,Fs,no_of_samples,SCREEN,"Proportional Control (Kp)","Time(sec)","Rotation(rad)");
            pthread_mutex_unlock(&completion);
            break;
        case 'h':
            printf("\n Case H \n");
            plot(ref,theta,Fs,no_of_samples,PS,"Proportional Control (Kp)","Time(sec)","Rotation(rad)");
            pthread_mutex_unlock(&completion);
            break;
        case 'q': 
            printf("\n Case Q \n");
            printf("Done! \n");
            pthread_mutex_unlock(&completion);
            exit(0);
        case 'f': 
            printf("\n Case F \n");
            printf(" Enter a new Frequency :\n");
            scanf("%f",&Fs);
            printf("New Fs value = %f \n", Fs);
		    pthread_mutex_unlock(&completion);
            break;
        case 't':
            printf("\n Case T \n");
            printf("Enter Run Time :\n");
            scanf("%f",&run_time);
            printf("New Tf value = %f \n", run_time);
            pthread_mutex_unlock(&completion);
            break;
        default:
            printf("\n Case DEFAULT \n");
            printf("Invalid\n");
		    pthread_mutex_unlock(&completion);
            break;
        }
       
    }
    return 0;
}
