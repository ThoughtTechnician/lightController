#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <sys/stat.h>
#include <linux/kd.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#define ERROR -1
int fd; /* File descriptor for console (/dev/tty/) */
int sighandler(int signum);
int main()
{
  int i;
          /* To be used as the fd in ioctl(). */
  if ((fd = open("/dev/console", O_NOCTTY)) == ERROR) {
     perror("open");
     return (ERROR);
  }
	  signal(SIGINT,  sighandler);
  signal(SIGTERM, sighandler);
  signal(SIGQUIT, sighandler);
  signal(SIGTSTP, sighandler);
	  printf("w00w00!\n\n");
  printf("To exit hit Control-C.\n");
	  while (1) {
     for (i = 0x01; i <= 0x04; i++) {
         /* We do this because there is no LED for 0x03. */
       	 if (i == 0x03) continue; 
	         usleep(50000);
	         if ((ioctl(fd, KDSETLED, i)) == ERROR) {
            perror("ioctl");
            close(fd);
            return (ERROR);
         }
     }
  }
	  close(fd);
}
int sighandler(int signum)
{
  /* Turn off all leds. No LED == 0x0. */
  if ((ioctl(fd, KDSETLED, 0x0)) == ERROR) { 
    perror("ioctl");
    close(fd);
    return (ERROR);
  }
	  printf("\nw00w00!\n");
  close(fd);
  return (0);
}
