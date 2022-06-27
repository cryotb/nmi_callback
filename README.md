# nmi_callback
This driver once loaded, will fire an NMI on your first cpu core and attempt to handle it using NMI callback.
If the NMI was triggered successfully and catched by the callback, you will see following result in WinDbg:

![alt text](https://github.com/cryotb/nmi_callback/blob/master/demo.PNG)
