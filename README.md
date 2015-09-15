Dream Machine System Utilities: Macbook Keyboard Backlight
==============================
Neat Macbook keyboard backlight setter/getter written in C.

I have only made a couple of modifications, all credit goes to its original author: 
ewaller @ https://bbs.archlinux.org/viewtopic.php?id=197863

Installation & allowing access to all users.
----
```
$user: gcc backlight.c
$user: su 
$root: mv a.out /usr/share/bin/backlight.c
$root: cmod +s !$
$root: exit
``` 