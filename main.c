#define MAX_STRING 25
#define MAX_PROCESS 100

#include <stdio.h>

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

    for (int i = 0; i < nY; i++)
    {
        printf("Process ID: %d, Arrival Time: %d, Burst Time: %d", sProcesses[i].nPID, sProcesses[i].nArrival, sProcesses[i].nBurst, sProcesses[i].nRemain);
    }
}

void printOutput(struct Output sOutputs[], int nY)
{
    for (int i = 0; i < nY; i++)
    {
        printf("P[%d] ", sOutputs[i].nPID);
        struct Node *pCurrent = sOutputs[i].pHead;
        while (pCurrent != NULL)
        {
            printf("Start Time %d End Time: %d | ", pCurrent->nStart, pCurrent->nEnd);
            pCurrent = pCurrent->pNext;
        }
        printf("Wait Time: %d", sOutputs[i].nWait);
    }
}

// sort by burst time
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
    double dTotal = 0;

    sortArrival(sProcesses, nY);

    for (i = 0; i < nY; i++)
    {
        if (sOutputs[i].pHead = malloc(sizeof(struct Node)) == NULL)
        {
            printf("ERROR : not enough memory\n");
        }
        else
        {
            sOutputs[i].pTail = sOutputs[i].pHead;
            sOutputs[i].nPID = sProcesses[i].nPID;
            if (i == 0)
            {
                sOutputs[i].pHead->nStart = sProcesses[i].nArrival;
            }
            else
            {
                sOutputs[i].pHead->nStart = sOutputs[i - 1].pHead->nEnd > sProcesses[i].nArrival ? sOutputs[i - 1].pHead->nEnd : sProcesses[i].nArrival; // max (sOutputs[i - 1].pHead->nEnd, sProcesses[i].nArrival)
            }
            sOutputs[i].nWait = sOutputs[i].pHead->nStart - sProcesses[i].nArrival;
            sOutputs[i].pHead->nEnd = sOutputs[i].pHead->nStart + sProcesses[i].nBurst;

            dTotal += sOutputs[i].nWait;

            sOutputs[i].pHead->pNext = NULL;
        }
    }

    return dTotal / nY;
}

int SJF(struct Process sProcesses[], struct Output sOutputs[], int nY)
{
    int i;
    double dTotal = 0;

    sortBurst(sProcesses, nY);

    // remaining processes
    for (i = 0; i < nY; i++)
    {
        if (sOutputs[i].pHead = malloc(sizeof(struct Node)) == NULL)
        {
            printf("ERROR : not enough memory\n");
        }
        else
        {
            sOutputs[i].pTail = sOutputs[i].pHead;
            sOutputs[i].nPID = sProcesses[i].nPID;
            if (i == 0)
            {
                sOutputs[i].pHead->nStart = sProcesses[i].nArrival;
            }
            else
            {
                sOutputs[i].pHead->nStart = sOutputs[i - 1].pHead->nEnd > sProcesses[i].nArrival ? sOutputs[i - 1].pHead->nEnd : sProcesses[i].nArrival; // max (sOutputs[i - 1].pHead->nEnd, sProcesses[i].nArrival)
            }
            sOutputs[i].nWait = sOutputs[i].pHead->nStart - sProcesses[i].nArrival;
            sOutputs[i].pHead->nEnd = sOutputs[i].pHead->nStart + sProcesses[i].nBurst;

            dTotal += sOutputs[i].nWait;

            sOutputs[i].pHead->pNext = NULL;
        }
    }

    return dTotal / nY;
}

// RETURNS THE INDEX???
// CAN INSERT A TIME TICK -> ub
int getMinRemaining(struct Process sProcesses[], int nY, int nCurrent)
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

void copyBurstToRemain(struct Process sProcesses[], int nY)
{
    int i;
    for (i = 0; i < nY; i++)
    {
        sProcesses[i].nRemain = sProcesses[i].nBurst;
    }
}

int SRTF(struct Process sProcesses[], struct Output sOutputs[], int nY)
{
    int nCurrIdx;
    int nPrevIdx;
    int nTime = 0;
    double dTotal = 0;

    sortArrival(sProcesses, nY);
    copyBurstToRemain(sProcesses, nY);

    // remaining processes
    while ((nCurrIdx = getMinRemaining(sProcesses, nY, nTime)) != -1)
    {

        if (sOutputs[nCurrIdx].pHead = malloc(sizeof(struct Node)) == NULL)
        {
            printf("ERROR : not enough memory\n");
        }
        else
        {
            sOutputs[nCurrIdx].pTail = sOutputs[nCurrIdx].pHead;
            sOutputs[nCurrIdx].nPID = sProcesses[nCurrIdx].nPID;

            if (nCurrIdx == 0)
            {
                sOutputs[nCurrIdx].pHead->nStart = sProcesses[nCurrIdx].nArrival;
            }
            else
            {
                sOutputs[nCurrIdx].pHead->nStart = sOutputs[nPrevIdx].pHead->nEnd > sProcesses[nCurrIdx].nArrival ? sOutputs[nPrevIdx].pHead->nEnd : sProcesses[nCurrIdx].nArrival; // max (sOutputs[nPrevIdx].pHead->nEnd, sProcesses[nCurrIdx].nArrival)
            }
            sOutputs[nCurrIdx].nWait = sOutputs[nCurrIdx].pHead->nStart - sProcesses[nCurrIdx].nArrival;
            sOutputs[nCurrIdx].pHead->nEnd = sOutputs[nCurrIdx].pHead->nStart + sProcesses[nCurrIdx].nBurst;

            dTotal += sOutputs[nCurrIdx].nWait;

            sOutputs[nCurrIdx].pHead->pNext = NULL;
        }

        nTime++;
        sProcesses[nCurrIdx].nRemain--;
        nPrevIdx = nCurrIdx;
    }
}

int RR(struct Process sProcesses[], struct Output sOutputs[], int nY, int nZ)
{
    int isFinished = 1;
    double dTotal = 0;
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
                // dTotal += sOutputs[i].nWait;

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

    if ((pInput = fopen(sFilename, "r")) != NULL) // the file exists
    {

        fscanf(pInput, "%d %d %d", nX, nY, nZ);

        if (nX != 3)
            nZ = 1;

        for (i = 0; i < nY; i++)
        {
            fscanf(pInput, "%d %d %d", sProcesses[i].nPID, sProcesses[i].nArrival, sProcesses[i].nBurst);
        }

        switch (nX)
        {
        case 0:
            FCFS(sProcesses, sOutputs, nY);
            break;
        case 1:
            SJF(sProcesses, nY);
            break;
        case 2:
            SRTF(sProcesses, nY);
            break;
        case 3:
            RR(sProcesses, nY, nZ);
            break;
        }

        fclose(pInput);
    }
    else
    {
        printf("%s not found.", sFilename);
    }

    return 0;
}