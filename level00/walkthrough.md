# Level 0

Lorsqu'on décompile le code avec `Cutter`, on obtient un code très explicite.

```C
__isoc99_scanf(0x8048636, aiStack20);
if (aiStack20[0] != 0x149c) {
	puts("\nInvalid Password!")
} else {
	puts("\nAuthenticated!");
	system("/bin/sh");
}
return aiStack20[0] != 0x149c;
```

Premièrement, on remarque qu'il y a un `scanf`. Cela signifie que le programme attend un `input` en entrée standard.

On observe également qu'il y a une comparaison entre notre `input` et `0x149c`.
Si notre `input` correspond à `0x149c`, alors un `shell` sera lancé avec les droits de l'utilisateur `level01`.

`0x149c` correspond à `5276` en `base10`. 
Il faut donc que notre `input` soit `5276`.

```
> ./level00
[...]
Password: 5276
$ whoami
level01
$ cat /home/users/level01/.pass
uSq2ehEGT6c9S24zbshexZQBXUGrncxn5sD5QfGL
```