Dream Machine: MB KB Backlight
==============================
Neat Macbook (Pro/Air) keyboard backlight setter/getter written in C.

It's really only a couple of small changes applied to https://github.com/Lindstream/dm-sys-backlight

All credit goes to Backlight's original author: ewaller @ https://bbs.archlinux.org/viewtopic.php?id=197863

Installation & allowing access to all users.
----
```
$user: gcc keylight.c
$user: su 
$root: mv a.out /usr/share/bin/keylight.c
$root: chmod +s !$
$root: exit
``` 