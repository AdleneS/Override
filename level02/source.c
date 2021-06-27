int main(int argc, char*argv[]) {
    int ac = argc;
    char **av = argv;

    int8_t *username; 
    int8_t *pass;
    int8_t *pwd;
    size_t read_ret = 0;
    FILE *pwd_file = 0;
  
    memset(username, 0, 12);
    memset(pass, 0, 5);
    memset(pwd, 0, 12);

    pwd_file = fopen("/home/users/level03/.pass", 'r');
    if (pwd_file == 0) {
        fwrite("ERROR: failed to open password file\n", 1, 0x24, stderr);
        exit(1);
    }

    read_ret = fread(pass, 1, 0x29, pwd_file);
    pass[strcspn(pass, "\n")] = 0;
    if (read_ret == 0x29) {
        fwrite("ERROR: failed to read password file\n", 1, 0x24, stderr);
        exit(1);
    }

    fclose(pwd_file);

    puts("===== [ Secure Access System v1.0 ] =====");
    puts("/***************************************\\");
    puts("| You must login to access this system. |");
    puts("\\**************************************/");
    printf("--[ Username: ");
    fgets(username, 0x64, stdin);
    username[strcspn(username, "\n")] = 0;
    printf("--[ Password: ");
    fgets(pwd, 0x64, stdin);
    pwd[strcspn(pwd, "\n")] = 0;
    puts("*****************************************");

    if (strncmp(pass, pwd, 0x29)) { // 41
        printf(username);
        puts(" does not have access!");
        exit(1);
    }

    printf("Greetings, %s!\n", username);
    system("/bin/sh");
    
    return 0;
}
