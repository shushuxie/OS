### 进程课后练习



### 常用模拟器命令

```shell
Options:
  -h, --help            show this help message and exit
  -s SEED, --seed=SEED  the random seed
  -P PROGRAM, --program=PROGRAM
                        more specific controls over programs
  -l PROCESS_LIST, --processlist=PROCESS_LIST
                        a comma-separated list of processes to run, in the
                        form X1:Y1,X2:Y2,... where X is the number of
                        instructions that process should run, and Y the
                        chances (from 0 to 100) that an instruction will use
                        the CPU or issue an IO (i.e., if Y is 100, a process
                        will ONLY use the CPU and issue no I/Os; if Y is 0, a
                        process will only issue I/Os)
  -L IO_LENGTH, --iolength=IO_LENGTH
                        how long an IO takes //每个IO花费的时间，默认7（1，5，1）
  -S PROCESS_SWITCH_BEHAVIOR, --switch=PROCESS_SWITCH_BEHAVIOR
                        when to switch between processes: 
                        SWITCH_ON_IO, io过程切换
                        SWITCH_ON_END io结束切换
  -I IO_DONE_BEHAVIOR, --iodone=IO_DONE_BEHAVIOR
                        type of behavior when IO ends: 
                        IO_RUN_LATER,  io结束后等待一会
                        IO_RUN_IMMEDIATE io结束后立即切换
  -c                    compute answers for me
  -p, --printstats      print statistics at end; only useful with -c flag
                        (otherwise stats are not printed)
```



#### 1. 用以下标志运行程序：./process-run.py -l 5:100,5:100。CPU 利用率（CPU 使用时间 的百分比）应该是多少？为什么你知道这一点？利用 -c 标记查看你的答案是否正确

​	根据命令 -l 5:100 指代含义有5条命令，占用100%cpu，因为没有进行I/O

```shell
  python3 ./process-run.py -l 5:100,5:100 -c -p
Time        PID: 0        PID: 1           CPU           IOs
  1        RUN:cpu         READY             1
  2        RUN:cpu         READY             1
  3        RUN:cpu         READY             1
  4        RUN:cpu         READY             1
  5        RUN:cpu         READY             1
  6           DONE       RUN:cpu             1
  7           DONE       RUN:cpu             1
  8           DONE       RUN:cpu             1
  9           DONE       RUN:cpu             1
 10           DONE       RUN:cpu             1

Stats: Total Time 10
Stats: CPU Busy 10 (100.00%)
Stats: IO Busy  0 (0.00%)
```





#### 2．现在用这些标志运行：./process-run.py -l 4:100,1:0。这些标志指定了一个包含 4 条指 令的进程（都要使用 CPU），并且只是简单地发出 I/O 并等待它完成。完成这两个进程需要 多长时间？利用-c 检查你的答案是否正确。

分析：一次I/O是默认阻塞5个时间片，同时I/O开始，结束还要各 占用一个时间片告诉cpu工作结束



```shell
   python3 ./process-run.py -l 4:100,1:0 -c -p
Time        PID: 0        PID: 1           CPU           IOs
  1        RUN:cpu         READY             1
  2        RUN:cpu         READY             1
  3        RUN:cpu         READY             1
  4        RUN:cpu         READY             1
  5           DONE        RUN:io             1
  6           DONE       BLOCKED                           1
  7           DONE       BLOCKED                           1
  8           DONE       BLOCKED                           1
  9           DONE       BLOCKED                           1
 10           DONE       BLOCKED                           1
 11*          DONE   RUN:io_done             1

Stats: Total Time 11
Stats: CPU Busy 6 (54.55%)
Stats: IO Busy  5 (45.45%)
```

#### 3．现在交换进程的顺序：./process-run.py -l 1:0,4:100。现在发生了什么？交换顺序是否 重要？为什么？同样，用-c 看看你的答案是否正确。

分析：占用时间应该会减少，run：io操作后 有5个时间片的阻塞时间，这时可以进行进程2的执行，因此总占用时间7个时间片；



```python
  python3 ./process-run.py -l 1:0,4:100 -c -p
Time        PID: 0        PID: 1           CPU           IOs
  1         RUN:io         READY             1
  2        BLOCKED       RUN:cpu             1             1
  3        BLOCKED       RUN:cpu             1             1
  4        BLOCKED       RUN:cpu             1             1
  5        BLOCKED       RUN:cpu             1             1
  6        BLOCKED          DONE                           1
  7*   RUN:io_done          DONE             1

Stats: Total Time 7
Stats: CPU Busy 6 (85.71%)
Stats: IO Busy  5 (71.43%)
```



#### 4．现在探索另一些标志。一个重要的标志是-S，它决定了当进程发出 I/O 时系统如何 反应。将标志设置为 SWITCH_ON_END，在进程进行 I/O 操作时，系统将不会切换到另一 个进程，而是等待进程完成。当你运行以下两个进程时，会发生什么情况？一个执行 I/O， 另一个执行 CPU 工作。（-l 1:0,4:100 -c -S SWITCH_ON_END）

分析：和2中的答案一致，io进行时可以运行的程序仍在等待；

```
  python3 ./process-run.py -l 1:0,4:100 -c -S SWITCH_ON_END -p
Time        PID: 0        PID: 1           CPU           IOs
  1         RUN:io         READY             1
  2        BLOCKED         READY                           1
  3        BLOCKED         READY                           1
  4        BLOCKED         READY                           1
  5        BLOCKED         READY                           1
  6        BLOCKED         READY                           1
  7*   RUN:io_done         READY             1
  8           DONE       RUN:cpu             1
  9           DONE       RUN:cpu             1
 10           DONE       RUN:cpu             1
 11           DONE       RUN:cpu             1

Stats: Total Time 11
Stats: CPU Busy 6 (54.55%)
Stats: IO Busy  5 (45.45%)
```

