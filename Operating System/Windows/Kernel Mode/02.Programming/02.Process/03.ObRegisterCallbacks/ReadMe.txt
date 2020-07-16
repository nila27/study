NTSTATUS ObRegisterCallbacks(_In_ POB_CALLBACK_REGISTRATION CallbackRegistration, _Outptr_ PVOIOD *RegistrationHandle);

Prior to registration, an OB_CALLBACK_REGISTRATION structure must be initialized, which provides teh necessary details about what the driver is registering for.
The RegistrationHandle is the return value upon a successful registartion, which is just an opaque pointer used for unregistration by calling obUnregisterCallbacks.

Let's see OB_CALLBACK_REGISTRATION

this is define in wdm.h

typedef struct _OB_CALLBACK_REGISTRATION {
    _In_ USHORT                     Version;
    _In_ USHORT                     OperationRegistrationCount;
    _In_ UNICODE_STRING             Altitude;
    _In_ PVOID                      RegistrationContext;
    _In_ OB_OPERATION_REGISTRATION  *OperationRegistration;
} OB_CALLBACK_REGISTRATION, *POB_CALLBACK_REGISTRATION;

Version is a just a constant that must be set to OB_FLT_REGISTRATION_VERSION(0x100) also using ObGetFilterVersion() produce same effect.

OperationRegistrationCount determins the number of OB_OPERATION_REGISTRATION structures that are pointed to by OperationRegistration.

Altitude specifies a number that affects the order of callbacks invocations for this driver. (Altitude will study later in minifilter)

RegistrationContext value to the callback routine when the callback routine is run. The meaning of this value is driver-defined.

OB_OPERATION_REGISTRATION strctures that the driver sets up its callbacks, determines that object ypes and operation are of interest

this is define in wdm.h

typedef struct _OB_OPERATION_REGISTRATION {
    _In_ POBJECT_TYPE                *ObjectType;
    _In_ OB_OPERATION                Operations;
    _In_ POB_PRE_OPERATION_CALLBACK  PreOperation;
    _In_ POB_POST_OPERATION_CALLBACK PostOperation;
} OB_OPERATION_REGISTRATION, *POB_OPERATION_REGISTRATION;

ObjectType is a pointer to the objet type for this instance registration. process, thread or desktop.
this pointeris exported as global kernel variables. like PsPorcessType, PsThreawdType and ExDesktopObjectType(only windows 10)

Operation is an bit flags enumeration selecting create, open is OB_OPERATION_HANDLE_CREATE and duplicate is OB_OPERATION_HANDLE_DUPLICATE

OB_OPERATION_HANDLE_CREATE refers to calls to user modefunction such as CreateProcess, CreateThread, CreateDesktop, OpenProcess, OpenThread, OPenDesktop.. so on 
OB_OPERATION_HANDLE_DUPLICATE refers to handle duplication for DuplicateHandle user mode API

OB_PRE_OPERATION_CALLBACK is invoked before the actual create, open, duplicate operation complete.



now, we need to see pre, post operation callback function.

1st, let's see the pre operation callback function.

OB_PREOP_CALLBACK_STATUS PreCallback(PVOID RegistrationContext, POB_PRE_OPERATION_INFORMATION pOperationInformation)
{
    UNREFERENCED_PARAMETER(RegistrationContext);
    UNREFERENCED_PARAMETER(pOperationInformation);

    return OB_PREOP_SUCCESS;
}

You can see the POB_PRE_OPERATION_INFORMATION

this is define in wdm.h

typedef struct _OB_PRE_OPERATION_INFORMATION {
    _In_ OB_OPERATION           Operation;
    union {
        _In_ ULONG Flags;
        struct {
            _In_ ULONG KernelHandle:1;
            _In_ ULONG Reserved:31;
        };
    };
    _In_ PVOID                         Object;
    _In_ POBJECT_TYPE                  ObjectType;
    _Out_ PVOID                        CallContext;
    _In_ POB_PRE_OPERATION_PARAMETERS  Parameters;
} OB_PRE_OPERATION_INFORMATION, *POB_PRE_OPERATION_INFORMATION;

Operation is that indicates what operation. This member might be one of the following values OB_OPERATION_HANDLE_CREATE or OB_OPERATION_HANDLE_DUPLICATE
OB_OPERATION_HANDLE_CREATE is a new handle to a process or thread will be opened. Use Parameters->CreateHandleInformation for create-specific information.
OB_OPERATION_HANDLE_DUPLICATE is a process or thread handle will be duplicated. Use Parameters->DuplicateHandleInformation for duplicate-specific information.

KernelHandle is specifies whether the handle is a kernel handle. If this value is TRUE, the handle is a kernel handle. Otherwise, the handle is not a kernel handle.
This flag allows the driver to perhaps ignore kernel request. cause kernel handles can only created by kernel code

Object is the pointer to the object for that a handle is create, open, duplicate
if the target is process, Object is Eprocess structre address, or is thread, Object is ETHREAD structure address

ObjectType is object type like, PsProcessType, PsThreadType or ExDesktopObjectType.

CallContext is a pointer to driver-specific context information for the operation. By default, the Filter Manager sets this member to NULL, but the ObjectPreCallback routine can reset the CallContext member in a driver-specific manner. The Filter Manager passes this value to the matching ObjectPostCallback routine.

Parameter is OB_PRE_OPERATION_PARAMERTS structure

this is define in wdm.h

typedef union _OB_PRE_OPERATION_PARAMETERS {
    _Inout_ OB_PRE_CREATE_HANDLE_INFORMATION        CreateHandleInformation;
    _Inout_ OB_PRE_DUPLICATE_HANDLE_INFORMATION     DuplicateHandleInformation;
} OB_PRE_OPERATION_PARAMETERS, *POB_PRE_OPERATION_PARAMETERS;

Now we have to look up OB_PRE_CREATE_HANDLE_INFORMATION, OB_PRE_DUPLICATE_HANDLE_INFORMATION

Let's see OB_PRE_CREATE_HANDLE_INFORMATION

this is define in wdm.h

typedef struct _OB_PRE_CREATE_HANDLE_INFORMATION {
    _Inout_ ACCESS_MASK         DesiredAccess;
    _In_ ACCESS_MASK            OriginalDesiredAccess;
} OB_PRE_CREATE_HANDLE_INFORMATION, *POB_PRE_CREATE_HANDLE_INFORMATION;

Normally DesiredAccess is copy of what is in OriginalDesiredAccess. 

OriginalDesiredAccess is an access mask specified by the caller.

this is define in wdm.h

//
//  The following are masks for the predefined standard access types
//
Drivers can use the following flags for handles to processes:

PROCESS_CREATE_PROCESS      Create a new child process of the process.  
PROCESS_CREATE_THREAD   Create a new thread in the context of the process.  
PROCESS_DUP_HANDLE      Duplicate handles to or from the context of the process, such as by calling the user-mode DuplicateHandle routine.  
PROCESS_SET_QUOTA   Set the working set size for the process, such as by calling the user-mode SetProcessWorkingSetSize routine.  
PROCESS_SET_INFORMATION     Modify process settings, such as by calling the user-mode SetPriorityClass routine.  
PROCESS_SUSPEND_RESUME   Suspend or resume the process.  
PROCESS_TERMINATE    Terminate the process, such as by calling the user-mode TerminateProcess routine..  
PROCESS_VM_OPERATION    Modify the address space of the process, such as by calling the user-mode WriteProcessMemory and VirtualProtectEx routines.  
PROCESS_VM_WRITE     Write to the address space of the process, such as by calling the user-mode WriteProcessMemory routine.  
  
Drivers can use the following flags for handles to threads:

THREAD_DIRECT_IMPERSONATION  Enable a server thread to impersonate one of its clients.  
THREAD_IMPERSONATE  Impersonate the operating system's anonymous logon token, such as by calling the user-mode ImpersonateAnonymousToken routine.  
THREAD_SET_CONTEXT  Modify the thread's execution context, such as by calling the user-mode SetThreadContext routine.  
THREAD_SET_INFORMATION  Modify thread settings, such as by calling the user-mode SetThreadIdealProcessor routine. The operations that are permitted by this access right are a superset of those that are permitted by the THREAD_SET_LIMITED_INFORMATION access right.  
THREAD_SET_LIMITED_INFORMATION  Modify a limited set of thread settings, such as by calling the user-mode SetThreadAffinityMask and SetThreadPriorityBoost routines.  
THREAD_SET_THREAD_TOKEN  Modify properties of the thread's impersonation token, such as by calling the user-mode SetTokenInformation routine.  
THREAD_SUSPEND_RESUME  Suspend or resume the thread, such as by calling the user-mode SuspendThread and ResumeThread routines.  
THREAD_TERMINATE  Terminate the thread, such as by calling the user-mode TerminateThread routine.  


#define DELETE                           (0x00010000L)
#define READ_CONTROL                     (0x00020000L)
#define WRITE_DAC                        (0x00040000L)
#define WRITE_OWNER                      (0x00080000L)
#define SYNCHRONIZE                      (0x00100000L)
#define STANDARD_RIGHTS_REQUIRED         (0x000F0000L)
#define STANDARD_RIGHTS_READ             (READ_CONTROL)
#define STANDARD_RIGHTS_WRITE            (READ_CONTROL)
#define STANDARD_RIGHTS_EXECUTE          (READ_CONTROL)
#define STANDARD_RIGHTS_ALL              (0x001F0000L)
#define SPECIFIC_RIGHTS_ALL              (0x0000FFFFL)
//
// AccessSystemAcl access type
//
#define ACCESS_SYSTEM_SECURITY           (0x01000000L)
//
// MaximumAllowed access type
//
#define MAXIMUM_ALLOWED                  (0x02000000L)
//
//  These are the generic rights.
//
#define GENERIC_READ                     (0x80000000L)
#define GENERIC_WRITE                    (0x40000000L)
#define GENERIC_EXECUTE                  (0x20000000L)
#define GENERIC_ALL                      (0x10000000L)



You can see usually values that

40
1000
1010
1028
102A
1400
2000
21410
100000
101000
1FFFFF

this will explain later.




Now see the OB_PRE_DUPLICATE_HANDLE_INFORMATION strucutre

this is define in wdm.h

typedef struct _OB_PRE_DUPLICATE_HANDLE_INFORMATION {
    _Inout_ ACCESS_MASK         DesiredAccess;
    _In_ ACCESS_MASK            OriginalDesiredAccess;
    _In_ PVOID                  SourceProcess;
    _In_ PVOID                  TargetProcess;
} OB_PRE_DUPLICATE_HANDLE_INFORMATION, * POB_PRE_DUPLICATE_HANDLE_INFORMATION;

DesiredAccess, OriginalDesiredAccess is same with _OB_PRE_CREATE_HANDLE_INFORMATION's entity

SourceProcess is which a handle is being duplicated

TargetProcess is process the new handle will be duplicated into




2nd, let's see the post operation callback function.

except for ReturnStatus and Parameters, it is the same as _OB_PRE_OPERATION_INFORMATION so omit

this is define in wdm.h

typedef struct _OB_POST_OPERATION_INFORMATION {
    _In_ OB_OPERATION  Operation;
    union {
        _In_ ULONG Flags;
        struct {
            _In_ ULONG KernelHandle:1;
            _In_ ULONG Reserved:31;
        };
    };
    _In_ PVOID                          Object;
    _In_ POBJECT_TYPE                   ObjectType;
    _In_ PVOID                          CallContext;
    _In_ NTSTATUS                       ReturnStatus;
    _In_ POB_POST_OPERATION_PARAMETERS  Parameters;
} OB_POST_OPERATION_INFORMATION,*POB_POST_OPERATION_INFORMATION;

ReturnStatus is user will get back a valid handle operation.

Parameters is POB_POST_OPERATION_PARAMETERS structures 

this is define in wdm.h

typedef union _OB_POST_OPERATION_PARAMETERS {
  OB_POST_CREATE_HANDLE_INFORMATION    CreateHandleInformation;
  OB_POST_DUPLICATE_HANDLE_INFORMATION DuplicateHandleInformation;
} OB_POST_OPERATION_PARAMETERS, *POB_POST_OPERATION_PARAMETERS;


Now we have to look up OB_POST_CREATE_HANDLE_INFORMATION, OB_PRE_DUPLICATE_HANDLE_INFORMATION

Let's see OB_POST_CREATE_HANDLE_INFORMATION

this is define in wdm.h

typedef struct _OB_POST_CREATE_HANDLE_INFORMATION {
  ACCESS_MASK GrantedAccess;
} OB_POST_CREATE_HANDLE_INFORMATION, *POB_POST_CREATE_HANDLE_INFORMATION;

GrantedAccess specifies the access that is granted for the handle

Now see the OB_POST_DUPLICATE_HANDLE_INFORMATION strucutre

this is define in wdm.h

typedef struct _OB_POST_DUPLICATE_HANDLE_INFORMATION {
    _In_ ACCESS_MASK            GrantedAccess;
} OB_POST_DUPLICATE_HANDLE_INFORMATION, * POB_POST_DUPLICATE_HANDLE_INFORMATION;

same as OB_POST_CREATE_HANDLE_INFORMATION

