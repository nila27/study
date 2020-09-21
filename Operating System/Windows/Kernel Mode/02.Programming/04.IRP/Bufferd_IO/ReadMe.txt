How to use?

1st. set the device object

DeviceObject->Flags |= DO_BUFFERD_IO;



BUFFERD_I/O is I/O Manager allocate the temporary buffer(System Buffer) from non-paged pool same as user buffer size

You can find the buffer size intthe current I/O stack location's Parameters.Read.Length or Parameters.Write.Length (IrpSp->Parameters.Read.Length)

Cause the buffer is in system space, driver can use the system buffer pointer directly without any check.



If Buffer's size is pretty larger, BUFFERD_I/O can waste virtual memory
