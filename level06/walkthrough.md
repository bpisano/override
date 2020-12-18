# Level 6

On commence par décompiler l'exécutable. Dans la fonction `main`, on remarque qu'il y un a appel à la fonction `auth`. Si le retour de la fonction `auth` est égal à `0`, alors un `shell` avec les droits de l'utilisateur `level06` sera lancé. 

```C
fgets (eax, *(stdin), 0x20);
iVar1 = auth(acStack52, iStack56);
if (iVar1 == 0) {
	puts();
	system();
}
```

On remarque que la fonction `fgets` est appelée. Notre programme attend donc un argument en entrée standard. En exécutant le programme, on se rend compte que deux entrées sont attendus. 

Notre objectif sera donc que la fonction `auth` retourne `0`.
Lorsque l'on observe la fonction `auth`, on remarque un appel à la fonction `ptrace`.


```C
else {
   eax = ptrace (0, 0, 1, 0);
   if (eax == -1) {
      puts ("\e[31m| !! TAMPERING DETECTED !! |");
   }
}
```

```
L'appel système **ptrace**() fournit au processus parent un moyen de contrôler l'exécution d'un autre processus et d'éditer son image mémoire. L'utilisation primordiale de cette fonction est l'implémentation de points d'arrêt pour le débogage, et pour suivre les appels système.
```

`ptrace` nous empêche d'utiliser `gdb`, il va donc falloir contourner ce problème. 
On observe que `ptrace` nous bloque dans l'utilisation de `gdb` si `eax == -1`. 
Si on met `eax` à `0`, `ptrace` va interpréter cela comme un succès et ne va pas nous bloquer.

Nous pouvons donc observer la fonction `auth` désassemblé dans `gdb`.

```
> (gdb) disass auth
[...]
   0x08048863 <+283>:	mov    0xc(%ebp),%eax
   0x08048866 <+286>:	cmp    -0x10(%ebp),%eax
   0x08048869 <+289>:	je     0x8048872 <auth+298>
[...]
   0x08048878 <+304>:	ret
End of assembler dump.
```

On observe que juste avant le retour de la fonction `auth`, une instruction `cmp ` est effectuée, ce qui est donc une comparaison. Nous allons `break` ici pour observer la `stack` et voir la valeur contenue dans `ebp-0x10`.
Nous allons devoir utiliser ces commandes dans `gdb` avant de `run` le programme afin que `ptrace` ne nous bloque pas l'accès : 
```
catch syscall ptrace
commands 1
set ($eax) = 0
continue
end
```
``` 
> (gdb) b *0x08048866
[...]
> (gdb) r 
[...]
-> Enter Login: themarch
[...]
-> Enter Serial: aaa
[...]
> (gdb) x $ebp-0x10
0xffffd678:	0x005f216e
```
Nous avons affiché l'adresse de la valeur de comparaison. 
`0x005f216e` en base 10 équivaut à `6234478`.
Après plusieurs essais, nous nous sommes rendus compte que cette valeur obtenue était le `serial` attendu. Nous nous sommes également rendu compte que la valeur de comparaison était propre à chaque login.
En effet, en saisissant `bpisano` comme `login`, notre serial sera différent.

```
> (gdb) r 
[...]
-> Enter Login: bpisano
[...]
-> Enter Serial: aaa
[...]
> (gdb) x $ebp-0x10
0xffffd678:	0x005f1ec2
```

On remarque ici, qu'après avoir saisis `bpisano` comme `login`, le `serial` attendue est : `0x005f1ec2` soit `6233794` en base 10. Le `serial` est donc différent, en fonction de chaque `login`.

Nous pouvons donc lancer notre programme avec par exemple `bpisano` et `6233794` comme `input`.

En observant le code, nous avons remarqué qu'il y avait un `strnlen` d'effectué. `iVar1` semble être notre premier `input`, à savoir : `login`. Si le `login` est inférieur à `6` caractères, alors la valeur de retour de la fonction `auth` sera de `1`. Nous pouvons vérfier cela dans `gdb`.

```C
iVar1 = strnlen();
if (iVar1 < 6) {
	uVar2 = 1;
}
[...]
return uVar2;
```

En saisissant un `login` inférieur à 6 caractères, nous ne rentrons pas dans cette condition de comparaison.

```
> (gdb) r 
[...]
-> Enter Login: the
[...]
-> Enter Serial: aaa
[Inferior 1 (process 2611) exited with code 01]
(gdb) x $ebp-0x10
No registers.
```

```
> ./level06
***********************************
*		level06		  *
***********************************
-> Enter Login: bpisano
***********************************
***** NEW ACCOUNT DETECTED ********
***********************************
-> Enter Serial: 6233794
Authenticated!
$ whoami
level07
$ cat /home/users/level07/.pass
GbcPDRgsFK77LNnnuh7QyFYA2942Gp8yKj9KrWD8
```