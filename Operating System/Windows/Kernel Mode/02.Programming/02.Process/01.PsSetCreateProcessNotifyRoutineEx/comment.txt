When you execute the code, you will see the filename with directory path.

like \??\c:\windows\system32\svchost.exe

that is createinfo->ImageFileName
createinfo->commandLine is c:\windows\system32\svchost.exe



Let's explain the code

PsSetCreateProcessNotifyRoutineEx is callback function that wether process execute or not

Let's see define in ntddk.h

PsSetCreateProcessNotifyRoutineEx (
    _In_ PCREATE_PROCESS_NOTIFY_ROUTINE_EX NotifyRoutine,
    _In_ BOOLEAN Remove
    );

You can see _In_ SAL. Last code in Hellodriver, you learned the SAL remeber it

So, see driverentry function

NTSTATUS DriverEntry(_In_ PDRIVER_OBJECT driverobject, _In_ PUNICODE_STRING RegistryPath)
{
	UNREFERENCED_PARAMETER(RegistryPath);

    driverobject->DriverUnload = DriverUnload;

    DbgPrintEx(DPFLTR_IHVAUDIO_ID, 0, "Start");


    PsSetCreateProcessNotifyRoutineEx(CreateProcessNotifyRoutineEx, FALSE);

    return STATUS_SUCCESS;
}

before we declared notifyroutine function, CreateProcessNotifyRoutineEx and load the function so FALSE flag

watch out. we used the PsSetCreateProcessNotifyRoutineEx callback function, so must remove that


Let's see DriverUnload

VOID DriverUnload(_In_ PDRIVER_OBJECT driverobject)
{
    UNREFERENCED_PARAMETER(driverobject);
    PsSetCreateProcessNotifyRoutineEx(CreateProcessNotifyRoutineEx, TRUE);

    DbgPrintEx(DPFLTR_IHVAUDIO_ID, 0, "Stop");
}

You can find TRUE flag. now you know that


Now we see the callback function.

VOID CreateProcessNotifyRoutineEx(PEPROCESS Process, HANDLE ProcessId, PPS_CREATE_NOTIFY_INFO CreateInfo)
{
    UNREFERENCED_PARAMETER(Process);
    UNREFERENCED_PARAMETER(ProcessId);

    if (CreateInfo == NULL)
    {
        DbgPrintEx(DPFLTR_IHVDRIVER_ID, 0, "Process Exit");
    }
    else
    {
        
        DbgPrintEx(DPFLTR_IHVDRIVER_ID, 0, "Processs Name   :   (%wZ)\n", CreateInfo->ImageFileName);
        DbgPrintEx(DPFLTR_IHVDRIVER_ID, 0, "Processs Name   :   (%wZ)\n", CreateInfo->CommandLine);
	    
    }
}

In callback function, we don't know the PPS_CREATE_NOTIFY_INFO information.

Let's see the define that in ntddk.h


typedef struct _PS_CREATE_NOTIFY_INFO {
    _In_ SIZE_T Size;
    union {
        _In_ ULONG Flags;
        struct {
            _In_ ULONG FileOpenNameAvailable : 1;
            _In_ ULONG IsSubsystemProcess : 1;
            _In_ ULONG Reserved : 30;
        };
    };
    _In_ HANDLE ParentProcessId;
    _In_ CLIENT_ID CreatingThreadId;
    _Inout_ struct _FILE_OBJECT *FileObject;
    _In_ PCUNICODE_STRING ImageFileName;
    _In_opt_ PCUNICODE_STRING CommandLine;
    _Inout_ NTSTATUS CreationStatus;
} PS_CREATE_NOTIFY_INFO, *PPS_CREATE_NOTIFY_INFO;

there is a flags, and fields

IsSubsystemProcesss - this flag is set if this process is a Pico process. this can only be if the driver registered with PsSetCreateProcessNotifyRoutineEx2


ParentProcessId : the parent processs ID(not a handle). this process may be the same provided by CreateThreadId. 
UniqueProcesss, but may be different, as it's possible as part of process creation to pass in a different parent inherit some properties from

CreatingThreadId : a combination of thread and process Id of the caller to the processs creation function

ImageFileName : the image file name of the executable, available if the flag FileOpenNameAvailable is set

CommandLine : the full command line used to create the process. Note that it may be NULL. 

CreateStatus : this is the status that would return to the caller. this is where the driver can stop the process from being created by placing some failure status


