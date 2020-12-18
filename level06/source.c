undefined4 auth(char *s1, int32_t arg_ch)
{
    int32_t iVar1;
    undefined4 uVar2;
    int32_t iVar3;
    int32_t var_14h;
    uint32_t var_10h;
    undefined4 var_ch;
    
    iVar1 = strcspn();
    s1[iVar1] = '\0';
    iVar1 = strnlen();
    if (iVar1 < 6) {
        uVar2 = 1;
    } else {
        iVar3 = ptrace();
        if (iVar3 == -1) {
            puts();
            puts();
            puts();
            uVar2 = 1;
        } else {
            var_10h = ((int32_t)s1[3] ^ 0x1337U) + 0x5eeded;
            var_14h = 0;
            while (var_14h < iVar1) {
                if (s1[var_14h] < ' ') {
                    return 1;
                }
                var_10h = var_10h + ((int32_t)s1[var_14h] ^ var_10h) % 0x539;
                var_14h = var_14h + 1;
            }
            if (arg_ch == var_10h) {
                uVar2 = 0;
            } else {
                uVar2 = 1;
            }
        }
    }
    return uVar2;
}


uint32_t main(undefined4 placeholder_0, char **envp)
{
    int32_t iVar1;
    uint32_t uVar2;
    int32_t in_GS_OFFSET;
    int32_t iStack56;
    char acStack52 [32];
    int32_t iStack20;
    
    iStack20 = *(int32_t *)(in_GS_OFFSET + 0x14);
    puts();
    puts();
    puts();
    printf();
    fgets();
    puts();
    puts();
    puts();
    printf();
    __isoc99_scanf();
    iVar1 = auth(acStack52, iStack56);
    if (iVar1 == 0) {
        puts();
        system();
    }
    uVar2 = (uint32_t)(iVar1 != 0);
    if (iStack20 != *(int32_t *)(in_GS_OFFSET + 0x14)) {
        uVar2 = __stack_chk_fail();
    }
    return uVar2;
}
