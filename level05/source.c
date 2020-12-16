void main(void)
{
    uint8_t uVar1;
    uint32_t uVar2;
    uint32_t uVar3;
    uint8_t *puVar4;
    uint8_t uVar5;
    uint8_t auStack120 [100];
    uint32_t uStack20;
    
    uVar5 = 0;
    uStack20 = 0;
    fgets(auStack120, 100, _reloc.stdin);
    uStack20 = 0;
    do {
        uVar2 = uStack20;
        uVar3 = 0xffffffff;
        puVar4 = auStack120;
        do {
            if (uVar3 == 0) break;
            uVar3 = uVar3 - 1;
            uVar1 = *puVar4;
            puVar4 = puVar4 + (uint32_t)uVar5 * -2 + 1;
        } while (uVar1 != 0);
        if (~uVar3 - 1 <= uStack20) {
            printf(auStack120);
            exit(0);
            __i686.get_pc_thunk.bx(uVar2);
            .init();
            return;
        }
        if (('@' < (char)auStack120[uStack20]) && ((char)auStack120[uStack20] < '[')) {
            auStack120[uStack20] = auStack120[uStack20] ^ 0x20;
        }
        uStack20 = uStack20 + 1;
    } while( true );
}
