/* who03.c - read /etc/utmp and list info therein
 * supperess empty records
 * formats time nicely
 */
#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>
#include<stdlib.h>
#include<utmp.h>
#include<time.h>
/*display time in a format fit for human consuption
 *uses ctime to build a string then picks parts out of it 
 *Note: %12.12s prints a string 12 chars wide and LIMITS it to 12 chars
 */
void showtime(long timeval){
   char *cp;
   cp = ctime(&timeval);
   printf("%12.12s",cp+4); //picks 12 chars from pos 4

}
/* show_info 
 * display contents of utmp struct in human readable form
 ** note these sizes should not be hardwired
 */
void show_info(struct utmp *utbufp){
    if(utbufp->ut_type != USER_PROCESS)
       return;
    printf("%-8.8s",utbufp->ut_name); //the logname
    printf(" ");
    printf("%-8.8s",utbufp->ut_line); //the tty
    printf(" ");
    showtime(utbufp->ut_time);
 //   printf("%10ld",utbufp->ut_time); //login time
    printf(" ");
    #ifdef SHOWHOST
    printf("(%s)",utbufp->ut_host); //the host
    #endif
    printf("\n");
}

int main()
{
  struct utmp current_record ; //read info from here
  int utmpfd; //read from this descriptor
  int reclen = sizeof(current_record);
  if(( utmpfd = open(UTMP_FILE,O_RDONLY)) == -1){

        perror(UTMP_FILE); //UTMP_FILE is in utmp.h
        exit(1);
  }
  while(read(utmpfd,&current_record,reclen) == reclen)
        show_info(&current_record);
  close(utmpfd);
  return 0;
}



