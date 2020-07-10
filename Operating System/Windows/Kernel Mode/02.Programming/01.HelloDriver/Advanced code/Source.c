#include <ntddk.h>

VOID DriverUnload(_In_ PDRIVER_OBJECT DriverObject)
{
	UNREFERENCED_PARAMETER(DriverObject);
}


NTSTATUS DriverEntry(_In_ PDRIVER_OBJECT DriverObject,_In_ PUNICODE_STRING RegistryPath)
{
	UNREFERENCED_PARAMETER(DriverObject);
	UNREFERENCED_PARAMETER(RegistryPath);


	DbgPrintEx(DPFLTR_IHVDRIVER_ID, 0, "Hello Driver");

	DriverObject->DriverUnload = DriverUnload;


	return STATUS_SUCCESS;
}
