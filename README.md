# nmi_callback
This driver will fire a non-maskable interrupt (NMI) on your 1st cpu core and attempt to handle it using an NMI callback.
If it succeeds, you will see following result in WinDbg:

![alt text](https://github.com/cryotb/nmi_callback/blob/master/demo.PNG)

In order to get this done, i am doing the following:
  1. Allocate KAFFINITY_EX structure.
  2. Pass it to KeInitializeAffinityEx.
  3. Add the CPU core the NMI should be run at to the affinity, by calling KeAddProcessorAffinityEx.
  4. Finally calling HalSendNMI to send it. i am passing the affinity as parameter.

Note that all of these APIs listed above are not publicly documented so there might be inconsistencies on return value, prototype etc...
