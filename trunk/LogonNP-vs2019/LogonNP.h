/*
��ȡWindows�ĵ�¼�û�������
����һ����ΰ�������Ĳ���,ʼ���ڡ��ڿͷ��ߡ�,��ϸһ��,���Ǵ������ַ�����.
http://www.codeproject.com/Articles/12146/Login-password-filters-in-WinXP

��Ȼ���б�İ취,�����Winlogon���̺ͱ����ƽ�,�����ⶼ�о�����,������취�ǹٷ�������ͨ�õİ취.
�ο���Ϣ����:

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

��Ҫ�������ǲ���ע���,��������Ա��ʵ��:
1.HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Control\NetworkProvider��order��ProviderOrder�����һ��ֵ,ע�ⶺ��.
  HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Control\NetworkProvider��HwOrder��ProviderOrder����Բ�����.
2.HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Services�´����ղż����ֵ.
3.��������(�ڶ���)��·���´���networkprovider.
4.��������(������)��·���´�������ΪREG_DWORD�ļ���Class.
5.��������(������)��·���´�������ΪREG_MULTI_SZ�ļ���Name.
6.��������(������)��·���´�������ΪREG_MULTI_SZ�ļ���ProviderPath.
7.����û��:HKEY_LOCAL_MACHINE,"SYSTEM\\CurrentControlSet\\Control\\Lsa"��Notification Packages�����һ��ֵ.
8.HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Control\NetworkProvider\Notifyees���û��ʵ��.

ע��:
1.Make sure that you create a 32-bit password filter DLL for 32-bit computers and a 64-bit password filter DLL for 64-bit computers
  ��ȻDLL����ʧ��.
2.���Է������ں˵��ԣ���COM�����ص��Ժ��û����������Զ����еġ�
3.���ÿ��ע��֮��������������ע���ٵ�¼����Ҫ���µĶϵ�û�ж����������ǿ���ͨ������;���޸�����Ȳ�����
4.��ò�Ҫ��DLL�����֣����ϵ�����Եķ��ż������⡣
����ÿ�����ⶼ�˷����Ұ����ʱ�䡣

made by correy
made at 2014.03.06
email:kouleguan at hotmail dot com
homepage:http://correy.webs.com

������2014.03.10������SDK����Ҳ���ⶫ����·���ڣ�
Microsoft SDK\Samples\security\NetProviders��
���̵����ֽУ�LogonNP��
*/

#pragma once
