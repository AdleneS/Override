int auth(char *login_str, unsigned int serial)
{
	size_t login_len;

	login_str[strcspn(login_str, "\n")] = 0;
	login_len = strnlen(login_str, 32);
	
	if (login_len <= 5) {
		return 1;
	}
	if (ptrace(0, 0, 1, 0) == -1) {
		puts("\e[32m.---------------------------.");
		puts("\e[31m| !! TAMPERING DETECTED !!  |");
		puts("\e[32m.---------------------------.");
		return 1;
	}
	
	int hash;
	hash = ((int) (login_str[3])) ^ 0x1337 + 0x5eeded; 

	for (int i = 0; i < login_len; i++) {
		if (login_str[i] <= 31)
			return 1;
		int tmp1 = login_str[i] ^ hash;
        int tmp2 = 0x88233b2b * tmp1;
		int tmp3 = (tmp1 - tmp2) / 2;
        int tmp4 = (tmp3 + tmp2) / 1024 * 0x539;
        hash += tmp1 - tmp4;
	}
	
	if (hash == serial)
		return 0;
	else
		return 1;
}

int main()
{
	int8_t login_buffer[0x20];
	uint32_t serial;

	puts("***********************************");
	puts("*\t\tlevel06\t\t  *");
	puts("***********************************");
	printf("-> Enter Login: ");
	fgets(login_buffer, 0x20, stdin);

	puts("***********************************");
	puts("***** NEW ACCOUNT DETECTED ********");
	puts("***********************************");
	printf("-> Enter Serial: ");
	scanf("%u", &serial);

	if (!auth(login_buffer, serial)) {
		puts("Authenticated!");
		system("/bin/sh");
		return 0;
	}

	return 1;
}
