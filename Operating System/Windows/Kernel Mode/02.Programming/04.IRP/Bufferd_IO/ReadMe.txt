How to use?

1st. set the device object

DeviceObject->Flags |= DO_BUFFERD_IO;



BUFFERD_I/O is I/O Manager allocate the temporary buffer same as user buffer size

You can find the buffer size intthe current I/O stack location's Parameters.Read.Length or Parameters.Write.Length
