#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STRING 50
#define MAX_PROCESS 100
#define MAX_INT 2147483647

struct Process
{
    int nPID;     // A
    int nArrival; // B
    int nBurst;   // C
    int nRemain;
};

struct Output
{
    int nPID;
    struct Node *pHead; // pointer to the head of the linked list
    struct Node *pTail; // pointer to the tail of the linked list
    int nWait;
};

struct Node
{
    int nStart;
    int nEnd;
    struct Node *pNext; // pointer to the next element of the linked list
};

// This function initializes the values of the elements in the output array
// sOutputs[]   The struct Output array which records the process ID, start time, end time, and wait time of the processes.
// nY           The length of the sOutputs array
void initializeOutput(struct Output sOutputs[], int nY)
{
    int i;
    for (i = 0; i < nY; i++)
    {
        sOutputs[i].pHead = NULL;
        sOutputs[i].pTail = NULL;
        sOutputs[i].nWait = 0;
    }
}

// This function pushes a node which contains the start time and end time
// *sOutput     A pointer to a struct Output which contains records the process ID, start time, end time, and wait time of a process.
// nStart       The start time of a process
// nEnd         The end time of a a process
void pushNode(struct Output *sOutput, int nStart, int nEnd)
{
    struct Node *pTemp;

    if ((pTemp = malloc(sizeof(struct Node))) == NULL) // a node cannot be created
    {
        printf("ERROR : not enough memory\n");
    }
    else
    {
        // set the values of the node
        pTemp->nStart = nStart;
        pTemp->nEnd = nEnd;
        pTemp->pNext = NULL;

        if (sOutput->pHead == NULL && sOutput->pTail == NULL) // the linked list is currently empty
        {
            // points the head and tail to the newly created node
            sOutput->pHead = pTemp;
            sOutput->pTail = pTemp;
        }
        else
        {
            sOutput->pTail->pNext = pTemp; // inserts the newly created node to the end of the linked list
            sOutput->pTail = pTemp;        // points the tail to the newly created node
        }
    }
}

// This function sort the processes according to their arrival time
// sProcesses[] The struct Process array which records the process ID, arrival time, burst time, and remaining time of the processes.
// nY           The length of the sProcesses array
void sortArrival(struct Process sProcesses[], int nY)
{
    int i, j;
    int minIdx;
    struct Process minVal;

    // stable selection sort algorithm
    for (i = 0; i < nY - 1; i++)
    {
        minIdx = i;
        for (j = i + 1; j < nY; j++)
        {
            if (sProcesses[j].nArrival < sProcesses[minIdx].nArrival)
            {
                minIdx = j;
            }
        }

        minVal = sProcesses[minIdx];
        while (i < minIdx)
        {
            sProcesses[minIdx] = sProcesses[minIdx - 1];
            minIdx--;
        }
        sProcesses[i] = minVal;
    }
}

// This function sort the processes according to their process ID
// sOutputs[]   The struct Output array which records the process ID, start time, end time, and wait time of the processes.
// nY           The length of the sOutputs array
void sortID(struct Output sOutputs[], int nY)
{
    int i, j;
    int minIdx;
    struct Output minVal;

    // stable selection sort algorithm
    for (i = 0; i < nY - 1; i++)
    {
        minIdx = i;
        for (j = i + 1; j < nY; j++)
        {
            if (sOutputs[j].nPID < sOutputs[minIdx].nPID)
            {
                minIdx = j;
            }
        }

        minVal = sOutputs[minIdx];
        while (i < minIdx)
        {
            sOutputs[minIdx] = sOutputs[minIdx - 1];
            minIdx--;
        }
        sOutputs[i] = minVal;
    }
}

// This function prints the information of the processes. Note that this function is created for testing purposes only
// sProcesses[] The struct Process array which records the process ID, arrival time, burst time, and remaining time of the processes
// nY           The length of the sProcesses array
void printProcesses(struct Process sProcesses[], int nY)
{
    int i;
    for (i = 0; i < nY; i++)
    {
        printf("Process ID: %d, Arrival Time: %d, Burst Time: %d\n", sProcesses[i].nPID, sProcesses[i].nArrival, sProcesses[i].nBurst);
    }
}

// This function prints the given information of the outputs, sorted by their process ID
// sOutputs[]   The struct Output array which records the process ID, start time, end time, and wait time of the processes
// nY           The length of the sOutputs array
void printOutput(struct Output sOutputs[], int nY)
{
    int i;

    sortID(sOutputs, nY);

    for (i = 0; i < nY; i++)
    {
        printf("P[%d] ", sOutputs[i].nPID);

        struct Node *pCurrent = sOutputs[i].pHead;
        // traverse the linked list
        while (pCurrent != NULL)
        {
            printf("Start time: %d End time: %d | ", pCurrent->nStart, pCurrent->nEnd);
            pCurrent = pCurrent->pNext;
        }
        printf("Waiting time: %d\n", sOutputs[i].nWait);
    }
}

// This function implements the First Come First Serve scheduling algorithm
// sProcesses[] The struct Process array which records the process ID, arrival time, burst time, and remaining time of the processes
// sOutputs[]   The struct Output array which records the process ID, start time, end time, and wait time of the processes
// nY           The length of the sProcesses array
void FCFS(struct Process sProcesses[], struct Output sOutputs[], int nY)
{
    int i;
    int nTotalWait = 0;
    int nStart, nEnd;

    sortArrival(sProcesses, nY);

    initializeOutput(sOutputs, nY);

    for (i = 0; i < nY; i++)
    {
        sOutputs[i].nPID = sProcesses[i].nPID;

        if (i == 0) // it is the first process to be executed
        {
            nStart = sProcesses[i].nArrival;
        }
        else // it is NOT the first process to be executed
        {
            nStart = sOutputs[i - 1].pTail->nEnd > sProcesses[i].nArrival ? sOutputs[i - 1].pTail->nEnd : sProcesses[i].nArrival; // max (sOutputs[i - 1].pTail->nEnd, sProcesses[i].nArrival)
        }
        nEnd = nStart + sProcesses[i].nBurst;
        sOutputs[i].nWait = nStart - sProcesses[i].nArrival;

        pushNode(&sOutputs[i], nStart, nEnd);

        nTotalWait += sOutputs[i].nWait;
    }

    printOutput(sOutputs, nY);
    printf("Average waiting time: %.1lf\n", 1.0 * nTotalWait / nY);
}

// This function checks if all processes have finished executing based on their remaining time
// sProcesses[] The struct Process array which records the process ID, arrival time, burst time, and remaining time of the processes
// nY           The length of the sProcesses array
// Returns 1 if finished; otherwise, returns 0
int isFinished(struct Process sProcesses[], int nY)
{
    int i;
    for (i = 0; i < nY; i++)
    {
        if (sProcesses[i].nRemain != 0) // the process is finished if its remaining time is 0
            return 0;                   // 0 - not yet finished
    }

    return 1; // 1 - is finished
}

// This function determines the process with the minimum remaining time
// sProcesses[] The struct Process array which records the process ID, arrival time, burst time, and remaining time of the processes
// nY           The length of the sProcesses array
// nCurrent     The current time counter of the CPU
// Returns the index of the process with the minimum remaining time
int getMinRemain(struct Process sProcesses[], int nY, int nCurrent)
{
    int i;
    int minVal = MAX_INT;
    int minIdx = -1;

    for (i = 0; i < nY; i++)
    {
        // the following conditions must be satisfied before being considered for the minimum remaining time
        // (1) the process has already arrived
        // (2) the process has not finished executing
        // (3) the remaining time of the process is less than the current minimum remaining time
        if (sProcesses[i].nArrival <= nCurrent && sProcesses[i].nRemain > 0 && sProcesses[i].nRemain < minVal)
        {
            minIdx = i;
            minVal = sProcesses[i].nRemain;
        }
    }

    return isFinished(sProcesses, nY) ? -1 : minIdx; //-1 means all processes have 0 remaining time (i.e., is finished) or no process can be executed at the current time
}

// This function initializes the remain variables in the sProcesses[]
// sProcesses[] The struct Process array which records the process ID, arrival time, burst time, and remaining time of the processes
// nY           The length of the sProcesses array
void initializeRemain(struct Process sProcesses[], int nY)
{
    int i;
    for (i = 0; i < nY; i++)
    {
        sProcesses[i].nRemain = sProcesses[i].nBurst;
    }
}

// This function implements the Shortest Job First scheduling algorithm
// sProcesses[] The struct Process array which records the process ID, arrival time, burst time, and remaining time of the processes
// sOutputs[]   The struct Output array which records the process ID, start time, end time, and wait time of the processes
// nY           The length of the sProcesses array
void SJF(struct Process sProcesses[], struct Output sOutputs[], int nY)
{
    int nCurrIdx;
    int nCurrTime = 0;
    int nTotalWait = 0;
    int nStart, nEnd;

    sortArrival(sProcesses, nY);

    initializeOutput(sOutputs, nY);
    initializeRemain(sProcesses, nY);

    while (!isFinished(sProcesses, nY)) // repeats the loop while there is a process that has not yet finished executing
    {
        nCurrIdx = getMinRemain(sProcesses, nY, nCurrTime); // gets the index of the process to be executed
        if (nCurrIdx >= 0)                                  // a process can be executed at the current time
        {
            sOutputs[nCurrIdx].nPID = sProcesses[nCurrIdx].nPID;
            nStart = nCurrTime;
            nEnd = nStart + sProcesses[nCurrIdx].nBurst;

            nCurrTime += sProcesses[nCurrIdx].nBurst;                    // adds the burst time to the current time
            sProcesses[nCurrIdx].nRemain -= sProcesses[nCurrIdx].nBurst; // subtract the burst time that was executed from the remaining time

            pushNode(&sOutputs[nCurrIdx], nStart, nEnd);

            sOutputs[nCurrIdx].nWait = nStart - sProcesses[nCurrIdx].nArrival;
            nTotalWait += sOutputs[nCurrIdx].nWait;
        }
        else // no process can be executed at the current time
        {
            nCurrTime++;
        }
    }

    printOutput(sOutputs, nY);
    printf("Average waiting time: %.1lf\n", 1.0 * nTotalWait / nY);
}

// This function implements the Shortest Remaining Time First scheduling algorithm
// sProcesses[] The struct Process array which records the process ID, arrival time, burst time, and remaining time of the processes
// sOutputs[]   The struct Output array which records the process ID, start time, end time, and wait time of the processes
// nY           The length of the sProcesses array
void SRTF(struct Process sProcesses[], struct Output sOutputs[], int nY)
{
    int nCurrIdx;
    int nCurrTime = 0;
    int nTotalWait = 0;
    int nBurstTime;
    int nStart, nEnd;

    sortArrival(sProcesses, nY);

    initializeOutput(sOutputs, nY);
    initializeRemain(sProcesses, nY);

    while (!isFinished(sProcesses, nY)) // repeats the loop while there is a process that has not yet finished executing
    {
        nCurrIdx = getMinRemain(sProcesses, nY, nCurrTime); // gets the index of the process to be executed
        if (nCurrIdx >= 0)                                  // a process can be executed at the current time
        {
            sOutputs[nCurrIdx].nPID = sProcesses[nCurrIdx].nPID;
            nStart = nCurrTime;

            nBurstTime = 0;

            while (getMinRemain(sProcesses, nY, nCurrTime) == nCurrIdx) // repeats the loop while the process to be executed is the same after increasing the time counter by 1
            {
                nCurrTime++;
                nBurstTime++;
                sProcesses[nCurrIdx].nRemain--;
            }

            nEnd = nStart + nBurstTime;

            pushNode(&sOutputs[nCurrIdx], nStart, nEnd);

            if (sProcesses[nCurrIdx].nRemain == 0) // computes the wait time when the process has fully finished executing
            {
                sOutputs[nCurrIdx].nWait = sOutputs[nCurrIdx].pTail->nEnd - (sProcesses[nCurrIdx].nArrival + sProcesses[nCurrIdx].nBurst);
                nTotalWait += sOutputs[nCurrIdx].nWait;
            }
        }
        else // no process can be executed at the current time
        {
            nCurrTime++;
        }
    }

    printOutput(sOutputs, nY);
    printf("Average waiting time: %.1lf\n", 1.0 * nTotalWait / nY);
}

int getProcessIdx(struct Process sProcesses[], int nY, struct Process sProcess)
{
    int i;
    for (i = 0; i < nY; i++)
    {
        // printf("sProcesses[i].nPID %d", sProcesses[i].nPID);
        // printf("sProcess.nPID %d", sProcess.nPID);
        if (sProcesses[i].nPID == sProcess.nPID)
        {
            return i;
        }
    }
    return -1;
}

// This function implements the Round Robin scheduling algorithm
// sProcesses[] The struct Process array which records the process ID, arrival time, burst time, and remaining time of the processes
// sOutputs[]   The struct Output array which records the process ID, start time, end time, and wait time of the processes
// nY           The length of the sProcesses array
// nZ           The time slice value
void RR(struct Process sProcesses[], struct Output sOutputs[], int nY, int nZ)
{
    int nHeadIdx = 0;   // pointer to the head of the queue
    int nTailIdx = 0;   // pointer to the tail of the queue
    int nArriveIdx = 0; // index of the last element that arrived
    int nCurrIdx;
    int nCurrTime = 0;
    int nTotalWait = 0;
    int nBurstTime;
    int nStart, nEnd;

    int i;

    // the maximum capacity of the queue using this implementation is N-1
    // since there may be at most nY process in the queue, the size of the queue is thus set to nY+1
    struct Process *pQueue = malloc(sizeof(struct Process) * (nY + 1));

    sortArrival(sProcesses, nY);

    initializeOutput(sOutputs, nY);
    initializeRemain(sProcesses, nY);

    while (!isFinished(sProcesses, nY)) // repeats the loop while there is a process that has not yet finished executing
    {

        // check if a new process has arrived (NOTE: SAME CODE AS BELOW)
        for (i = nArriveIdx; i < nY; i++)
        {
            if (sProcesses[i].nArrival <= nCurrTime)
            {
                pQueue[nTailIdx] = sProcesses[i];
                nTailIdx = (nTailIdx + 1) % (nY + 1);
                nArriveIdx++;
            }
        }

        if (nHeadIdx != nTailIdx) // the queue is not empty
        {

            nCurrIdx = getProcessIdx(sProcesses, nY, pQueue[nHeadIdx]); // find the index of the current process in the sProcesses array
            nHeadIdx = (nHeadIdx + 1) % (nY + 1);

            sOutputs[nCurrIdx].nPID = sProcesses[nCurrIdx].nPID;
            nStart = nCurrTime;

            if (sProcesses[nCurrIdx].nRemain > nZ)
            {
                nBurstTime = nZ;
            }
            else // the remaining time is less than the time slice, thus the burst time is equal to the remaining time
            {
                nBurstTime = sProcesses[nCurrIdx].nRemain;
            }

            nEnd = nStart + nBurstTime;
            sProcesses[nCurrIdx].nRemain -= nBurstTime; // subtract the burst time that was executed from the remaining time
            nCurrTime += nBurstTime;

            pushNode(&sOutputs[nCurrIdx], nStart, nEnd);

            // check if a new process has arrived (NOTE: SAME CODE AS ABOVE)
            for (i = nArriveIdx; i < nY; i++)
            {
                if (sProcesses[i].nArrival <= nCurrTime)
                {
                    pQueue[nTailIdx] = sProcesses[i];
                    nTailIdx = (nTailIdx + 1) % (nY + 1);
                    nArriveIdx++;
                }
            }

            // add the process to the back of the queue if it is not yet finished
            if (sProcesses[nCurrIdx].nRemain > 0)
            {
                pQueue[nTailIdx] = sProcesses[nCurrIdx];
                nTailIdx = (nTailIdx + 1) % (nY + 1);
            }

            if (sProcesses[nCurrIdx].nRemain == 0) // computes the wait time when the process has fully finished executing
            {
                sOutputs[nCurrIdx].nWait = sOutputs[nCurrIdx].pTail->nEnd - (sProcesses[nCurrIdx].nArrival + sProcesses[nCurrIdx].nBurst);
                nTotalWait += sOutputs[nCurrIdx].nWait;
            }
        }
        else // the queue is empty; no process can be executed at the current time
        {
            nCurrTime++;
        }
    }

    printOutput(sOutputs, nY);
    printf("Average waiting time: %.1lf\n", 1.0 * nTotalWait / nY);
}

int main()
{
    char sFilename[MAX_STRING];
    int i, nX, nY, nZ;
    FILE *pInput;
    struct Process sProcesses[MAX_PROCESS];
    struct Output sOutputs[MAX_PROCESS];

    printf("Input Filename: ");
    fgets(sFilename, MAX_STRING, stdin);

    sFilename[strlen(sFilename) - 1] = '\0'; // replace \n with \0
    strcat(sFilename, ".txt");

    if ((pInput = fopen(sFilename, "r")) != NULL) // the file exists
    {

        // reads the value of X, Y, and Z from the text file
        fscanf(pInput, "%d %d %d", &nX, &nY, &nZ);

        if (nX != 3)
        {
            nZ = 1;
        }

        // stores the process ID, arrival time, and burst time to the process array
        for (i = 0; i < nY; i++)
        {
            fscanf(pInput, "%d %d %d", &sProcesses[i].nPID, &sProcesses[i].nArrival, &sProcesses[i].nBurst);
        }

        // Prints the contents of the processes (for testing purposes)
        // printProcesses(sProcesses, nY);

        switch (nX)
        {
        case 0:
            FCFS(sProcesses, sOutputs, nY);
            break;
        case 1:
            SJF(sProcesses, sOutputs, nY);
            break;
        case 2:
            SRTF(sProcesses, sOutputs, nY);
            break;
        case 3:
            RR(sProcesses, sOutputs, nY, nZ);
            break;
        default:
            printf("Invalid CPU Scheduling Algorithm");
        }

        fclose(pInput);
    }
    else
    {
        printf("%s not found.", sFilename);
    }
    return 0;
}
