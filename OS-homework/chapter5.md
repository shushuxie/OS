#### 1．编写一个调用 fork()的程序。谁调用 fork()之前，让主进程访问一个变量（例如 x） 并将其值设置为某个值（例如 100）。子进程中的变量有什么值？当子进程和父进程都改变 x 的值谁，变量会发生什么？

分析：父进程和子进程分别独立使用x变量；子进程接收到的x值是父进程在fork之前的最后结果，子进程后面改变x的值不会影响父进程的结果；

```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h> 

int main(){
	int x = 0;
	printf(" parent init x = %d \n",x);
	x = 100;
	printf(" parent before fork set value x = %d \n",x);
	int result = fork();
		if (result == 0){
		printf("child befor set x = %d\n",x);
		x = 200;
		printf("child set x = %d \n",x);
	} else {
		int wc = wait(NULL);
		printf("parent after x = %d \n",x);
}

	return 0;

}

// 输出结果
parent init x = 0 
parent before fork set value x = 100 
child befor set x = 100
child set x = 200 
parent after x = 100  //即使等待子进程结束，父进程仍然不受影响
```

#### 2．编写一个打开文件的程序（使用 open()系统调用），然后调用 fork()创建一个新进程。 子进程和父进程都可以访问 open()返回的文件描述符吗？当它并发（即同时）写入文件时， 会发生什么？

分析：出乎意料，父进程和子进程都可以访问文件；同时写入会发生争夺；

```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h> 
#include <sys/types.h>
#include <fcntl.h>

int main(){

	int f = open("./file.txt",O_WRONLY|O_APPEND);
    printf("file description = %d\n",f);

    int fk = fork();
    if (fk < 0) {
        fprintf(stderr,"child process create fail");
    } else if (fk == 0) {
        printf("child get file handle f=%d\n",f);

        int a = write(f,"child write",12);
        if (a > 0) {
            printf("child write success ");
        } else {
            printf("child write fail");
        }
    } else {
        int w = write(f,"parent write",12);
        printf("file write return = %d\n",w);
    }
    close(f);

	return 0;
}
// 结果是两个进程都写入了数据，
// 原因应该是文件描述符独立于线程，只要文件打开谁拿到文件描述符都可以操作文件；

```





#### 3．使用 fork()编写另一个程序。子进程应打印“hello”，父进程应打印“goodbye”。你 应该尝试子进程始终先打印。你能否不在父进程调用 wait()而做到这一点呢？

分析：这样估计应该要是线程睡眠才行

```c
#include <stdio.h>
#include <unistd.h>
#include<sys/types.h>
#include <fcntl.h>
int main() {
	int i = 0;
        int rc = fork();
        if (rc < 0) {
            fprintf(stderr,"child create fail");
        } else if(rc == 0){
            printf("hello");
        } else {
	    sleep(1);
            printf("goodbye;\n");
        }

    return 0;
}


```





#### 4．编写一个调用 fork()的程序，然后调用某种形式的 exec()来运行程序/bin/ls。看看是 否可以尝试 exec()的所有变体，包括 execl()、execle()、execlp()、execv()、execvp()和 execvP()。为什么同样的基本调用会有这谁多变种？



#### 5．现谁编写一个程序，谁父进程中使用 wait()，等待子进程完成。wait()返回谁谁？如 果你谁子进程中使用 wait()会发生谁谁？



####  6．对前一个程序稍作修改，这次使用 waitpid()而我是 wait()。谁谁谁谁 waitpid()会 有用？



####  7．编写一个创建子进程的程序，然后谁子进程中关闭标准输出（STDOUT_FILENO）。 如果子进程谁关闭描述符后调用 printf()打印输出，会发生谁谁？



####  8．编写一个程序，创建两个子进程，并使用 pipe()系统调用，将一个子进程的标准输 出连接到另一个子进程的标准输入。