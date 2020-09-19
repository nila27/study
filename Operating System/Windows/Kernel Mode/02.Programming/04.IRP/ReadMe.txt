
This is defined in wdm.h

typedef struct DECLSPEC_ALIGN(MEMORY_ALLOCATION_ALIGNMENT) _IRP {
    CSHORT Type;
    USHORT Size;

    PMDL MdlAddress;
    ULONG Flags;
    union {
        struct _IRP *MasterIrp;
        __volatile LONG IrpCount;
        PVOID SystemBuffer;
    } AssociatedIrp;

    LIST_ENTRY ThreadListEntry;
    IO_STATUS_BLOCK IoStatus;
    KPROCESSOR_MODE RequestorMode;
    BOOLEAN PendingReturned;
    CHAR StackCount;
    CHAR CurrentLocation;
    BOOLEAN Cancel;
    KIRQL CancelIrql;
    CCHAR ApcEnvironment;
    UCHAR AllocationFlags;
    PIO_STATUS_BLOCK UserIosb;
    PKEVENT UserEvent;
    union {
        struct {
            union {
                PIO_APC_ROUTINE UserApcRoutine;
                PVOID IssuingProcess;
            };
            PVOID UserApcContext;
        } AsynchronousParameters;
        LARGE_INTEGER AllocationSize;
    } Overlay;

    __volatile PDRIVER_CANCEL CancelRoutine;
    PVOID UserBuffer;
    
    union {

        struct {

            union {
            
                KDEVICE_QUEUE_ENTRY DeviceQueueEntry;

                struct {

                    PVOID DriverContext[4];

                } ;

            } ;
            
            PETHREAD Thread;
            PCHAR AuxiliaryBuffer;
            
            struct {
            
                LIST_ENTRY ListEntry;

                union {

                    struct _IO_STACK_LOCATION *CurrentStackLocation;
                    ULONG PacketType;
                };
            };

            PFILE_OBJECT OriginalFileObject;

        } Overlay;

        KAPC Apc;

        PVOID CompletionKey;

    } Tail;

} IRP;




