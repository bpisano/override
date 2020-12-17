void set_username(char *arg1)
{
    int64_t iVar1;
    char **ppcVar2;
    char *var_98h;
    char *s;
    char *var_4h;
    
    iVar1 = 0x10;
    ppcVar2 = &s;
    while (iVar1 != 0) {
        iVar1 = iVar1 + -1;
        *ppcVar2 = (char *)0x0;
        ppcVar2 = ppcVar2 + 1;
    }
    puts(0xbe4);
    printf(0xbdf);
    fgets(&s, 0x80, *_reloc.stdin);
    var_4h._0_4_ = 0;
    while (((int32_t)var_4h < 0x29 && (*(char *)((int64_t)&s + (int64_t)(int32_t)var_4h) != '\0'))) {
        arg1[(int64_t)(int32_t)var_4h + 0x8c] = *(char *)((int64_t)&s + (int64_t)(int32_t)var_4h);
        var_4h._0_4_ = (int32_t)var_4h + 1;
    }
    printf(0xbfb, arg1 + 0x8c, arg1 + 0x8c);
    return;
}

void set_msg(char *arg1)
{
    int64_t iVar1;
    char **ppcVar2;
    char *dest;
    char *src;
    
    iVar1 = 0x80;
    ppcVar2 = &src;
    while (iVar1 != 0) {
        iVar1 = iVar1 + -1;
        *ppcVar2 = (char *)0x0;
        ppcVar2 = ppcVar2 + 1;
    }
    puts(0xbcd);
    printf(0xbdf);
    fgets(&src, 0x400, *_reloc.stdin);
    strncpy(arg1, &src, (int64_t)*(int32_t *)(arg1 + 0xb4), &src);
    return;
}

void handle_msg(void)
{
    int64_t var_c0h;
    undefined8 uStack60;
    undefined8 uStack52;
    undefined8 uStack44;
    undefined8 uStack36;
    undefined8 uStack28;
    int64_t var_ch;
    
    uStack60 = 0;
    uStack52 = 0;
    uStack44 = 0;
    uStack36 = 0;
    uStack28 = 0;
    var_ch._0_4_ = 0x8c;
    set_username((char *)&var_c0h);
    set_msg((char *)&var_c0h);
    puts(0xbc0);
    return;
}

undefined8 main(void)
{
    puts(0xc10);
    handle_msg();
    return 0;
}
