#include <ntddk.h>

PVOID hRegist = NULL;

OB_PREOP_CALLBACK_STATUS PreCallback(PVOID RegistrationContext, POB_PRE_OPERATION_INFORMATION pOperationInformation)
{
    UNREFERENCED_PARAMETER(RegistrationContext);
    UNREFERENCED_PARAMETER(pOperationInformation);

    return OB_PREOP_SUCCESS;
}

VOID PostCallback(PVOID RegistrationConext, POB_POST_OPERATION_INFORMATION pOperationInformation)
{
    UNREFERENCED_PARAMETER(RegistrationConext);
    UNREFERENCED_PARAMETER(pOperationInformation);

    PEPROCESS PP = (PEPROCESS)pOperationInformation->Object;

    DbgPrintEx(DPFLTR_IHVDRIVER_ID, 0, "Process Name    :   %s",(const char*)(unsigned char*)PP + 0x450);
}

NTSTATUS ObRegistercallbackfunction() 
{
    OB_CALLBACK_REGISTRATION OCR = { 0, };
    OB_OPERATION_REGISTRATION OOR = { 0, };

    OCR.Version = OB_FLT_REGISTRATION_VERSION;
    OCR.OperationRegistrationCount = 1;

    RtlInitUnicodeString(&OCR.Altitude, L"1234");
    OCR.RegistrationContext = NULL;

    OOR.ObjectType = PsProcessType;
    OOR.Operations = OB_OPERATION_HANDLE_CREATE;
    OOR.PreOperation = PreCallback;
    OOR.PostOperation = PostCallback;

    OCR.OperationRegistration = &OOR;

    return ObRegisterCallbacks(&OCR, &hRegist);
}


VOID DriverUnload(_In_ PDRIVER_OBJECT driverobject)
{
	UNREFERENCED_PARAMETER(driverobject);

    ObUnRegisterCallbacks(hRegist);

    DbgPrintEx(DPFLTR_IHVAUDIO_ID, 0, "Stop");
}

NTSTATUS DriverEntry(_In_ PDRIVER_OBJECT driverobject, _In_ PUNICODE_STRING RegistryPath)
{
	UNREFERENCED_PARAMETER(RegistryPath);

    driverobject->DriverUnload = DriverUnload;

    DbgPrintEx(DPFLTR_IHVAUDIO_ID, 0, "Start");

    NTSTATUS NTS = STATUS_SUCCESS;

    NTS = ObRegistercallbackfunction();

    return STATUS_SUCCESS;
}

