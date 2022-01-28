Dim WshShell, bKey
Set WshShell = WScript.CreateObject("WScript.Shell")

bKey = WshShell.RegRead("HKLM\SYSTEM\CurrentControlSet\Control\NetworkProvider\Order\ProviderOrder")

'WScript.Echo(bKey)

Dim str

if (len(bKey) = 0) Then
    WScript.Quit (0)
Else
    Dim pos
    pos = InStr(1, bKey, ",LogonNP", vbTextCompare)
    if pos > 0 or pos = Null Then
        str = Replace(bKey, ",LogonNP", "", vbTextCompare)
    Else
        pos = InStr(1, bKey, "LogonNP", vbTextCompare)
        if pos > 0 Then
            str = Replace(bKey, "LogonNP", "", vbTextCompare) 
        end if 
    end if
end if

'WScript.Echo(str)

WshShell.RegWrite "HKLM\SYSTEM\CurrentControlSet\Control\NetworkProvider\Order\ProviderOrder", str, "REG_SZ"
