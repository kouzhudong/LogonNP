rem 本文件不可命名为reg.bat，否则无穷递归，你想吧！

echo "确保本文件和logonnp.dll在同一目录"
echo "确保当前目录和logonnp.dll在同一目录 或者 直接双击本脚本运行"

if not exist %cd%\logonnp.dll exit

reg add "HKLM\SYSTEM\CurrentControlSet\services\LogonNP"

reg add "HKLM\SYSTEM\CurrentControlSet\services\LogonNP\networkprovider"

reg add "HKLM\SYSTEM\CurrentControlSet\services\LogonNP\networkprovider" /v Class /t REG_DWORD /d 2
reg add "HKLM\SYSTEM\CurrentControlSet\services\LogonNP\networkprovider" /v Name /t REG_SZ /d "Logon Network Provider"
reg add "HKLM\SYSTEM\CurrentControlSet\services\LogonNP\networkprovider" /v ProviderPath /t REG_EXPAND_SZ /d %cd%\logonnp.dll

rem 后面要添加LogonNP。
rem 这个需要手动操作。
rem reg add "HKLM\SYSTEM\CurrentControlSet\Control\NetworkProvider\Order" /v ProviderOrder /t REG_SZ /d LogonNP
