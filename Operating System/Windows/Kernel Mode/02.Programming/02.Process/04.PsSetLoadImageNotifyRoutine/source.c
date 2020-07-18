#include <ntddk.h>

void LoadImageRoutine(PUNICODE_STRING FullImageName, HANDLE ProcessId, PIMAGE_INFO ImageInfo)
{

	UNREFERENCED_PARAMETER(ImageInfo);
	DbgPrintEx(DPFLTR_IHVDRIVER_ID, 0, "(ID:0x%X)  :	(%wZ)\n", ProcessId, FullImageName);
	

}

VOID DriverUnload(_In_ PDRIVER_OBJECT driverobject)
{
    	UNREFERENCED_PARAMETER(driverobject);
    	PsRemoveLoadImageNotifyRoutine(LoadImageRoutine);
  	  DbgPrintEx(DPFLTR_IHVAUDIO_ID, 0, "Stop");
}	


NTSTATUS DriverEntry(_In_ PDRIVER_OBJECT driverobject, _In_ PUNICODE_STRING RegistryPath)
{
	UNREFERENCED_PARAMETER(DriverObject);
	UNREFERENCED_PARAMETER(RegistryPath);

 	driverobject->DriverUnload = DriverUnload;

  	DbgPrintEx(DPFLTR_IHVAUDIO_ID, 0, "Start");

  	PsSetLoadImageNotifyRoutine(LoadImageRoutine);

  	return STATUS_SUCCESS;
}
