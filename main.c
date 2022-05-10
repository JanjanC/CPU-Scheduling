#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STRING 25
#define MAX_PROCESS 100
#define MAX_INT 2147483647

struct Process
{
    int nPID;     // A
    int nArrival; // B
    int nBurst;   // C
    int nRemain;
};

// struct Output
// {
//     int nPID;
//     int nStart;
//     int nEnd;
//     int nWait;
//     struct Output *pNext;
// };

struct Output
{
    int nPID;
    struct Node *pHead;
    struct Node *pTail;
    int nWait;
};

struct Node
{
    int nStart;
    int nEnd;
    struct Node *pNext;
};

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

void pushNode(struct Output *sOutput, int nStart, int nEnd)
{
    struct Node *pTemp;
    if ((pTemp = malloc(sizeof(struct Node))) == NULL)
    {
        printf("ERROR : not enough memory\n");
    }
    else
    {
        pTemp->nStart = nStart;
        pTemp->nEnd = nEnd;
        pTemp->pNext = NULL;

        if (sOutput->pHead == NULL && sOutput->pTail == NULL)
        {
            sOutput->pHead = pTemp;
            sOutput->pTail = pTemp;
        }
        else
        {
            sOutput->pTail->pNext = pTemp;
            sOutput->pTail = pTemp;
        }
    }
}

// sort by burst time, break
void sortBurst(struct Process sProcesses[], int nY)
{
    int i, j;
    struct Process sTemp;
    for (i = 0; i < nY - 1; i++)
    {
        for (j = i + 1; j < nY; j++)
        {
            if (sProcesses[i].nBurst > sProcesses[j].nBurst)
            {
                sTemp = sProcesses[i];
                sProcesses[i] = sProcesses[j];
                sProcesses[j] = sTemp;
            }
            else if (sProcesses[i].nBurst == sProcesses[j].nBurst)
            {
                if (sProcesses[i].nArrival > sProcesses[j].nArrival)
                {
                    sTemp = sProcesses[i];
                    sProcesses[i] = sProcesses[j];
                    sProcesses[j] = sTemp;
                }
            }
        }
    }
}

void sortArrival(struct Process sProcesses[], int nY)
{
    int i, j;
    struct Process sTemp;
    for (i = 0; i < nY - 1; i++)
    {
        for (j = i + 1; j < nY; j++)
        {
            if (sProcesses[i].nArrival > sProcesses[j].nArrival)
            {
                sTemp = sProcesses[i];
                sProcesses[i] = sProcesses[j];
                sProcesses[j] = sTemp;
            }
        }
    }
}

void sortEndTime(struct Output sOutputs[], int nY)
{
    int i, j;
    struct Output sTemp;
    for (i = 0; i < nY - 1; i++)
    {
        for (j = i + 1; j < nY; j++)
        {
            if (sOutputs[i].pTail->nEnd > sOutputs[j].pTail->nEnd)
            {
                sTemp = sOutputs[i];
                sOutputs[i] = sOutputs[j];
                sOutputs[j] = sTemp;
            }
        }
    }
}

// For Testing
void printProcesses(struct Process sProcesses[], int nY)
{
    int i;
    for (i = 0; i < nY; i++)
    {
        printf("Process ID: %d, Arrival Time: %d, Burst Time: %d\n", sProcesses[i].nPID, sProcesses[i].nArrival, sProcesses[i].nBurst, sProcesses[i].nRemain);
    }
}

void printOutput(struct Output sOutputs[], int nY)
{
    int i;

    sortEndTime(sOutputs, nY);

    for (i = 0; i < nY; i++)
    {
        printf("P[%d] ", sOutputs[i].nPID);
        struct Node *pCurrent = sOutputs[i].pHead;
        while (pCurrent != NULL)
        {
            printf("Start time %d End time: %d | ", pCurrent->nStart, pCurrent->nEnd);
            pCurrent = pCurrent->pNext;
        }
        printf("Waiting time: %d\n", sOutputs[i].nWait);
    }
}

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
        if (i == 0)
        {
            nStart = sProcesses[i].nArrival;
        }
        else
        {
            nStart = sOutputs[i - 1].pTail->nEnd > sProcesses[i].nArrival ? sOutputs[i - 1].pTail->nEnd : sProcesses[i].nArrival; // max (sOutputs[i - 1].pTail->nEnd, sProcesses[i].nArrival)
        }
        nEnd = nStart + sProcesses[i].nBurst;
        sOutputs[i].nWait = nStart - sProcesses[i].nArrival;

        pushNode(&sOutputs[i], nStart, nEnd);

        nTotalWait += sOutputs[i].nWait;
    }

    printOutput(sOutputs, nY);
    printf("Average waiting time: %.2lf\n", 1.0 * nTotalWait / nY);
}

void SJF(struct Process sProcesses[], struct Output sOutputs[], int nY)
{
    int i;
    int nTotalWait = 0;
    int nStart, nEnd;

    sortBurst(sProcesses, nY);

    initializeOutput(sOutputs, nY);

    for (i = 0; i < nY; i++)
    {
        sOutputs[i].nPID = sProcesses[i].nPID;
        if (i == 0)
        {
            nStart = sProcesses[i].nArrival;
        }
        else
        {
            nStart = sOutputs[i - 1].pTail->nEnd > sProcesses[i].nArrival ? sOutputs[i - 1].pTail->nEnd : sProcesses[i].nArrival; // max (sOutputs[i - 1].pTail->nEnd, sProcesses[i].nArrival)
        }
        nEnd = nStart + sProcesses[i].nBurst;
        sOutputs[i].nWait = nStart - sProcesses[i].nArrival;

        pushNode(&sOutputs[i], nStart, nEnd);

        nTotalWait += sOutputs[i].nWait;
    }

    printOutput(sOutputs, nY);
    printf("Average waiting time: %.2lf\n", 1.0 * nTotalWait / nY);
}

int isFinished(struct Process sProcesses[], int nY)
{
    int i;
    for (i = 0; i < nY; i++)
    {
        if (sProcesses[i].nRemain != 0)
            return 0; // 0 - not yet finished
    }

    return 1; // 1 - is finished
}

int getMinRemain(struct Process sProcesses[], int nY, int nCurrent)
{
    int i;
    int minVal = MAX_INT;
    int minIdx = -1;

    for (i = 0; i < nY; i++)
    {
        if (sProcesses[i].nArrival <= nCurrent && sProcesses[i].nRemain > 0 && sProcesses[i].nRemain < minVal)
        {
            minIdx = i;
            minVal = sProcesses[i].nRemain;
        }
    }

    // printf("%d %d", minVal, minIdx);

    if (isFinished(sProcesses, nY))
    {
        return -69; //-69 means all processes have 0 remaining time
    }
    else
    {
        // remaining times are zero
        return minIdx; //-1 means no process can be executed at the current time
    }
}

void initializeRemain(struct Process sProcesses[], int nY)
{
    int i;
    for (i = 0; i < nY; i++)
    {
        sProcesses[i].nRemain = sProcesses[i].nBurst;
    }
}

void SRTF(struct Process sProcesses[], struct Output sOutputs[], int nY)
{
    int nCurrIdx;
    int nPrevIdx = -1;
    int nCurrTime = 0;
    int nTotalWait = 0;
    int nBurstTime = 0;
    int nStart, nEnd;

    sortArrival(sProcesses, nY);

    initializeOutput(sOutputs, nY);
    initializeRemain(sProcesses, nY);

    // https://www.geeksforgeeks.org/program-for-shortest-job-first-sjf-scheduling-set-2-preemptive/?ref=rp

    // if same

    // remaining processes

    // check if same (nprev==idx)
    // tick++

    // check for != -1
    //  calculate the burst time here

    // Process 0 Arrival 0 Remaining 6
    // Process 1 Arrival 10 Remaining 2

    // 0 (P0) 6 (None) 10 (P1) 12

    // 0
    // nPrevIdx = -1 && nCurrIdx = 0

    // 1-6
    // nPrevIdx = nCurrIdx = 0

    // 7
    // nPrevIdx = 0 && nCurrIdx = -1

    // 8 to 9
    // nPrevIdx = nCurrIdx = -1

    // 10
    // nPrevIdx = -1 && nCurrIdx = 1

    // 11 to 12
    // nPrevIdx = nCurrIdx = 1

    // Process 0 Arrival 0 Remaining 6
    // Process 1 Arrival 1 Remaining 4
    //  0 (P0) 1 (P1) 5 (P0) 11

    // 0
    // nPrevIdx = -1 && nCurrIdx = 0

    // 1
    // nPrevIdx = nCurrIdx = 0

    // 2
    // nPrevIdx = 0 && nCurrIdx = 1

    // 3 to 5
    // nPrevIdx = nCurrIdx = 1

    // 6
    // nPrevIdx = 1 && nCurrIdx = 0

    // 7 to 10
    // nPrevIdx = nCurrIdx = 1

    while ((nCurrIdx = getMinRemain(sProcesses, nY, nCurrTime)) != -69)
    {
        printf("%d %d %d\n", nCurrTime, nPrevIdx, nCurrIdx);
        // p  revIdx = 0, currIdx=-1
        // if it is -1, it may still need to be saved
        // if (nCurrIdx != -1)
        // {
        //  printf("%d %d", nPrevIdx, nCurrIdx);

        // nCurrIdx and nPrevIdx are different processes
        // if nPrevIdx is -1, then the previous process need not be saved
        if (nCurrIdx != nPrevIdx && nPrevIdx != -1) // not same
        {
            sOutputs[nPrevIdx].nPID = sProcesses[nPrevIdx].nPID;
            // save the burst of the previous process to the output
            nStart = sOutputs[nPrevIdx].pHead == NULL && sOutputs[nPrevIdx].pTail == NULL ? sProcesses[nPrevIdx].nArrival : sOutputs[nPrevIdx].pTail->nEnd;
            nEnd = nStart + nBurstTime;
            sOutputs[nPrevIdx].nWait += nStart - sProcesses[nPrevIdx].nArrival;

            pushNode(&sOutputs[nPrevIdx], nStart, nEnd);

            nTotalWait += sOutputs[nPrevIdx].nWait;

            if (nCurrIdx != -1) // CPU is currently idle, no process can be executed
            {
                nBurstTime = 1;
                sProcesses[nCurrIdx].nRemain--;
            }
        }
        else
        {
            nBurstTime++;
            sProcesses[nPrevIdx].nRemain--;
        }
        // }
        nCurrTime++;
        nPrevIdx = nCurrIdx;

        if (nCurrTime == 100)
            break;

        // Question:
        //  nRemain--, nPrevIdx
    }

    sOutputs[nCurrIdx].nPID = sProcesses[nCurrIdx].nPID;
    nStart = sOutputs[nCurrIdx].pHead == NULL && sOutputs[nCurrIdx].pTail == NULL ? sProcesses[nCurrIdx].nArrival : sOutputs[nCurrIdx].pTail->nEnd;
    nEnd = nStart + nBurstTime;
    sOutputs[nCurrIdx].nWait += nStart - sProcesses[nCurrIdx].nArrival;

    pushNode(&sOutputs[nCurrIdx], nStart, nEnd);

    nTotalWait += sOutputs[nCurrIdx].nWait;

    // printOutput(sOutputs, nY);
    // printf("galit na print");
    printf("Average waiting time: %.2lf\n", 1.0 * nTotalWait / nY);
}

void RR(struct Process sProcesses[], struct Output sOutputs[], int nY, int nZ)
{
    int isStandBy = 1;
    int nTotalWait = 0;
    int nStart, nEnd;
    int nBurstTime;
    int nCurrTime = 0;

    sortArrival(sProcesses, nY);
    initializeOutput(sOutputs, nY);
    initializeRemain(sProcesses, nY);

    while (!isFinished(sProcesses, nY))
    {
        isStandBy = 1;

        for (int i = 0; i < nY; i++)
        {
            if (sProcesses[i].nRemain > 0 && sProcesses[i].nArrival <= nCurrTime)
            {
                sOutputs[i].nPID = sProcesses[i].nPID;

                nStart = nCurrTime;

                if (sProcesses[i].nRemain > nZ)
                {
                    nBurstTime = nZ;
                }
                else
                {
                    nBurstTime = sProcesses[i].nRemain;
                }

                nEnd = nStart + nBurstTime;
                sProcesses[i].nRemain -= nBurstTime;
                nCurrTime += nBurstTime;

                pushNode(&sOutputs[i], nStart, nEnd);

                if (sProcesses[i].nRemain == 0)
                {
                    sOutputs[i].nWait = sOutputs[i].pTail->nEnd - (sProcesses[i].nArrival + sProcesses[i].nBurst);
                    nTotalWait += sOutputs[i].nWait;
                }

                isStandBy = 0;
            }
        }
        if (isStandBy)
        {
            nCurrTime++;
        }
    }

    // Input
    // Process ID: 0, Arrival Time: 0, Burst Time: 6
    // Process ID: 1, Arrival Time: 2, Burst Time: 2

    // Current Output
    // P[0] Start time 0 End time: 2 | Start time 4 End time: 6 | Start time 6 End time: 8 | Waiting time: 2
    // P[1] Start time 2 End time: 4 | Waiting time: 0

    // P0 P1
    // P0
    // P0

    // Question:
    // P[0] Start time 0 End time: 2 | Start time 2 End time: 4 | Start time 6 End time: 8 | Waiting time: 2
    // P[1] Start time 4 End time: 6 | Waiting time: 0

    // P0
    // P0 P1
    // P0
    printOutput(sOutputs, nY);
    printf("Average waiting time: %.2lf\n", 1.0 * nTotalWait / nY);
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

    if ((pInput = fopen(sFilename, "r")) != NULL) // the file exists
    {

        fscanf(pInput, "%d %d %d", &nX, &nY, &nZ);

        if (nX != 3)
            nZ = 1;

        for (i = 0; i < nY; i++)
        {
            fscanf(pInput, "%d %d %d", &sProcesses[i].nPID, &sProcesses[i].nArrival, &sProcesses[i].nBurst);
        }

        printProcesses(sProcesses, nY);

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
            printf("Invalid CPU Schedule Algorithm");
        }

        fclose(pInput);
    }
    else
    {
        printf("%s not found.", sFilename);
    }

    return 0;
}