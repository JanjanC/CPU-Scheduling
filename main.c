#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STRING 25
#define MAX_PROCESS 100

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

void initializeOutput(struct Output sOutputs[], int nY)
{
    int i;
    for (i = 0; i < nY; i++)
    {
        sOutputs[i].pHead = NULL;
        sOutputs[i].pTail = NULL;
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

int FCFS(struct Process sProcesses[], struct Output sOutputs[], int nY)
{
    int i;
    int nTotal = 0;
    int nStart, nEnd;

    sortArrival(sProcesses, nY);

    printProcesses(sProcesses, nY);
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
        sOutputs[i].nWait = nStart - sProcesses[i].nArrival;
        nEnd = nStart + sProcesses[i].nBurst;

        pushNode(&sOutputs[i], nStart, nEnd);

        nTotal += sOutputs[i].nWait;
    }

    printOutput(sOutputs, nY);
    printf("Average waiting time: %.2lf\n", 1.0 * nTotal / nY);

    return 1.0 * nTotal / nY;
}

int SJF(struct Process sProcesses[], struct Output sOutputs[], int nY)
{
    int i;
    int nTotal = 0;
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
        sOutputs[i].nWait = nStart - sProcesses[i].nArrival;
        nEnd = nStart + sProcesses[i].nBurst;

        pushNode(&sOutputs[i], nStart, nEnd);

        nTotal += sOutputs[i].nWait;
    }

    return 1.0 * nTotal / nY;
}

// RETURNS THE INDEX???
// CAN INSERT A TIME TICK -> ub
int getMinRemain(struct Process sProcesses[], int nY, int nCurrent)
{
    int i;
    int minIndex = 0;
    int isFinished = 1;
    // https://www.geeksforgeeks.org/program-for-shortest-job-first-sjf-scheduling-set-2-preemptive/?ref=rp

    for (i = 1; i < nY && sProcesses[i].nArrival <= nCurrent; i++)
    {
        if (sProcesses[minIndex].nRemain == 0 || sProcesses[i].nRemain < sProcesses[minIndex].nRemain)
            minIndex = i;
    }

    for (i = 0; i < nY; i++)
    {
        if (sProcesses[i].nRemain != 0)
            isFinished = 0;
    }

    if (isFinished)
    {
        return -69; //-69 means all processes have 0 remaining time
    }
    else
    {
        // remaining times are zero
        return sProcesses[minIndex].nRemain == 0 ? -1 : minIndex; //-1 means no process can be executed at the current given time
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

int SRTF(struct Process sProcesses[], struct Output sOutputs[], int nY)
{
    int nCurrIdx, nPrevIdx;
    int nTime = 0;
    int nTotal = 0;
    int nStart, nEnd;

    sortArrival(sProcesses, nY);

    initializeOutput(sOutputs, nY);
    initializeRemain(sProcesses, nY);

    // remaining processes
    while ((nCurrIdx = getMinRemain(sProcesses, nY, nTime)) != -1)
    {
        sOutputs[nCurrIdx].nPID = sProcesses[nCurrIdx].nPID;

        if (nCurrIdx == 0)
        {
            nStart = sProcesses[nCurrIdx].nArrival;
        }
        else
        {
            nStart = sOutputs[nPrevIdx].pTail->nEnd > sProcesses[nCurrIdx].nArrival ? sOutputs[nPrevIdx].pTail->nEnd : sProcesses[nCurrIdx].nArrival; // max (sOutputs[nPrevIdx].pTail->nEnd, sProcesses[nCurrIdx].nArrival)
        }
        sOutputs[nCurrIdx].nWait = sOutputs[nCurrIdx].pTail->nStart - sProcesses[nCurrIdx].nArrival;
        nEnd = sOutputs[nCurrIdx].pTail->nStart + sProcesses[nCurrIdx].nBurst;

        pushNode(&sOutputs[nCurrIdx], nStart, nEnd);

        nTotal += sOutputs[nCurrIdx].nWait;

        nTime++;
        sProcesses[nCurrIdx].nRemain--;
        nPrevIdx = nCurrIdx;
    }
}

int RR(struct Process sProcesses[], struct Output sOutputs[], int nY, int nZ)
{
    int isFinished = 1;
    int nTotal = 0;
    sortArrival(sProcesses, nY);

    while (!isFinished)
    {
        isFinished = 1;
        for (int i = 0; i < nY; i++)
        {
            if (sProcesses[i].nRemain > 0)
            {
                // -------------------- LINKED LIST WILL BE USED HERE ---------------------------
                // sOutputs[i].nPID = sProcesses[i].nPID;
                // sOutputs[i].nStart = sOutputs[i - 1].nEnd > sProcesses[i].nArrival ? sOutputs[i - 1].nEnd : sProcesses[i].nArrival; // max (sOutputs[i - 1].nEnd, sProcesses[i].nArrival)
                // sOutputs[i].nWait = sOutputs[i].nStart - sProcesses[i].nArrival;
                // sOutputs[i].nEnd = sOutputs[i].nStart + sProcesses[i].nBurst;
                // nTotal += sOutputs[i].nWait;

                isFinished = 0;
            }
        }
    }
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