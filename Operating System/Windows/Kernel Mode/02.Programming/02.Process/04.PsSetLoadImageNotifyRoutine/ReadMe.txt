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

Properties : Rpresents all the bi values within the struct that Properties is unioned with

ImageAddressIngMode : Always set to IMAGE_ADDRESSIGN_MODE_32bit

SystemModeImage : if kernle image - 1 else user image - 0

ImageMappedToAllPids : Always zero

ExtendInfoPresent : if this flag 9s set, IMAGE_INFO structure is part of IMAGE_INFO_EX 

MachineTypeMismatch : Always zero

ImageSignatureLevel : which code integrity has labeld the image. This value is one of SE_SIGNING_LEVEL the constatns in ntddk.h

ImageSignatureType : which code integrity has labeld the image. This value is a SE_IMAGE_SIGNATURE_TYPE enmueration value

Image_PartialMap : as stated in the annotation, This value is non-zero if the image mapping view called out for is a partial view that does not map the entire image

Reserved : Always zero

ImageBase : the virtual address in to which the image is loaded. this value is same as PEB's ImageBase

ImageSize : size of image

ImageSectionNumber : always zero

Let's see at SE_SIGNING_LEVEL from ImageSignatureType, one of the fields above

This is defined in ntddk.h

typedef UCHAR SE_SIGNING_LEVEL, *PSE_SIGNING_LEVEL;

#define SE_SIGNING_LEVEL_UNCHECKED         0x00000000
#define SE_SIGNING_LEVEL_UNSIGNED          0x00000001
#define SE_SIGNING_LEVEL_ENTERPRISE        0x00000002
#define SE_SIGNING_LEVEL_CUSTOM_1          0x00000003
#define SE_SIGNING_LEVEL_DEVELOPER         SE_SIGNING_LEVEL_CUSTOM_1
#define SE_SIGNING_LEVEL_AUTHENTICODE      0x00000004
#define SE_SIGNING_LEVEL_CUSTOM_2          0x00000005
#define SE_SIGNING_LEVEL_STORE             0x00000006
#define SE_SIGNING_LEVEL_CUSTOM_3          0x00000007
#define SE_SIGNING_LEVEL_ANTIMALWARE       SE_SIGNING_LEVEL_CUSTOM_3
#define SE_SIGNING_LEVEL_MICROSOFT         0x00000008
#define SE_SIGNING_LEVEL_CUSTOM_4          0x00000009
#define SE_SIGNING_LEVEL_CUSTOM_5          0x0000000A
#define SE_SIGNING_LEVEL_DYNAMIC_CODEGEN   0x0000000B
#define SE_SIGNING_LEVEL_WINDOWS           0x0000000C
#define SE_SIGNING_LEVEL_CUSTOM_7          0x0000000D
#define SE_SIGNING_LEVEL_WINDOWS_TCB       0x0000000E
#define SE_SIGNING_LEVEL_CUSTOM_6          0x0000000F

And SE_IMAGE_SIGNATURE_TYPE

Also this is defined in ntddk.h

typedef enum _SE_IMAGE_SIGNATURE_TYPE
{
    SeImageSignatureNone = 0, //0
    SeImageSignatureEmbedded,
    SeImageSignatureCache,
    SeImageSignatureCatalogCached,
    SeImageSignatureCatalogNotCached,
    SeImageSignatureCatalogHint,
    SeImageSignaturePackageCatalog,
} SE_IMAGE_SIGNATURE_TYPE, *PSE_IMAGE_SIGNATURE_TYPE;

Last, we need to see the IMAGE_INFO_EX that is explaining ExtendInfoPresent

this is defined in ntddk.h

typedef struct _IMAGE_INFO_EX {
    SIZE_T              Size;
    IMAGE_INFO          ImageInfo;
    struct _FILE_OBJECT *FileObject;
} IMAGE_INFO_EX, *PIMAGE_INFO_EX;

Size : IMAGE_INFO_EX size

ImageInfo : this is explained

FileObject  : Pointer to the file object of the backing file for the image. The driver can take a reference to this object or use it for other operations.

Addintionally, if you access this structure, must use the CONTAINING_RECORD(defined in ntdef.h)

But not explained here

