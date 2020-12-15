int32_t verify_user_name(void)
{
    int32_t iVar1;
    code *pcVar2;
    code *pcVar3;
    undefined uVar4;
    undefined uVar5;
    uint8_t uVar6;
    
    uVar6 = 0;
    uVar4 = &stack0xfffffff4 < (undefined *)0x10;
    uVar5 = (undefined *)*(BADSPACEBASE **)0x10 == (undefined *)0x1c;
    puts();
    iVar1 = 7;
    pcVar2 = a_user_name;
    pcVar3 = (code *)"dat_wil";
    do {
        if (iVar1 == 0) break;
        iVar1 = iVar1 + -1;
        uVar4 = (uint8_t)*pcVar2 < (uint8_t)*pcVar3;
        uVar5 = *pcVar2 == *pcVar3;
        pcVar2 = pcVar2 + (uint32_t)uVar6 * -2 + 1;
        pcVar3 = pcVar3 + (uint32_t)uVar6 * -2 + 1;
    } while ((bool)uVar5);
    return (int32_t)(char)((!(bool)uVar4 && !(bool)uVar5) - uVar4);
}


int32_t verify_user_pass(char *arg_8h)
{
    int32_t iVar1;
    uint8_t *puVar2;
    undefined in_CF;
    undefined in_ZF;
    
    iVar1 = 5;
    puVar2 = (uint8_t *)"admin";
    do {
        if (iVar1 == 0) break;
        iVar1 = iVar1 + -1;
        in_CF = (uint8_t)*arg_8h < *puVar2;
        in_ZF = *arg_8h == *puVar2;
        arg_8h = (char *)((uint8_t *)arg_8h + 1);
        puVar2 = puVar2 + 1;
    } while ((bool)in_ZF);
    return (int32_t)(char)((!(bool)in_CF && !(bool)in_ZF) - in_CF);
}

undefined4 main(void)
{
    undefined4 uVar1;
    int32_t iVar2;
    undefined4 *puVar3;
    undefined4 auStack84 [16];
    int32_t iStack20;
    int32_t var_bp_8h;
    
    iVar2 = 0x10;
    puVar3 = auStack84;
    while (iVar2 != 0) {
        iVar2 = iVar2 + -1;
        *puVar3 = 0;
        puVar3 = puVar3 + 1;
    }
    iStack20 = 0;
    puts("********* ADMIN LOGIN PROMPT *********");
    printf("Enter Username: ");
    fgets(a_user_name, 0x100, _reloc.stdin);
    iStack20 = verify_user_name();
    if (iStack20 == 0) {
        puts("Enter Password: ");
        fgets(auStack84, 100, _reloc.stdin);
        iStack20 = verify_user_pass((char *)auStack84);
        if ((iStack20 == 0) || (iStack20 != 0)) {
            puts("nope, incorrect password...\n");
            uVar1 = 1;
        } else {
            uVar1 = 0;
        }
    } else {
        puts("nope, incorrect username...\n");
        uVar1 = 1;
    }
    return uVar1;
}