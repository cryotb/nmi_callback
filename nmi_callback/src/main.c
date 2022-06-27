#include "stuff.h"

PVOID g_NmiCallbackHandle;
PKAFFINITY_EX g_NmiAffinity;
PNMI_CONTEXT g_NmiContext;

BOOLEAN NmiCallback(PVOID context, BOOLEAN handled)
{
	PNMI_CONTEXT cpuContext = &((PNMI_CONTEXT)context)[KeGetCurrentProcessorNumberEx(NULL)];

	++cpuContext->NumFired;

	return TRUE;
}

VOID DriverUnload(PDRIVER_OBJECT drvObj)
{
	DbgPrint("[nmi_cb]  unload\n");
	
	if (g_NmiCallbackHandle) KeDeregisterNmiCallback(g_NmiCallbackHandle);
	if (g_NmiAffinity) ExFreePoolWithTag(g_NmiAffinity, NMI_CB_POOL_TAG);
	if (g_NmiContext) ExFreePoolWithTag(g_NmiContext, NMI_CB_POOL_TAG);
}

NTSTATUS DriverEntry(PDRIVER_OBJECT drvObj, PUNICODE_STRING regPath)
{
	DbgPrint("\n[nmi_cb]  entry\n");

	ULONG numCores = KeQueryActiveProcessorCountEx(NULL);

	drvObj->DriverUnload = DriverUnload;

	ULONG nmiContextLength = numCores * sizeof(NMI_CONTEXT);

	g_NmiAffinity = ExAllocatePoolWithTag(NonPagedPool, sizeof(KAFFINITY_EX), NMI_CB_POOL_TAG);
	g_NmiContext = (PNMI_CONTEXT)ExAllocatePoolWithTag(NonPagedPool, nmiContextLength, NMI_CB_POOL_TAG);
	g_NmiCallbackHandle = KeRegisterNmiCallback(NmiCallback, g_NmiContext);

	if (!g_NmiAffinity || !g_NmiContext || !g_NmiCallbackHandle)
		return STATUS_FAILED_DRIVER_ENTRY;

	memset(g_NmiContext, 0, nmiContextLength);

	HLP_FireNMI(0, g_NmiAffinity); /* this will fire 2 NMIs on CPU #0 */
	HLP_DelayExecutionThread(2); /* can adjust this delay, it's been arbitrarily chosen. */

	for (ULONG i = 0; i < numCores; i++)
	{
		PNMI_CONTEXT cpuContext = &g_NmiContext[i];

		DbgPrint("[nmi_cb]  CPU#%i NmiCallbackInvokes=%i\n", i, cpuContext->NumFired);
	}

	return STATUS_SUCCESS;
}
