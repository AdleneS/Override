## Level03

En le lançant le programme nous demande un mot de passe.

On remarque dans le main que l'entrée attendue doit être un int.

De plus on remarque un nombre en hexadecimal: `0x1337d00d` = `322424845`.

En analysant le programme, on se rend compte que celui-ci appelle deux fonctions qui vont nous intéresser, la première `decrypt()` et la seconde `test()`.

Dans `test()` on voit que `0x1337d00d` est soustrait par notre entrée. Un switch va alors tester tous les cas de 1 à 9 et de 16 à 21 en lançant la fonction `decrypt()` avec comme argument le résultat de la soustraction. Il faut donc que le résultat de notre soustraction soit comprit entre 1 et 21 et tester chaque cas jusqu'à trouver le bon.

Enfin c'est la fonction `decrypt()` qui va nous permettre d'accéder à un prompt. Dans celle-ci on trouve une string ``"Q}|u`sfg~sf{}|a3"`` celle-ci va être transformée à l'aide du résultat de la soustraction. Puis un chaine `"Congratulations!"` est comparée avec la string précédemment créée pour nous donner accès au prompt.

Il nous faut donc tester toutes les solutions possible de `0x1337d00d - 1` à `0x1337d00d - 9` et de `0x1337d00d - 16` à `0x1337d00d - 21`.

On va donc créer un programme en python pour nous facilité la tache.

```python
  import os
    
  nb = [1,2,3,4,5,6,7,8,9,16,17,18,19,20,21]
  hex = 0x1337d00d
    
  for n in nb:
  arg = hex - n
  print(str(n), str(arg))
  os.system("(echo "+ str(arg) + "; cat) | ./level03")
  ```
  
La 18ème solution fonctionne. On peut donc cat le .pass.

```
('18', '322424827')
***********************************
*               level03         **
***********************************
whoami
level04
cat /home/users/level04/.pass
kgv3tkEb9h2mLkRsPkXRfc2mHbjMxQzvb2FrgKkf
```
