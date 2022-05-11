#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STRING 25
#define MAX_PROCESS 100
#define MAX_INT 2147483647

// Question:
// Good morning po ma'am, may I ask po if the input for the text file in MP1 should be "filename.txt" or simply "filename". Thank you very much po!

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

void sortID(struct Output sOutputs[], int nY)
{
    int i, j;
    struct Output sTemp;
    for (i = 0; i < nY - 1; i++)
    {
        for (j = i + 1; j < nY; j++)
        {
            if (sOutputs[i].nPID > sOutputs[j].nPID)
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

    sortID(sOutputs, nY);

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

    return isFinished(sProcesses, nY) ? -1 : minIdx; //-1 means all processes have 0 remaining time or no process can be executed at the current time
}

void initializeRemain(struct Process sProcesses[], int nY)
{
    int i;
    for (i = 0; i < nY; i++)
    {
        sProcesses[i].nRemain = sProcesses[i].nBurst;
    }
}

void SJF(struct Process sProcesses[], struct Output sOutputs[], int nY)
{
    int nCurrIdx;
    int nCurrTime = 0;
    int nTotalWait = 0;
    int nBurstTime;
    int nStart, nEnd;

    sortArrival(sProcesses, nY);

    initializeOutput(sOutputs, nY);
    initializeRemain(sProcesses, nY);

    while (!isFinished(sProcesses, nY))
    {
        nCurrIdx = getMinRemain(sProcesses, nY, nCurrTime);
        if (nCurrIdx >= 0)
        {
            sOutputs[nCurrIdx].nPID = sProcesses[nCurrIdx].nPID;
            nStart = nCurrTime;
            nEnd = nStart + sProcesses[nCurrIdx].nBurst;

            nCurrTime += sProcesses[nCurrIdx].nBurst;
            sProcesses[nCurrIdx].nRemain -= sProcesses[nCurrIdx].nBurst;

            pushNode(&sOutputs[nCurrIdx], nStart, nEnd);

            sOutputs[nCurrIdx].nWait = nStart - sProcesses[nCurrIdx].nArrival;
            nTotalWait += sOutputs[nCurrIdx].nWait;
        }
        else
        {
            nCurrTime++;
        }
    }

    printOutput(sOutputs, nY);
    printf("Average waiting time: %.2lf\n", 1.0 * nTotalWait / nY);
}

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

    while (!isFinished(sProcesses, nY))
    {
        nCurrIdx = getMinRemain(sProcesses, nY, nCurrTime);
        if (nCurrIdx >= 0)
        {
            sOutputs[nCurrIdx].nPID = sProcesses[nCurrIdx].nPID;
            nStart = nCurrTime;

            nBurstTime = 0;

            while (getMinRemain(sProcesses, nY, nCurrTime) == nCurrIdx)
            {
                nCurrTime++;
                nBurstTime++;
                sProcesses[nCurrIdx].nRemain--;
            }

            nEnd = nStart + nBurstTime;

            pushNode(&sOutputs[nCurrIdx], nStart, nEnd);

            if (sProcesses[nCurrIdx].nRemain == 0)
            {
                sOutputs[nCurrIdx].nWait = sOutputs[nCurrIdx].pTail->nEnd - (sProcesses[nCurrIdx].nArrival + sProcesses[nCurrIdx].nBurst);
                nTotalWait += sOutputs[nCurrIdx].nWait;
            }
        }
        else
        {
            nCurrTime++;
        }
    }

    printOutput(sOutputs, nY);
    printf("Average waiting time: %.2lf\n", 1.0 * nTotalWait / nY);
}

void RR(struct Process sProcesses[], struct Output sOutputs[], int nY, int nZ)
{
    int i;
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

        for (i = 0; i < nY; i++)
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