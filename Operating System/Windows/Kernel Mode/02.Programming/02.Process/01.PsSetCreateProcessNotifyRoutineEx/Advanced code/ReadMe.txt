When execute basic code, you can see directory path. but when execute advanced code, just print imagefilename.

Let's see the code

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

ofcourse, you can use paramter Process without using PP

like

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
        
        DbgPrintEx(DPFLTR_IHVDRIVER_ID, 0, "Processs Name   :   (%s)\n", (const char*)(unsigned char*)Process + 0x450);

    }
}

It doesn't matter



I used 0x450. what does mean?

Windows expresses process as eprocess structure

Let's see eprcoess structure

   +0x000 Pcb              : _KPROCESS
                   .
                   .
                   .             
   +0x450 ImageFileName    : [15] UChar
                   .
                   .
                   .    
   +0x878 MmHotPatchContext : Ptr64 Void      
   
   
 You can get not only ImageFileName but also peb address, activeprocesslinks, etc..
 
