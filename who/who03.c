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
#include<sys/types.h>

#define NRECS 16
#define NULLUT ((struct utmp *)NULL)
#define UTSIZE (sizeof(struct utmp))
static char utmpbuf[NRECS*UTSIZE];
static int fd_utmp = -1;  //read from
static cur_rec;//记录缓冲区中已被使用的数据个数
static num_recs; //记录缓冲区中的数据个数


int utmp_open(char *filename)
{

   fd_utmp = open(filename, O_RDONLY);
   cur_rec = num_recs = 0;      
   return fd_utmp;
}
// read bunches of records into buffer
int utmp_reload()
{
  int amt_read;
  amt_read = read(fd_utmp,utmpbuf,NRECS * UTSIZE);
  num_recs = amt_read/UTSIZE;
  cur_rec = 0;
     return num_recs;
}
//从缓冲区中取得下一个结构的数据
struct utmp * utmp_next()
{
    struct utmp * recp;
    if(fd_utmp == -1)
      return NULLUT;
    if(cur_rec ==num_recs && utmp_reload()==0) 
      return NULLUT;
    recp = (struct utmp *)&utmpbuf[cur_rec*UTSIZE];
    cur_rec++;
    return recp;
}
void utmp_close()
{
 if(fd_utmp != -1)
 close(fd_utmp);
}


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
  struct utmp *utbufp,*utmp_next(); //read info from here
 
  
  if(utmp_open(UTMP_FILE)== -1){

        perror(UTMP_FILE); //UTMP_FILE is in utmp.h
        exit(1);
  }
  while((utbufp = utmp_next()) != ((struct utmp *)NULL))
        show_info(utbufp);
        utmp_close();
  return 0;
}



