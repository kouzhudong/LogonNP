reg delete "HKLM\SYSTEM\CurrentControlSet\services\LogonNP" /f 

rem 手动删除HKLM\SYSTEM\CurrentControlSet\Control\NetworkProvider\Order里的ProviderOrder里的LogonNP。
rem reg delete "HKLM\SYSTEM\CurrentControlSet\Control\NetworkProvider\Order" /v ProviderOrder /t REG_SZ /d LogonNP