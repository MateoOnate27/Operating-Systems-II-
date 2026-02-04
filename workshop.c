#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main() {
    int fd;
    char buffer[128];
    int bytes;
    
    // System Call: open
    fd = open("sample.txt", O_RDONLY);
    if (fd == -1) return 1;

    // System Call: read
    bytes = read(fd, buffer, sizeof(buffer));

    // System Call: write (1 = stdout)
    write(1, buffer, bytes);

    // System Call: close
    close(fd);
    return 0;
}
