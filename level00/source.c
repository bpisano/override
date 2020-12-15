bool main(void)
{
    int32_t aiStack20 [4];
    
    puts("***********************************");
    puts("* \t     -Level00 -\t\t  *");
    puts("***********************************");
    printf("Password:");
    __isoc99_scanf(0x8048636, aiStack20);
    if (aiStack20[0] != 0x149c) {
        puts("\nInvalid Password!");
    } else {
        puts("\nAuthenticated!");
        system("/bin/sh");
    }
    return aiStack20[0] != 0x149c;
}