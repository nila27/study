#include <ntddk.h>

VOID CreateProcessNotifyRoutineEx(PEPROCESS Process, HANDLE ProcessId, PPS_CREATE_NOTIFY_INFO CreateInfo)
{
    UNREFERENCED_PARAMETER(Process);
    UNREFERENCED_PARAMETER(ProcessId);

    PEPROCESS PP = Process;

    if (CreateInfo == NULL)
    {
        DbgPrintEx(DPFLTR_IHVDRIVER_ID, 0, "Process Exit");
    }
    else
    {
        
        DbgPrintEx(DPFLTR_IHVDRIVER_ID, 0, "Processs Name   :   (%s)\n", (const char*)(unsigned char*)PP + 0x450);

    }
}

VOID DriverUnload(_In_ PDRIVER_OBJECT driverobject)
{
	UNREFERENCED_PARAMETER(driverobject);
    PsSetCreateProcessNotifyRoutineEx(CreateProcessNotifyRoutineEx, TRUE);

    DbgPrintEx(DPFLTR_IHVAUDIO_ID, 0, "Stop");
}

NTSTATUS DriverEntry(_In_ PDRIVER_OBJECT driverobject, _In_ PUNICODE_STRING RegistryPath)
{
	UNREFERENCED_PARAMETER(RegistryPath);

    driverobject->DriverUnload = DriverUnload;

    DbgPrintEx(DPFLTR_IHVAUDIO_ID, 0, "Start");


    PsSetCreateProcessNotifyRoutineEx(CreateProcessNotifyRoutineEx, FALSE);

    return STATUS_SUCCESS;
}
