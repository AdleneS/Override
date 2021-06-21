
## LEVEL 06

Quand on lance le programme on nous demande un login, puis une serial key, mais rien ne semble marcher.

On remarque que le main appelle la fonction auth. Celle-ci effectue plusieurs comparaison avec l'entrer de login et hash notre login pour en créer une serial key. 

Cette serial key est ensuit comparer avec l'entrée "serial key".

On va donc avoir besoin de trouver la serial key qui fonctionne pour notre login.

Le programme demande un login d'au moins 6 caractère prenons : ABCDEFG.

Nous allons manipuler le programme à l'aide de gdb pour nous permettre d'afficher la valeur qui est comparer avec notre entrée dans "serial key". 

Il faudra donc contourner le ptrace de la fonction auth pour pouvoir passer à la suite du programme.
```
(gdb) disas auth
Dump of assembler code for function auth:
   ...
   0x080487ae <+102>:   movl   $0x0,(%esp)
   0x080487b5 <+109>:   call   0x80485f0 <ptrace@plt>   /// Ptrace
   0x080487ba <+114>:   cmp    $0xffffffff,%eax		/// 1ère Comparaison
   0x080487bd <+117>:   jne    0x80487ed <auth+165>	/// 1er breakpoint
   0x080487bf <+119>:   movl   $0x8048a68,(%esp)
   0x080487c6 <+126>:   call   0x8048590 <puts@plt>
   0x080487cb <+131>:   movl   $0x8048a8c,(%esp)
   0x080487d2 <+138>:   call   0x8048590 <puts@plt>
   0x080487d7 <+143>:   movl   $0x8048ab0,(%esp)
   0x080487de <+150>:   call   0x8048590 <puts@plt>
   0x080487e3 <+155>:   mov    $0x1,%eax
   0x080487e8 <+160>:   jmp    0x8048877 <auth+303>
   0x080487ed <+165>:   mov    0x8(%ebp),%eax		/// Fin du ptrace -> Jump
   0x080487f0 <+168>:   add    $0x3,%eax
   0x080487f3 <+171>:   movzbl (%eax),%eax
   0x080487f6 <+174>:   movsbl %al,%eax
   ...
   0x08048863 <+283>:   mov    0xc(%ebp),%eax
   0x08048866 <+286>:   cmp    -0x10(%ebp),%eax		/// 2ème Comparaison
   0x08048869 <+289>:   je     0x8048872 <auth+298>	/// 2ème Breakpoint
   0x0804886b <+291>:   mov    $0x1,%eax
   0x08048870 <+296>:   jmp    0x8048877 <auth+303>
   0x08048872 <+298>:   mov    $0x0,%eax
   0x08048877 <+303>:   leave
   0x08048878 <+304>:   ret
```

On va donc mettre des breakpoints pour arrêter le programme, dans un premier temps pour sauter la comparaison du ptrace() et dans un second temps pour nous permettre de print la valeur comparer avec notre serial key.

Créons les breakpoints :
```
(gdb) b *auth+117
Breakpoint 1 at 0x80487bd
(gdb) b *auth+289
Breakpoint 2 at 0x8048869
```

On lance le programme :
```
(gdb) r
Starting program: /home/users/level06/level06
***********************************
*               level06           *
***********************************
-> Enter Login: ABCDEFG
***********************************
***** NEW ACCOUNT DETECTED ********
***********************************
-> Enter Serial: AAAAAAAAA

Breakpoint 1, 0x080487bd in auth ()
(gdb) jump *auth+165
Continuing at 0x80487ed.

Breakpoint 2, 0x08048869 in auth ()
(gdb) x $ebp-0x10
0xffffd2a8:     0x005f1ab3
```

On print l'ebp-0x10 qui contient la valeur comparée.

0x005f1ab3 en décimal donne : **6232755**

On relance le programme, toujours avec le même login et on entre **6232755** comme serial key.

```
level06@OverRide:~$ ./level06
***********************************
*               level06           *
***********************************
-> Enter Login: ABCDEFG
***********************************
***** NEW ACCOUNT DETECTED ********
***********************************
-> Enter Serial: 6232755
Authenticated!
$ whoami
level07
$ cat /home/users/level07/.pass
GbcPDRgsFK77LNnnuh7QyFYA2942Gp8yKj9KrWD8
```
