typedef struct s_message
{
    char text[140];
    char username[40];
    int len;
} t_message;

void secret_backdoor() {
    char buffer[128];

    fgets(buffer, 128, stdin);
    system(buffer);
}

void set_msg(t_message *message) {
    char local_buffer[0x400];

    memset(local_buffer, '\0', 0x400);

    puts(">: Msg @Unix-Dude");
    printf(">>: ");
    fgets(local_buffer, 0x400, stdin);
    strncpy(message->text, local_buffer, message->len);
}

void set_username(t_message *message) {
    int32_t i;
    char local_buffer[0x80];
    
    memset(local_buffer, 0, 0x80);

    puts(">: Enter your username");
    printf(">>: ");
    fgets(local_buffer, 0x80, stdin);

    *(int32_t *)(local_buffer + 140) = 0;

    for (i = 0; i <= 0x28 && local_buffer[i]; i++) {
        message->username[i] = local_buffer[i];
    }

    printf(">: Welcome, %s", message->username);
}

void handle_msg() {
    t_message message;

    memset(message.username, 0, 0x28);
    message.len = 0x8c;

    set_username(&message);
    set_msg(&message);

    puts(">: Msg sent!");
}

int main() {
    puts("--------------------------------------------\n|   ~Welcome to l33t-m$n ~    v1337        |\n--------------------------------------------");
    handle_msg();
    return 0;
}
