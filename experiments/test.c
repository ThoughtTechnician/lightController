/*
 *Standard C userspace application to test turning the keyboard lights
 *on and off.
 */
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <sys/stat.h>
#include <linux/kd.h>
#include <sys/types.h>
#include <sys/ioctl.h>


#define ERROR -1
#define SUCCESS 0

int main(int argc, char *argv[])
{

int fd; /* File descriptor for console (/dev/tty/) */
      /* To be used as the fd in ioctl(). */
  if ((fd = open("/dev/console", O_RDWR)) == ERROR) {
     perror("open");
     return (ERROR);
  }
/*  for (int i = 0; i < 6; i++) {
	 if ((ioctl(fd, KDSETLED, i)) == ERROR) {
            perror("ioctl");
            //close(fd);
            return (ERROR);
         }
  }*/
	 if ((ioctl(fd, KDSETLED, 4)) == ERROR) {
            perror("ioctl");
            //close(fd);
            return (ERROR);
         }

	  int beep = (125 << 16) + 1591; // This is the default beep. 
  long int arg = 0;
	 if ((ioctl(fd, KDGETLED, &arg)) == ERROR) {
            perror("ioctl");
            close(fd);
            return (ERROR);
         }
/*	for (int i = 0; i <= 25; i++) { 
              if ((ioctl(fd, KDMKTONE, beep)) == ERROR) {
  	         perror("ioctl");
	         close(fd);
	         return (ERROR);
	      }

              beep += 50;
              usleep(150000);
	  }*/
printf("KDGETLED: %d\n", arg);
char* test = "test";
printf("here: %s\n", test);
return SUCCESS;
}
