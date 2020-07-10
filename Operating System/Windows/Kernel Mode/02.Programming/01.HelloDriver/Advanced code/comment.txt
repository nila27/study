what is differnce with basic code?

Advanced code is used _In_ in function's parameter. 

_In_ is SAL(Source code Annotation Language) defined in the header file <sal.h>


Parameter Annotation _In_  is Data is passed to the called function, and is treated as read-only

why use SAL? cause that improve the security and reliability of your code. 

When Do I Annotate?

Here are some guidelines:

•Annotate all pointer parameters.


•Annotate value-range annotations so that Code Analysis can ensure buffer and pointer safety.


•Annotate locking rules and locking side effects. For more information, see Annotating Locking Behavior.


•Annotate driver properties and other domain-specific properties.


more information - 
https://docs.microsoft.com/ko-kr/cpp/code-quality/understanding-sal?view=vs-2019
https://docs.microsoft.com/ko-kr/windows-hardware/drivers/devtest/sal-2-annotations-for-windows-drivers
