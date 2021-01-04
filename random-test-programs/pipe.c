#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>

struct point
{
	int x,y;
};

int main()
{
	int fd[2];

	if(pipe(fd)==-1)
		perror("pipe");

	pid_t pid = fork();
	if(pid==-1)
		perror("fork");
	else if(pid>0) //parent
	{
		struct point *a=(struct point*)malloc(sizeof(struct point));
		close(fd[0]);
		a->x=10;
		a->y=10;
		write(fd[1],a,sizeof(struct point));
		a->y=10;
		a->y=8;
		write(fd[1],a,sizeof(struct point));
	}
	else // child
	{
		struct point *a=(struct point*)malloc(sizeof(struct point));
		close(fd[1]);
		read(fd[0],a,sizeof(struct point));
		printf("%d %d\n",a->x,a->y);
		read(fd[0],a,sizeof(struct point));
		printf("%d %d\n",a->x,a->y);
	}
}