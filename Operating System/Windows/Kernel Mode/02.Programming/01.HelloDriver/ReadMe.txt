What is DriverEntry

It is the driver's entry as its name suggest.
Let's see the code in source.c

NTSTATUS DriverEntry(PDRIVER_OBJECT DriverObject, PUNICODE_STRING RegistryPath)
{
	UNREFERENCED_PARAMETER(DriverObject);
	UNREFERENCED_PARAMETER(RegistryPath);


	DbgPrintEx(DPFLTR_IHVDRIVER_ID, 0, "Hello Driver");

	DriverObject->DriverUnload = DriverUnload;


	return STATUS_SUCCESS;
}


You can find 'UNREFERENCE_PARAMETER()'
this must be used unless you use the function's parameters. otherwise, an error occurs during the build process.
but when using parameter, using B doesn't cause an error

I used the DbgPrintEx(). but you can use KdPrint() it is similar.
And you can't see the text int the usual way. you must install the tool 'DbgView'  

driver must be unload. cause they induce leak. so i defined driverunload function. this function name isn't static like driverentry

VOID DriverUnload(PDRIVER_OBJECT DriverObject)
{
	UNREFERENCED_PARAMETER(DriverObject);
}

finally, return STATUS_SUCCESS
it is defined by 
#define STATUS_SUCCESS                   ((NTSTATUS)0x00000000L)    // ntsubauth



The kernel was mostly made by CLang and a little C++

so when programming in c++ to kernel, must use 'extern "C"' 

let's see source.cpp

#include <ntddk.h>

VOID DriverUnload(PDRIVER_OBJECT DriverObject)
{
	UNREFERENCED_PARAMETER(DriverObject);
}


extern "C" NTSTATUS DriverEntry(PDRIVER_OBJECT DriverObject, PUNICODE_STRING RegistryPath)
{
	UNREFERENCED_PARAMETER(RegistryPath);


	DbgPrintEx(DPFLTR_IHVDRIVER_ID, 0, "Hello Driver");

	DriverObject->DriverUnload = DriverUnload;


	return STATUS_SUCCESS;
}

It's similar to source.c except DrivereEntry function

