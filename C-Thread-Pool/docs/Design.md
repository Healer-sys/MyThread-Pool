## High level

	Description: Library providing a threading pool where you can add work on the fly. The number
	             of threads in the pool is adjustable when creating the pool. In most cases
	             this should equal the number of threads supported by your cpu.

	             For an example on how to use the threadpool, check the main.c file or just read
	             the documentation found in the README.md file.

	             In this header file a detailed overview of the functions and the threadpool's logical
	             scheme is presented in case you wish to tweak or alter something.

			描述：本库提供了一个线程池，可以在运行时添加工作。
				 创建线程池时可以调整线程池中的线程数量。
				 在大多数情况下，这个数量应该等于你的 CPU 支持的线程数。

				 要了解如何使用线程池，请查看 `main.c` 文件或阅读 `README.md` 文件中的文档。

				 在这个头文件中，提供了函数的详细概述以及线程池的逻辑方案，以便你想要进行调整或修改时参考。


	             _______________________________________________________
	            /                                                       \
	            |   JOB QUEUE       | job1 | job2 | job3 | job4 | ..    |
	            |                                                       |
	            |   threadpool      | thread1 | thread2 | ..            |
	            \_______________________________________________________/


	   Description:       Jobs are added to the job queue. Once a thread in the pool
	                      is idle, it is assigned the first job from the queue (and that job is
	                      erased from the queue). It is each thread's job to read from
	                      the queue serially (using lock) and executing each job
	                      until the queue is empty.

		  描述：			任务被添加到任务队列中。一旦线程池中的某个线程空闲，
							它就会被分配队列中的第一个任务（并将该任务从队列中移除）。
							每个线程的任务是串行地从队列中读取（使用锁）并执行每个任务，
							直到队列为空。

	   Scheme:

	   thpool______                jobqueue____                      ______
	   |           |               |           |       .----------->|_job0_| Newly added job
	   |           |               |  rear  ----------'             |_job1_|
	   | jobqueue----------------->|           |                    |_job2_|
	   |           |               |  front ----------.             |__..__|
	   |___________|               |___________|       '----------->|_jobn_| Job for thread to take


	   job0________
	   |           |
	   | function---->
	   |           |
	   |   arg------->
	   |           |         job1________
	   |  next-------------->|           |
	   |___________|         |           |..
