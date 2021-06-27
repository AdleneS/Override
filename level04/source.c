int main()
{
    char buffer[32];
    pid_t waitedPid = 0;
    pid_t backupPid;
    pid_t thirdPid;
    int var2 = 0;

    pid_t pid = fork();

    memset(buffer, 0, 0x20);
    
    if (pid == 0) {
        prctl(1, 1);
        ptrace(0, 0, 0, 0);
        puts("Give me some shellcode, k");
        gets(buffer);
        return 0;
    }

    while (1) {
        wait(&waitedPid);
        backupPid = waitedPid;
        thirdPid = backupPid;
        if (WIFEXITED(thirdPid)) {
            puts("child is exiting");
            return 0;
        }

        if (ptrace(3, ret, 0x2c, 0) == 0xb) { // If syscall exec
            puts("no exec() for you");
            kill(ret, 9);
        }
    }

    return 0;
}
