void log_wrapper(undefined8 arg1, char *arg2, undefined8 arg3)
{
    char cVar1;
    int64_t iVar2;
    uint64_t uVar3;
    uint64_t uVar4;
    char **ppcVar5;
    int64_t in_FS_OFFSET;
    uint8_t uVar6;
    int64_t var_130h;
    undefined8 size;
    char *src;
    undefined8 stream;
    char *format;
    int64_t canary;
    
    uVar6 = 0;
    canary = *(int64_t *)(in_FS_OFFSET + 0x28);
    stream = arg1;
    strcpy(&format, arg2, arg2);
    uVar3 = 0xffffffffffffffff;
    ppcVar5 = &format;
    do {
        if (uVar3 == 0) break;
        uVar3 = uVar3 - 1;
        cVar1 = *(char *)ppcVar5;
        ppcVar5 = (char **)((int64_t)ppcVar5 + (uint64_t)uVar6 * -2 + 1);
    } while (cVar1 != '\0');
    uVar4 = 0xffffffffffffffff;
    ppcVar5 = &format;
    do {
        if (uVar4 == 0) break;
        uVar4 = uVar4 - 1;
        cVar1 = *(char *)ppcVar5;
        ppcVar5 = (char **)((int64_t)ppcVar5 + (uint64_t)uVar6 * -2 + 1);
    } while (cVar1 != '\0');
    snprintf((int64_t)&stream + ~uVar4 + 7, 0xfe - (~uVar3 - 1), arg3);
    iVar2 = strcspn(&format, 0x400d4c);
    *(undefined *)((int64_t)&format + iVar2) = 0;
    fprintf(stream, "LOG: %s\n", &format, "LOG: %s\n");
    if (canary != *(int64_t *)(in_FS_OFFSET + 0x28)) {
        __stack_chk_fail();
    }
    return;
}


undefined8 main(uint32_t argc, char **argv)
{
    char cVar1;
    int32_t iVar2;
    int64_t arg1;
    int64_t iVar3;
    undefined8 uVar4;
    uint64_t uVar5;
    char *pcVar6;
    int64_t in_FS_OFFSET;
    uint8_t uVar7;
    int64_t var_a8h;
    char **filename;
    uint32_t var_94h;
    undefined8 var_88h;
    undefined8 stream;
    uint32_t fildes;
    char *ptr;
    undefined2 uStack112;
    char cStack110;
    int64_t canary;
    
    uVar7 = 0;
    canary = *(int64_t *)(in_FS_OFFSET + 0x28);
    ptr._0_1_ = -1;
    if (argc != 2) {
        printf("Usage: %s filename\n", *argv, *argv);
    }
    arg1 = fopen("./backups/.log", 0x400d6b, 0x400d6b);
    if (arg1 == 0) {
        printf("ERROR: Failed to open %s\n", "./backups/.log");
        exit(1);
    }
    log_wrapper(arg1, "Starting back up: ", argv[1]);
    iVar3 = fopen(argv[1], 0x400da9, 0x400da9);
    if (iVar3 == 0) {
        printf("ERROR: Failed to open %s\n", argv[1], argv[1]);
        exit(1);
    }
    stack0xffffffffffffff88 = (char *)"./backups/"._0_8_;
    uStack112 = "./backups/"._8_2_;
    cStack110 = "./backups/"[10];
    uVar5 = 0xffffffffffffffff;
    pcVar6 = (char *)((int64_t)&ptr + 1);
    do {
        if (uVar5 == 0) break;
        uVar5 = uVar5 - 1;
        cVar1 = *pcVar6;
        pcVar6 = pcVar6 + (uint64_t)uVar7 * -2 + 1;
    } while (cVar1 != '\0');
    strncat((int64_t)&ptr + 1, argv[1], 99 - (~uVar5 - 1), argv[1]);
    iVar2 = open((int64_t)&ptr + 1, 0xc1, 0x1b0);
    if (-1 < iVar2) goto code_r0x00400bee;
    printf("ERROR: Failed to open %s%s\n", "./backups/", argv[1]);
    exit(1);
    do {
        write(iVar2, &ptr, 1, &ptr);
code_r0x00400bee:
        ptr._0_1_ = fgetc();
    } while ((char)ptr != -1);
    log_wrapper(arg1, "Finished back up ", argv[1]);
    fclose(iVar3);
    close(iVar2);
    uVar4 = 0;
    if (canary != *(int64_t *)(in_FS_OFFSET + 0x28)) {
        uVar4 = __stack_chk_fail();
    }
    return uVar4;
}