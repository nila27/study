#include <ntddk.h>

VOID DriverUnload(_In_ PDRIVER_OBJECT DriverObject)
{
	_Unreferenced_parameter_(DriverObject);
}


extern "C" NTSTATUS DriverEntry(_In_ PDRIVER_OBJECT DriverObject,_In_ PUNICODE_STRING RegistryPath)
{
	UNREFERENCED_PARAMETER(DriverObject);
	UNREFERENCED_PARAMETER(RegistryPath);


	DbgPrintEx(DPFLTR_IHVDRIVER_ID, 0, "Hello Driver");


	return STATUS_SUCCESS;
}
