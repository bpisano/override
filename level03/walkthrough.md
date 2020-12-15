# Level 3

On commence par décompiler le code. La fonction `main` va appeler plusieurs fonctions.

```C
undefined4 main(void)
{
	int32_t iStack20;

	time();
	srand();
	puts();
	puts();
	puts();
	printf();
	__isoc99_scanf();
	test(iStack20, 0x1337d00d);
	return 0;
}
```
On observe un `scanf`. Cela sigfnique que notre programme attend un `input` en entrée standart.
Le `main` va faire un `call` à la fonction `test` avec deux arguments.
Le premier argument est notre `input`, puis le deuxième est le nombre
`0x1337d00d`, ce qui correspond à `322424845` en base 10.

```C
arg_ch = arg_ch - arg_8h;
switch(arg_ch) {
default:
	arg_8h_00 = rand();
	decrypt(arg_8h_00);
	break;
case 1:
	decrypt(arg_ch);
	break;
[...]
case 0x15:
	decrypt(arg_ch);
}
return;
```
La fonction `test` commence par faire `322424845` - `input`.
Suite à cela, un `switch` va être effectué avec les différentes valeurs ci-dessous.
```
1, 2, 3, 4, 5, 6, 7, 8, 9, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15
```
Si le résultat de la soustraction correspond avec l'une de ces valeurs-ci, alors la fonction `decrypt` va être appelé avec `322424845` comme argument.
Si le résultat de la soustraction ne correspond pas avec ces différentes valeurs, alors la fonction `decrypt` va être appelé, mais avec un nombre généré aléatoirement, comme nous l'indique le `default` dans le `switch`.

Nous savons qu'en testant les 13 valeurs correpsondant au résultats de la soustraction, un de ces résulats sera le bon et la fonction `decrypt` éxécutera un `shell` avec les droits de l'utilisateur `level04`.

Nous pouvons donc faire un `script` `bash` qui va nous permettre de lancer le `programme` avec les différentes valeurs vu précédemment.
Premièrement, on déclare un tableau contenant le résultat de la soustraction entre `322424845` et les différentes valeurs du `switch`. 

```
array=(322424845-1, 322424845-2, 322424845-3, 322424845-4, 322424845-5, 322424845-6, 322424845-7, 322424845-8, 322424845-9, 322424845-16, 322424845-17, 322424845-18, 322424845-19, 322424845-20, 322424845-21)
```

Ensuite, on boucle sur l'exécutable, en donnant comme argument notre nombre. Il nous suffit d'afficher le `password` du `level04`.

```
for i in "${array[@]}"; do echo $i; (python -c "print $i"; cat -) | ./level03; done
[...]
322424827,
***********************************
*		level03					 **
***********************************
cat /home/users/level04/.pass
kgv3tkEb9h2mLkRsPkXRfc2mHbjMxQzvb2FrgKkf
```