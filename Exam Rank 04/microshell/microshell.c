#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

void puterr(char *str)
{
    while (*str)
    {
        write(2, str, 1);
        str++;
    }
}

int cd(char **argv, int i)
{
    if (i != 2)
        return (puterr("error: cd: bad arguments\n"), 1);
    if (chdir(argv[1]) == -1)
        return (puterr("error: cd: cannot change directory to "), puterr(argv[1]), puterr("\n"), 1);
    return (0);
}

void set_pipe(int has_pipe, int fd[], int end)
{
    if (has_pipe && (dup2(fd[end], end) == -1 || (close(fd[1]) == -1) || (close(fd[0]) == -1)))
        (puterr("error: fatal\n"), exit(1));
}

int exec(char **argv, int i, char **envp)
{
    int status;
    int fd[2];
    int has_pipe;
    int pid;

    has_pipe = argv[i] && !strcmp(argv[i], "|");
    if (!has_pipe && !strcmp(argv[0], "cd"))
        return (cd(argv, i));
    if (has_pipe && (pipe(fd) == -1))
        (puterr("error: fatal\n"), exit(1));
    if ((pid = fork()) == -1)
        (puterr("error: fatal\n"), exit(1));
    if (pid == 0)
    {
        argv[i] = NULL;
        set_pipe(has_pipe, fd, STDOUT_FILENO);
        if (!strcmp(argv[0], "cd"))
            exit(cd(argv, i));
        execve(argv[0], argv, envp);
        puterr("error: cannot execute "), puterr(argv[0]), puterr("\n"), exit(1);
    }
    waitpid(pid, &status, 0);
    set_pipe(has_pipe, fd, STDIN_FILENO);
    return (WIFEXITED(status) && WEXITSTATUS(status));
}

int main(int argc, char **argv, char **envp)
{
    (void)argc;
    int status;
    int i;

    status = 0;
    i = 0;
    while (argv[i])
    {
        argv += i + 1;
        i = 0;
        while (argv[i] && strcmp(argv[i], "|") && strcmp(argv[i], ";"))
            i++;
        if (i)
            status = exec(argv, i, envp);
    }
    return (status);
}
