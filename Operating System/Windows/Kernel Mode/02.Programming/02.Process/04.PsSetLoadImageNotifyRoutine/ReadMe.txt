This function is called when process image load on memory

This is define in ntddk.h

NTSTATUS PsSetLoadImageNotifyRoutine( _In_ PLOAD_IMAGE_NOTIFY_ROUTINE NotifyRoutine);

Now we have to see PLOAD_IMAGE_NOTIFY_ROUTINE callback function

VOID (*PLOAD_IMAGE_NOTIFY_ROUTINE)( _In_opt_ PUNICODE_STRING FullImageName, _In_ HANDLE ProcessId, _In_ PIMAGE_INFO ImageInfo);

Let's see PIMAGE_INFO structure

This is define in ntddk.h

typedef struct _IMAGE_INFO {
    union {
        ULONG Properties;
        struct {
            ULONG ImageAddressingMode  : 8;  // Code addressing mode
            ULONG SystemModeImage      : 1;  // System mode image
            ULONG ImageMappedToAllPids : 1;  // Image mapped into all processes
            ULONG ExtendedInfoPresent  : 1;  // IMAGE_INFO_EX available
            ULONG MachineTypeMismatch  : 1;  // Architecture type mismatch
            ULONG ImageSignatureLevel  : 4;  // Signature level
            ULONG ImageSignatureType   : 3;  // Signature type
            ULONG ImagePartialMap      : 1;  // Nonzero if entire image is not mapped
            ULONG Reserved             : 12;
        };
    };
    PVOID       ImageBase;
    ULONG       ImageSelector;
    SIZE_T      ImageSize;
    ULONG       ImageSectionNumber;
} IMAGE_INFO, *PIMAGE_INFO;



