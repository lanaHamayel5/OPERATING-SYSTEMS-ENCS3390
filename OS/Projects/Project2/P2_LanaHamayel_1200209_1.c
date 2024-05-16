//Lana Hamayel
//1200209
//section 1
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include <stdio.h>
#include <limits.h>
#include <process.h>

typedef struct {
    int PID;
    int BurstTime;
    int ArrivalTime;
    int RemainingTime;
    int ready;
    int priority;
    int IsStarted;
    int comesBackAfter;
    int newArrivalTime;
    int finish;
    int completionTime;
    int waitingTime;
    int responseTime;
    int turnRoundTime;
} process;
typedef struct {
    double averageWaitingTime;
    double averageTurnaroundTime;
    double averageResponseTime;
} Values;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Functions Prototypes
void copyProcesses(process[], process[], int);

void RoundRobinAlgorithm(process[], int, double *, double *, double *);

void FirstComeFirstServed(process[], int, double *, double *, double *);

void priorityScheduling(process[], int, double *, double *, double *);

void prioritySchedulingNonPreemptive(process[], int, double *, double *, double *);

void ShortestJopFirst(process[], int, double *, double *, double *);

void ShortestRemainingJopFirst(process[], int, double *, double *, double *);

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int main() {
    printf("\n\t\t\t\t Scheduling Algorithms Simulator Program\n");
    printf("--------------------------------------------------------------------------------------------------------");
    int n = 7;
    process p[n];

    int pid[] = {1, 2, 3, 4, 5, 6, 7};
    int arrivalTimes[] = {0, 1, 3, 4, 6, 7, 8};
    int burstTimes[] = {10, 8, 14, 7, 5, 4, 6};
    int comesBackAfter[] = {2, 4, 6, 8, 3, 6, 9};
    int priorities[] = {3, 2, 3, 1, 0, 1, 2};
    //store the processes information
    for (int i = 0; i < n; i++) {
        p[i].PID = pid[i];
        p[i].ArrivalTime = arrivalTimes[i];
        p[i].BurstTime = burstTimes[i];
        p[i].comesBackAfter = comesBackAfter[i];
        p[i].priority = priorities[i];
        p[i].RemainingTime = p[i].BurstTime;
        p[i].ready = 0;
        p[i].IsStarted = 0;
    }
    //calling the copiedProcess function
    process copiedProcesses[n];
    Values v[6], result;
    result.averageResponseTime = 0;
    result.averageTurnaroundTime = 0;
    result.averageWaitingTime = 0;
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    int choiceNum;
    //show menu;
    while (1) {
        printf("\nChoose a CPU scheduling algorithm to be executed:\n");
        printf("1. First Come First Served.\n");
        printf("2. Shortest Job First.\n");
        printf("3. Shortest Remaining Time First.\n");
        printf("4. Round Robin (q = 5).\n");
        printf("5. Preemptive Priority Scheduling with Aging.\n");
        printf("6. Non-preemptive Priority Scheduling with Aging.\n");
        printf("7. Exit.\n\n");
        printf("Enter your choice: ");
        if (scanf("%d", &choiceNum) == 1) {
            // Input is an integer
            if (choiceNum > 7 || choiceNum < 1)
            {
                printf("Invalid choice. Try again\n");
                printf("----------------------------------------------------------------------------------\n");
                continue;  // Skip the rest of the loop and go to the next iteration
            }
            //printf("You entered: %d\n", choiceNum);
        } else {
            // Input is not an integer
            printf("Invalid input. Please enter an integer.\n");
            // Clear the input buffer to prevent issues
            while (getchar() != '\n');
        }
        printf("----------------------------------------------------------------------------------\n");
        // switch statement to execute user choice
        switch (choiceNum) {
            case 1:
                copyProcesses(p, copiedProcesses, n);
                FirstComeFirstServed(copiedProcesses, n, &v[0].averageResponseTime, &v[0].averageTurnaroundTime,
                                     &v[0].averageWaitingTime);
                break;
            case 2:
                copyProcesses(p, copiedProcesses, n);
                ShortestJopFirst(copiedProcesses, n, &v[4].averageResponseTime, &v[4].averageTurnaroundTime,
                                 &v[4].averageWaitingTime);
                break;
            case 3:
                copyProcesses(p, copiedProcesses, n);
                ShortestRemainingJopFirst(copiedProcesses, n, &v[5].averageResponseTime, &v[5].averageTurnaroundTime,
                                          &v[5].averageWaitingTime);
                break;
            case 4:
                copyProcesses(p, copiedProcesses, n);
                RoundRobinAlgorithm(copiedProcesses, n, &v[1].averageResponseTime, &v[1].averageTurnaroundTime,
                                    &v[1].averageWaitingTime);
                break;
            case 5:
                copyProcesses(p, copiedProcesses, n);
                priorityScheduling(copiedProcesses, n, &v[2].averageResponseTime, &v[2].averageTurnaroundTime,
                                   &v[2].averageWaitingTime);
                break;
            case 6:
                copyProcesses(p, copiedProcesses, n);
                prioritySchedulingNonPreemptive(copiedProcesses, n, &v[3].averageResponseTime,
                                                &v[3].averageTurnaroundTime,
                                                &v[3].averageWaitingTime);
                break;
            case 7:
                printf("\nThanks for your time!\n");
                printf("------------------------------------------------------------------------------------------------------");
                exit(0);

            default:
                printf("Invalid choice\n");
        }
    }
    return 0;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Function used to copy the original information of the set of processes.
void copyProcesses(process source[], process destination[], int n) {
    for (int i = 0; i < n; i++) {
        destination[i] = source[i];
    }
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Round Robin Algorithm function
void RoundRobinAlgorithm(process p[], int n, double *a, double *b, double *c) {
    printf("\t\t\t<< Round Robin CPU Scheduling Algorithm >>\n");
    printf("---------------------------------------------------------------------------------------------------------\n");
    int quantum = 5, completed = 0, currentTime = 0;
    int over = 0;
    int waitingTime = 0;
    int turnaroundTime;
    // Flag to track if a process is executed in the current time step
    int processExecuted = 0;
    int count = 5;
    while (currentTime < 200) {

        // Sort the processes based on arrival time
        for (int i = 0; i < n - 1; i++) {
            for (int j = 0; j < n - i - 1; j++) {
                if (p[j].ArrivalTime > p[j + 1].ArrivalTime) {
                    process temp = p[j];
                    p[j] = p[j + 1];
                    p[j + 1] = temp;
                }
            }

            for (int i = 0; i < n; i++) {
                if (p[i].RemainingTime > 0 && p[i].ready == 0 && p[i].ArrivalTime <= currentTime) {
                    if (p[i].RemainingTime <= quantum) {
                        if (p[i].IsStarted == 0) {
                            int responseTime = currentTime - p[i].ArrivalTime;
                            *a += responseTime;
                            p[i].IsStarted = 1;
                        }
                        currentTime += p[i].RemainingTime;
                        p[i].RemainingTime = 0;
                        p[i].ready = 1;
                        completed++;
                        if (currentTime > 200) {
                            over = currentTime - 200;
                            over = currentTime - 200;
                            currentTime = currentTime - over;
                            //printf("The process %d is complete executed at time %d.\n", p[3].PID, currentTime);
                            //completed++;
                            break;
                        }
                        printf("The process %d is complete executed at time %d.\n", p[i].PID, currentTime);
                        processExecuted = 1;
                        //adding avg waiting time
                        waitingTime = currentTime - p[i].BurstTime - p[i].ArrivalTime;
                        *c += waitingTime;
                        turnaroundTime = currentTime - p[i].ArrivalTime;
                        *b += turnaroundTime;
                        //restore the process with newArrivalTime
                        p[i].ArrivalTime = currentTime + p[i].comesBackAfter;
                        p[i].RemainingTime = p[i].BurstTime;
                        p[i].ready = 0;
                        p[i].IsStarted = 0;

                    } else {
                        if (p[i].IsStarted == 0) {
                            int responseTime = currentTime - p[i].ArrivalTime;
                            *a += responseTime;
                            p[i].IsStarted = 1;
                        }
                        currentTime += quantum;
                        p[i].RemainingTime -= quantum;
                        processExecuted = 1;
                        if (currentTime > 200) {
                            over = currentTime - 200;
                            over = currentTime - 200;
                            currentTime = currentTime - over;
                            //printf("The process %d is complete executed at time %d.\n", p[i].PID, currentTime);
                            completed++;
                            break;
                        }
                        printf("The process %d is still running at time %d.\n", p[i].PID, currentTime);
                        count--;
                        if (count == 1) {
                            currentTime += p[count - 1].RemainingTime;
                            p[count - 1].RemainingTime = 0;
                            p[count - 1].ready = 1;
                            processExecuted = 1;
                            p[count - 1].IsStarted = 1;
                            printf("The process %d is complete executed at time %d.\n", p[count - 1].PID, currentTime);
                            p[count - 1].ArrivalTime = currentTime + p[count - 1].comesBackAfter;
                            p[count - 1].RemainingTime = p[count - 1].BurstTime;
                            p[count - 1].ready = 0;
                            p[count - 1].IsStarted = 0;
                        }
                    }
                }

            }

            if (processExecuted == 0) {
                currentTime++;
                printf("Until time %d, there are no processes arrived 'IDLE'.\n", currentTime);
            }
        }
    }
    //calculate the avg waiting time
    *a = *a / 6;
    *c = *c / 6;
    *b = *b / 6;

    //printf("\nAverage Response Time: %.2f.\n", *a);
    printf("\nAverage Waiting Time: %.2f.\n", *c);
    printf("Average Turnaround Time: %.2f.\n", *b);
    printf("---------------------------------------------------------------------------------------------------------\n");
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//First Come, First Served algorithm function
void FirstComeFirstServed(process p[], int n, double *a, double *b, double *c) {
    printf("\t\t\t<< First Come First Served CPU Scheduling Algorithm >>\n");
    printf("---------------------------------------------------------------------------------------------------------\n");
    int currentTime = 0, completed = 0;
    int count = 0;
    // Sort the processes based on arrival time
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (p[j].ArrivalTime > p[j + 1].ArrivalTime) {
                process temp = p[j];
                p[j] = p[j + 1];
                p[j + 1] = temp;
            }
        }
    }
    while (currentTime < 200) {
        for (int i = 0; i < n; i++) {
            if (p[i].ArrivalTime <= currentTime && p[i].ready == 0) {
                if (p[i].IsStarted == 0) {
                    int responseTime = currentTime - p[i].ArrivalTime;
                    *a += responseTime;
                    p[i].IsStarted = 1;
                }
                currentTime += p[i].RemainingTime;
                *c += currentTime - p[i].ArrivalTime - p[i].BurstTime;
                *b += currentTime - p[i].ArrivalTime;
                p[i].ready = 1;
                completed++;

                if (currentTime > 200) {
                    //to stop at 200 and found how to the last process takes from it burst time
                    int over = 0;
                    over = currentTime - 200;
                    *b += currentTime - p[i].ArrivalTime + over;
                    currentTime -= over;
                    printf("The process %d is complete executed at time %d.\n", p[i].PID, currentTime);
                    break;
                }
                printf("The process %d is complete executed at time %d.\n", p[i].PID, currentTime);
                //restore the process with newArrivalTime
                p[i].ArrivalTime = currentTime + p[i].comesBackAfter;
                p[i].RemainingTime = p[i].BurstTime;
                p[i].ready = 0;
                p[i].IsStarted = 0;
            } else {
                currentTime++;
                printf("Until time %d, there no processes arrived 'IDLE'.\n", currentTime);
                i = i - 1;
            }

        }
    }
//calculating avg and turnAround Time
    *c = *c / n;
    *a = *a / n;
    *b = *b / n;

    // printf("\nAverage Response Time: %.2f.\n",*a);
    printf("\nAverage Waiting Time: %.2f\n", *c);
    printf("Average Turnaround Time: %.2f\n", *b);
    printf("-------------------------------------------------------------------------------------------------------------\n\n");
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//priority Scheduling function
void priorityScheduling(process p[], int n, double *a, double *b, double *c) {
    int completed = 0;
    int currentTime = 0;

    // Sort the processes based on arrival time
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (p[j].ArrivalTime > p[j + 1].ArrivalTime) {
                process temp = p[j];
                p[j] = p[j + 1];
                p[j + 1] = temp;
            }
        }
    }

    int currentProcess = -1;  // Variable to track the currently executing process

    while (completed < n) {
        int selectedProcess = -1;
        int highestPriority = 9999;

        for (int i = 0; i < n; i++) {
            if (p[i].ArrivalTime <= currentTime && p[i].ready == 0 && p[i].priority < highestPriority) {
                highestPriority = p[i].priority;
                selectedProcess = i;
            }
        }

        if (selectedProcess != -1) {
            if (p[selectedProcess].IsStarted == 0) {
                int responseTime = currentTime - p[selectedProcess].ArrivalTime;
                *a += responseTime;
                p[selectedProcess].IsStarted = 1;

            }
            if (currentProcess != -1 && p[selectedProcess].priority < p[currentProcess].priority) {

                // Preempt the current process.
                printf("The process %d is preempted at time %d.\n", p[currentProcess].PID, currentTime);

                p[currentProcess].ready = 0;  // Mark the preempted process as not completed
            }

            currentProcess = selectedProcess;  // Update the current process
            currentTime++;
            p[currentProcess].RemainingTime--;

            if (p[currentProcess].RemainingTime == 0) {
                // The current process has completed execution
                completed++;
                *c += currentTime - p[selectedProcess].ArrivalTime - p[selectedProcess].BurstTime;
                *b += currentTime - p[selectedProcess].ArrivalTime;
                p[currentProcess].ready = 1;
                printf("The process %d is complete executed at time %d.\n", p[currentProcess].PID, currentTime);
                currentProcess = -1;  // Reset the current process
            }
        } else {
            currentTime++;
            printf("Until time %d, there's no processes arrived 'IDEL'.\n", currentTime);
        }
        for (int i = 0; i < n; i++) {
            if (p[i].ArrivalTime <= currentTime && p[i].RemainingTime > 0) {
                p[i].priority--;
                // Check if priority becomes negative, reset it to 0
                if (p[i].priority < 0) {
                    p[i].priority = 0;
                }
            }
        }
    }
    *a = *a / n;
    *c = *c / n;
    *b = *b / n;
   // printf("\nAverage Response Time: %.2f.\n", *a);
    printf("Average Waiting Time: %.2f.\n", *c);
    printf("Average Turnaround Time: %.2f.\n", *b);
    printf("---------------------------------------------------------------------------------------------------------\n");
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// priority Scheduling Non Preemptive function
void prioritySchedulingNonPreemptive(process p[], int n, double *a, double *b, double *c) {
    int current_time = 0;
    int completed_processes = 0;
    int total_turnaround_time = 0;
    int total_waiting_time = 0;
    int total_response_time = 0;
    printf("\t\t\t<< Priority Scheduling Non Preemptive CPU Scheduling Algorithm >>\n");
    printf("---------------------------------------------------------------------------------------------------------\n");
    int j = 0, time = 0;
    for (int i = 0; i < n; i++) {
        p[i].RemainingTime = p[i].BurstTime;
        p[i].waitingTime = 0;
        p[i].turnRoundTime = 0;
        p[i].responseTime = -1;
        p[i].completionTime = 0;
    }
    // while (time <n) {

    while (completed_processes < n) {
        int highest_priority = 9999;
        int highest_process = -1;

        // Looking for the highest priority process
        for (int i = 0; i < n; i++) {
            if (p[i].ArrivalTime <= current_time && highest_priority > p[i].priority && p[i].RemainingTime > 0) {
                highest_priority = p[i].priority;
                highest_process = i;
            }
        }

        if (highest_process != -1) {
            completed_processes++;

            p[highest_process].completionTime = current_time + p[highest_process].BurstTime;
            p[highest_process].turnRoundTime = p[highest_process].completionTime - p[highest_process].ArrivalTime;
            p[highest_process].waitingTime = p[highest_process].turnRoundTime - p[highest_process].BurstTime;
            current_time += p[highest_process].BurstTime;
            total_turnaround_time += p[highest_process].turnRoundTime;
            total_waiting_time += p[highest_process].waitingTime;
            printf("The process %d is complete executed at time %d.\n", p[highest_process].PID, current_time);
            p[highest_process].RemainingTime = 0;
            time = current_time;
        } else {
            current_time++;
            time = current_time;

            continue;
        }

        for (int i = 0; i < n; i++) {
            if (p[i].ArrivalTime <= current_time && p[i].RemainingTime > 0) {
                p[i].priority--;
                // Check if priority becomes negative, reset it to 0
                if (p[i].priority < 0) {
                    p[i].priority = 0;
                }
            }
        }
    }
    // Calculate average performance metrics
    double avg_turnaround_time = (double) total_turnaround_time / n;
    double avg_waiting_time = (double) total_waiting_time / n;

    // Print the average performance metrics
    printf("\nAverage Turnaround Time: %.2lf\n", avg_turnaround_time);
    printf("Average Waiting Time: %.2lf\n", avg_waiting_time);
    printf("-----------------------------------------------------------------------------------------------------------\n");

}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Shortest Jop First function
void ShortestJopFirst(process p[], int n, double *a, double *b, double *c) {
    int current_time = 0;
    int size = n;
    int over;
    int completed_processes = 0;
    process waitingQueue[size];
    printf("\t\t\t<< Shortest Job First CPU Scheduling Algorithm >>\n");
    printf("---------------------------------------------------------------------------------------------------------\n");

    for (int i = 0; i < size; i++) {
        p[i].waitingTime = 0;
        p[i].turnRoundTime = 0;
        p[i].responseTime = -1;
        p[i].completionTime = 0;
        p[i].RemainingTime = p[i].BurstTime;
        p[i].newArrivalTime = 0;
        //waitingQueue[i] = p[i];
    }
    while (current_time < 200) {

        int shortest_process = -1;
        int shortest_burst_time = INT_MAX;
        //looking for the shortest process
        for (int i = 0; i < size; i++) {
            if (p[i].ArrivalTime <= current_time && p[i].RemainingTime < shortest_burst_time &&
                p[i].RemainingTime > 0) {
                shortest_burst_time = p[i].RemainingTime;
                shortest_process = i;
            }
        }
        if (shortest_process != -1) {
            p[shortest_process].completionTime = current_time + p[shortest_process].BurstTime;
            p[shortest_process].turnRoundTime = p[shortest_process].completionTime - p[shortest_process].ArrivalTime;
            p[shortest_process].waitingTime = p[shortest_process].turnRoundTime - p[shortest_process].BurstTime;
            p[shortest_process].responseTime = p[shortest_process].waitingTime;
            p[shortest_process].RemainingTime = 0;

            *b += p[shortest_process].turnRoundTime + p[shortest_process].BurstTime;
            *c += p[shortest_process].waitingTime;
            *a += p[shortest_process].responseTime;
            current_time += p[shortest_process].BurstTime;
            //checking if the current time is =200 or not
            if (current_time > 200) {
                over = current_time - 200;
                current_time = current_time - over;
                *b += p[shortest_process].turnRoundTime + p[shortest_process].BurstTime;
                printf("The process %d is complete executed at time %d.\n", p[shortest_process].PID, current_time);
                completed_processes++;
                break;
            }
            completed_processes++;
            printf("The process %d is complete executed at time %d.\n", p[shortest_process].PID,
                   current_time);

            p[shortest_process].RemainingTime = p[shortest_process].BurstTime;
            p[shortest_process].ArrivalTime = current_time + p[shortest_process].comesBackAfter;

        }
    }
    //calculating the avg waiting and turnaround
    *b = *b / 4;
    *c = *c / 4;
    *a = *a / 4;
    printf("\nAverage Waiting Time: %d(Infinity)\n", INT_MAX);
    printf("Average Turnaround Time: %.2lf\n", *b);
    //printf("Average Response Time: %.2lf\n", *a);
    printf("---------------------------------------------------------------------------------------------------------------------\n");
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Shortest Remaining Jop First function
void ShortestRemainingJopFirst(process p[], int n, double *a, double *b, double *c) {
    int over = 0;
    int current_time = 0;
    int completed_processes = 0;
    int total_turnaround_time = 0;
    int total_waiting_time = 0;
    int total_response_time = 0;
    printf("\n");
    printf("\t\t\t<< Shortest Remaining Job First CPU Scheduling Algorithm >>\n");
    printf("---------------------------------------------------------------------------------------------------------\n");

    for (int i = 0; i < n; i++) {
        p[i].RemainingTime = p[i].BurstTime;
        p[i].waitingTime = 0;
        p[i].turnRoundTime = 0;
        p[i].responseTime = -1;
        p[i].completionTime = 0;
    }

    while (current_time <= 200) {
        int shortest_remaining = INT_MAX;
        int shortest_process = -1;

        // Looking for the highest priority process
        for (int i = 0; i < n; i++) {
            if (current_time < p[i].ArrivalTime) {
                continue;
            } else {
                if (shortest_remaining > p[i].RemainingTime && p[i].RemainingTime > 0) {
                    shortest_remaining = p[i].RemainingTime;
                    shortest_process = i;
                }
            }
        }
        printf("The process %d is starting to execute at time %d.\n", p[shortest_process].PID, (current_time));

        //printf("The process %d is complete executed at time %d.\n", p[shortest_process].PID, (current_time+1));
        if (shortest_process != -1) {
            if (p[shortest_process].responseTime == -1) {
                p[shortest_process].responseTime = current_time;
                total_response_time += p[shortest_process].responseTime;
            }

            p[shortest_process].RemainingTime--;
            int finish = current_time;
            if (p[shortest_process].RemainingTime == 0) {
                completed_processes++;
                p[shortest_process].completionTime = current_time + 1;
                if (p[shortest_process].completionTime > 200) {
                    over = p[shortest_process].completionTime - 200;
                    p[shortest_process].completionTime = p[shortest_process].completionTime - over;
                    printf("The process %d is complete executed at time %d.\n", p[shortest_process].PID, current_time);
                    *b += p[shortest_process].turnRoundTime + p[shortest_process].comesBackAfter;
                    *c += p[shortest_process].waitingTime;
                    break;
                }
                printf("\nThe process %d is complete executed at time %d.\n", p[shortest_process].PID,
                       p[shortest_process].completionTime);

                p[shortest_process].turnRoundTime =
                        p[shortest_process].completionTime - p[shortest_process].ArrivalTime;
                p[shortest_process].waitingTime = p[shortest_process].turnRoundTime - p[shortest_process].BurstTime;
                *b += p[shortest_process].turnRoundTime + p[shortest_process].comesBackAfter;
                *c += p[shortest_process].waitingTime;

                p[shortest_process].RemainingTime = p[shortest_process].BurstTime;
                p[shortest_process].ArrivalTime = (finish + 2) + p[shortest_process].comesBackAfter;

            }

        }
        current_time++;
    }

    // Calculate average performance metrics
    *b = *b / 5;
    *c = *c / 4;
    *a = *a / 5;


    // Print the average performance metrics
    printf("\nAverage Turnaround Time: %.2lf\n", *b);
    printf("Average Waiting Time: %d(Infinity)\n)", INT_MAX);
    //printf("Average Response Time: %.2lf\n", *a);
    printf("---------------------------------------------------------------------------------------------------------------------\n\n");

}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
