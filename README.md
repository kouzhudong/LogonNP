# LogonNP
Windows NT Network provider Sample

本来是不想创建这个仓库的。  
这是一个很简单的，只有一两个文件，总共代码不足两百行的小技巧。  
同时，这也是一个很早（2003的SDK）的，十多年前都有不少人知道的小知识。  
但是，这也是一类功能：大多能被lsass.exe加载的DLL，顺便还能获取登录Windows操作系统的用户的密码。  
为了整理和分享的目的建立并分享此仓库。  

听说这个功能在Windows 11上还能用。  
在Windows 10上测试还可以，但是加载的进程不是lsass.exe，而是mpnotify.exe。  
