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

size - The size, in bytes, of this structure. The operating system uses this size to indicate the type of structure that it passes to CreateProcessNotifyEx. Currently, this member is always sizeof(PS_CREATE_NOTIFY_INFO).

Flags - Use the FileOpenNameAvailable member instead.

FileOpenNameAvailable - specifies wether the ImageFileName member contains the exact file name that is used to topen the process executable file

IsSubsystemProcesss - this flag is set if this process is a Pico process. this can only be if the driver registered with PsSetCreateProcessNotifyRoutineEx2
IsSubsystemProcess is only populated for subsystem processes other than Win32 when a driver has registered through PsSetCreateProcessNotifyRoutineEx2 with a type that allows for notifications from subsystem processes. 
When IsSubsystemProcess is set, the FileObject, ImageFileName, and CommandLine may be NULL. Drivers should use ProcessSubsystemInformation to query the subsystem type if needed. For more information, see NtQueryInformationProcess. 

ParentProcessId : The process ID of the parent process for the new process. Note that the parent process is not necessarily the same process as the process that created the new process. 
The new process can inherit certain properties of the parent process, such as handles or shared memory. (The process ID of the process creator is given by CreatingThreadId->UniqueProcess.)

CreatingThreadId : a combination of thread and process Id of the caller to the processs creation function. 
CreatingThreadId->UniqueProcess contains the process ID
CreatingThreadId->UniqueThread contains the thread ID.

ImageFileName : the image file name of the executable, available if the flag FileOpenNameAvailable is set
if IsSubsystemProcess is TRUE, this value may be NULL.

CommandLine : A pointer to a UNICODE_STRING string that holds the command that is used to execute the process. the full command line used to create the process. 
If the command is not available, CommandLine is NULL. 
If IsSubsystemProcess is TRUE, this value maybe NULL. 

CreateStatus : The NTSTATUS value to return for the process-creation operation. Drivers can change this value to an error code to prevent the process from being created.


