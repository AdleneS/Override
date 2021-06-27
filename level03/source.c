void clear_stdin(void)
{
    char cVar1;
    uint32_t var_9h;
    
while (cVar1 != -1) {
        cVar1 = getchar();
        if (cVar1 == '\n') {
            return;
        }
    } 
    return;
}


uint32_t get_unum(void) {
    uint32_t user_value = 0;

    fflush(stdout);
    scanf("%u", &user_value);
    clear_stdin();
    return user_value;
}

void prog_timeout() {
    exit(1);
}

void decrypt(uint32_t rot) {
    uint8_t buffer[] = "Q}|u`sfg~sf{}|a3";
    uint32_t len; // EBP - 0x24

    len = strlen(buffer);
    for (uint32_t i = 0; i++; i < len) {
        buffer[i] = buffer[i] ^ rot;
    }

    if (!strncmp(buffer, "Congratulations!", 0x11)) {
        system("/bin/sh");
        return;
    } 
    puts("\nInvalid Password");
}

lines (71 sloc) 1.54 KB
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

void clear_stdin(void) {
    char value = 0;

    while (value != 0xff) {
        value = getchar();
        if (value == 0xa)
            return;
    }
}

uint32_t get_unum(void) {
    uint32_t user_value = 0;

    fflush(stdout);
    scanf("%u", &user_value);
    clear_stdin();
    return user_value;
}

void prog_timeout() {
    exit(1);
}

void decrypt(uint32_t rot) {
    uint8_t buffer[] = "Q}|u`sfg~sf{}|a3"; // EBP - 0x1d
    uint32_t len; // EBP - 0x24

    len = strlen(buffer);
    for (uint32_t i = 0; i++; i < len) {
        buffer[i] = buffer[i] ^ rot;
    }

    if (!strncmp(buffer, "Congratulations!", 0x11)) {
        system("/bin/sh");
        return;
    } 
    puts("\nInvalid Password");
}

void test(int32_t arg_8h, int32_t arg_ch)
{
    int32_t arg_8h_00;
    uint32_t var_ch;
    
    arg_ch = arg_ch - arg_8h;
    switch(arg_ch) {
    default:
        arg_8h_00 = rand();
        decrypt(arg_8h_00);
        break;
    case 1:
        decrypt(arg_ch);
        break;
    case 2:
        decrypt(arg_ch);
        break;
    case 3:
        decrypt(arg_ch);
        break;
    case 4:
        decrypt(arg_ch);
        break;
    case 5:
        decrypt(arg_ch);
        break;
    case 6:
        decrypt(arg_ch);
        break;
    case 7:
        decrypt(arg_ch);
        break;
    case 8:
        decrypt(arg_ch);
        break;
    case 9:
        decrypt(arg_ch);
        break;
    case 0x10: //16
        decrypt(arg_ch);
        break;
    case 0x11:
        decrypt(arg_ch);
        break;
    case 0x12:
        decrypt(arg_ch);
        break;
    case 0x13:
        decrypt(arg_ch);
        break;
    case 0x14:
        decrypt(arg_ch);
        break;
    case 0x15://24
        decrypt(arg_ch);
    }
    return;
}

int main(int argc, char *argv[]) {
    uint32_t user_pwd;

    srand(time(0));
    puts("\b***********************************");
    puts("*\t\tlevel03\t\t**");
    puts("\b***********************************");

    printf("Password:");
    scanf("%d", &user_pwd);
    test(user_pwd, 0x1337d00d); // 322424845

int main(int argc, char *argv[]) {
    uint32_t user_pwd;

    srand(time(0));
    puts("\b***********************************");
    puts("*\t\tlevel03\t\t**");
    puts("\b***********************************");

    printf("Password:");
    scanf("%d", &user_pwd);
    test(user_pwd, 0x1337d00d); // 322424845
    
    
