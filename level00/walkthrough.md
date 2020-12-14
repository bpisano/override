# Level 0

Lorsqu'on décompile le code avec `Ghidra`, on obtient un code très explicite.

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

Il s'agit d'une comparaison entre notre `input` et un nombre en dur. 
Si notre `input` correspond aux nombres attendus, alors un `shell` sera lancé avec les droits du `level01`.

`0x149c` correspond à `5276`en `base10`. 
Il faut donc que notre `input` soit `5276` 

```
> ./level00
[...]
Password: 5276
$ whoami
level01
$ cat /home/users/level01/.pass
uSq2ehEGT6c9S24zbshexZQBXUGrncxn5sD5QfGL
```