#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>

int main(int argc, char **argv)
{
    int fd = open("/dev/mchardev", 0);
    // do checks

    int ret = ioctl(fd, 127, 0);
    printf("ioctl returned %d\n", ret);

    close(fd);

    return EXIT_SUCCESS;
}