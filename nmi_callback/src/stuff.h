#pragma once

#include <intrin.h>
#include <ntddk.h>

#define NMI_CB_POOL_TAG 'BCmN'

//0xa8 bytes (sizeof)
typedef struct _KAFFINITY_EX
{
    USHORT Count;                                                           //0x0
    USHORT Size;                                                            //0x2
    ULONG Reserved;                                                         //0x4
    ULONGLONG Bitmap[20];                                                   //0x8
} KAFFINITY_EX, *PKAFFINITY_EX;

typedef struct _NMI_CONTEXT
{
    INT NumFired;
} NMI_CONTEXT, *PNMI_CONTEXT;

EXTERN_C INT64 HLP_SecInNs(INT64 nSec);
EXTERN_C VOID HLP_DelayExecutionThread(INT64 nSeconds);
EXTERN_C BOOLEAN HLP_FireNMI(INT core, PKAFFINITY_EX affinity);

EXTERN_C VOID KeInitializeAffinityEx(PKAFFINITY_EX affinity);
EXTERN_C VOID KeAddProcessorAffinityEx(PKAFFINITY_EX affinity, INT num);

EXTERN_C VOID HalSendNMI(PKAFFINITY_EX affinity);
