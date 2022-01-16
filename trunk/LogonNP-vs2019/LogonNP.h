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

#pragma once
