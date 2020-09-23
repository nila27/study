How to use?

1st. set the device object

DeviceObject->Flags |= DO_DIRECT_IO;

DO_DIRECT_IO is using MDL(Memory Desicriptor List)

MDL is a data structure that knows how buffer is mapped to RAM

 The buffer set to MDL will be inhibited from paging, physical memory fiexed that corresponding to virtual memory
 
