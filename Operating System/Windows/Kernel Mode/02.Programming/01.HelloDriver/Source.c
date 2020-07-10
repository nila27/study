#include <ntddk.h>https://github.com/nila27/study/tree/master/Operating%20System/Windows/Kernel%20Mode/02.Programming/01.HelloDriver

VOID DriverUnload(PDRIVER_OBJECT DriverObject)
{
	_Unreferenced_parameter_(DriverObject);
}


NTSTATUS DriverEntry(PDRIVER_OBJECT DriverObject, PUNICODE_STRING RegistryPath)
{
	UNREFERENCED_PARAMETER(DriverObject);
	UNREFERENCED_PARAMETER(RegistryPath);


	DbgPrintEx(DPFLTR_IHVDRIVER_ID, 0, "Hello Driver");


	return STATUS_SUCCESS;
}
