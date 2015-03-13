#include	<stdio.h>
#include	<sys/types.h> //和dirent相关
#include	<sys/stat.h>//调用stat()返回目录的inode号
#include	<dirent.h>//操作目录的相关函数opendir() closedir() reddir()
#include	<stdlib.h>
#include	<string.h>
ino_t get_inode(char *);
void printpath(ino_t);
void inum_to_name(ino_t,char *,int);

int main()
{

	printpath(get_inode("."));//打印路径,先输入当前目录的名字
	putchar('\n');//换行
	return 0;
}
//返回文件的inode
ino_t get_inode(char *fname)
{
	 struct stat info;
	 if( stat(fname,&info)==-1){//把fname 的信息复制到指针info所指的结构中
		fprintf(stderr,"Cannot stat");
		perror(fname);
		exit(1);
	}
	return info.st_ino;
}
void inum_to_name(ino_t inode_to_find,char *namebuf,int buflen)
{
	DIR		*dir_ptr;
	struct dirent *direntp;
	dir_ptr= opendir(".");//dir_ptr相当于一个标记指向这个目录
	if(dir_ptr== NULL){
		perror(".");
		exit(1);
	}
	while((direntp = readdir(dir_ptr))!=NULL)
		if(direntp->d_ino == inode_to_find)//找到了inode号找到
		{
			strncpy(namebuf,direntp->d_name,buflen);//把名字复制过去，非常安全的复制方式
			namebuf[buflen-1] = '\0';
			closedir(dir_ptr);
			return;
		}
	fprintf(stderr,"error looking for inum %d\n",(int)inode_to_find);
	exit(1);
}
void printpath(ino_t this_node)
{
	ino_t my_inode;//节点号
	char its_name[BUFSIZ];//放目录名字的数组
	if(get_inode("..")!=this_node)//判断是否到了根目录
	{
		chdir("..");//这个函数的作用是改变调用进程的当前目录，这里到了父目录
		inum_to_name(this_node,its_name,BUFSIZ);//节点号转化成名字
		my_inode = get_inode(".");
        printpath(my_inode);//递归回溯
		printf("/%s",its_name);
    }
}
