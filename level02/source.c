undefined8 main(undefined8 argc, char **argv)
{
    int32_t iVar1;
    undefined8 uVar2;
    int64_t iVar3;
    char **ppcVar4;
    undefined8 *puVar5;
    char **var_120h;
    int64_t var_114h;
    char *ptr;
    char *format;
    int32_t var_ch;
    int64_t stream;
    
    iVar3 = 0xc;
    ppcVar4 = &format;
    while (iVar3 != 0) {
        iVar3 = iVar3 + -1;
        *ppcVar4 = (char *)0x0;
        ppcVar4 = ppcVar4 + 1;
    }
    *(undefined4 *)ppcVar4 = 0;
    iVar3 = 5;
    ppcVar4 = &ptr;
    while (iVar3 != 0) {
        iVar3 = iVar3 + -1;
        *ppcVar4 = (char *)0x0;
        ppcVar4 = ppcVar4 + 1;
    }
    *(undefined *)ppcVar4 = 0;
    iVar3 = 0xc;
    puVar5 = (undefined8 *)((int64_t)&var_114h + 4);
    while (iVar3 != 0) {
        iVar3 = iVar3 + -1;
        *puVar5 = 0;
        puVar5 = puVar5 + 1;
    }
    *(undefined4 *)puVar5 = 0;
    stream = 0;
    var_ch = 0;
    var_114h._0_4_ = (undefined4)argc;
    stream = fopen("/home/users/level03/.pass", 0x400bb0, 0x400bb0);
    if (stream == 0) {
        fwrite("ERROR: failed to open password file\n", 1, 0x24, _reloc.stderr);
        exit(1);
    }
    var_ch = fread(&ptr, 1, 0x29, stream);
    iVar3 = strcspn(&ptr, 0x400bf5);
    *(undefined *)((int64_t)&ptr + iVar3) = 0;
    if (var_ch != 0x29) {
        fwrite("ERROR: failed to read password file\n", 1, 0x24, _reloc.stderr);
        fwrite("ERROR: failed to read password file\n", 1, 0x24, _reloc.stderr);
        exit(1);
    }
    fclose(stream);
    puts("===== [ Secure Access System v1.0 ] =====");
    puts(0x400c50);
    puts("| You must login to access this system. |");
    puts(0x400cb0);
    printf("--[ Username: ");
    fgets(&format, 100, _reloc.stdin);
    iVar3 = strcspn(&format, 0x400bf5);
    *(undefined *)((int64_t)&format + iVar3) = 0;
    printf("--[ Password: ");
    fgets((int64_t)&var_114h + 4, 100, _reloc.stdin);
    iVar3 = strcspn((int64_t)&var_114h + 4, 0x400bf5);
    *(undefined *)((int64_t)&var_114h + iVar3 + 4) = 0;
    puts("*****************************************");
    iVar1 = strncmp(&ptr, (int64_t)&var_114h + 4, 0x29, (int64_t)&var_114h + 4);
    if (iVar1 != 0) {
        printf(&format);
        puts(" does not have access!");
        exit(1);
        uVar2 = .init();
        return uVar2;
    }
    printf("Greetings, %s!\n", &format, &format);
    system("/bin/sh");
    return 0;
}
