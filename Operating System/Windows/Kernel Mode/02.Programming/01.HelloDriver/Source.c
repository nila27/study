#include <ntddk.h>

VOID DriverUnload(PDRIVER_OBJECT DriverObject)
{
	UNREFERENCED_PARAMETER(DriverObject);
}


NTSTATUS DriverEntry(PDRIVER_OBJECT DriverObject, PUNICODE_STRING RegistryPath)
{
	UNREFERENCED_PARAMETER(DriverObject);
	UNREFERENCED_PARAMETER(RegistryPath);


	DbgPrintEx(DPFLTR_IHVDRIVER_ID, 0, "Hello Driver");

	DriverObject->DriverUnload = DriverUnload;


	return STATUS_SUCCESS;
}
