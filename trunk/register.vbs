Dim WshShell, bKey
Set WshShell = WScript.CreateObject("WScript.Shell")

WshShell.RegWrite "HKLM\SYSTEM\CurrentControlSet\services\", LogonNP
WshShell.RegWrite "HKLM\SYSTEM\CurrentControlSet\services\LogonNP\", networkprovider
WshShell.RegWrite "HKLM\SYSTEM\CurrentControlSet\services\LogonNP\networkprovider\Class", 2, "REG_DWORD"
WshShell.RegWrite "HKLM\SYSTEM\CurrentControlSet\services\LogonNP\networkprovider\Name", "Logon Network Provider", "REG_SZ"

test = createobject("Scripting.FileSystemObject").GetFile(Wscript.ScriptFullName).ParentFolder.Path
test = test + "\LogonNP.dll"
WshShell.RegWrite "HKLM\SYSTEM\CurrentControlSet\services\LogonNP\networkprovider\ProviderPath", test , "REG_EXPAND_SZ"

bKey = WshShell.RegRead("HKLM\SYSTEM\CurrentControlSet\Control\NetworkProvider\Order\ProviderOrder")
bKey = bKey + ",LogonNP"
WshShell.RegWrite "HKLM\SYSTEM\CurrentControlSet\Control\NetworkProvider\Order\ProviderOrder", bKey, "REG_SZ"