#include "stuff.h"

INT64 HLP_SecInNs(INT64 nSec)
{
	return (nSec * 10000000);
}

VOID HLP_DelayExecutionThread(INT64 nSeconds)
{
	LARGE_INTEGER nDelay;
	memset(&nDelay, 0, sizeof(nDelay));

	nDelay.QuadPart -= HLP_SecInNs(nSeconds);

	KeDelayExecutionThread(KernelMode, FALSE, &nDelay);
}

BOOLEAN HLP_FireNMI(INT core, PKAFFINITY_EX affinity)
{
	KeInitializeAffinityEx(affinity);
	KeAddProcessorAffinityEx(affinity, core);

	HalSendNMI(affinity);

	return TRUE;
}
