#pragma once
#include "MultiplicationHelper.h"
#define HAVE_STRUCT_TIMESPEC
#include "include/pthread.h"
#include "include/sched.h"
#include "Matrix.h"

// thread lists
std::vector<pthread_t> m_threads0;
std::vector<pthread_t> m_threads1;
std::vector<pthread_t> m_threads2;

//no threading loop
void *Matrix_Multiplication_noThread(void *arguments)
{
	struct threadArgs *args = (struct threadArgs *)arguments;

	for (USHORT i = 0; i < args->m_matrix0->getRowCount(); ++i) {
		for (USHORT j = 0; j < args->m_matrix1->getColCount(); ++j) {
			for (USHORT k = 0; k < args->m_matrix0->getColCount(); ++k)
			{
				args->m_matrix_final->getValue()[i][j] += args->m_matrix0->getValue()[i][k] * args->m_matrix1->getValue()[k][j];
			}
		}
	}
	return arguments;
}

//Thread first loop of multiplication
void *Matrix_Multiplication_stage_0(void *arguments)
{
	struct threadArgs *args = (struct threadArgs *)arguments;
	
	for (USHORT j = 0; j < args->m_matrix1->getColCount(); ++j) {
		for (USHORT k = 0; k < args->m_matrix0->getColCount(); ++k)
		{
			args->m_matrix_final->getValue()[args->i][j] += args->m_matrix0->getValue()[args->i][k] * args->m_matrix1->getValue()[k][j];
		}
	}
	pthread_exit(arguments);
	return args;
}

//Thread second loop of multiplication
void *Matrix_Multiplication_stage_1(void *arguments)
{
	struct threadArgs *args = (struct threadArgs *)arguments;

	for (USHORT k = 0; k < args->m_matrix0->getColCount(); ++k)
	{
		args->m_matrix_final->getValue()[args->i][args->j] += args->m_matrix0->getValue()[args->i][k] * args->m_matrix1->getValue()[k][args->j];
	}
	pthread_exit(arguments);
	return args;
}

//Thread last loop of multiplication
void *Matrix_Multiplication_stage_2(void *arguments)
{
	struct threadArgs *args = (struct threadArgs *)arguments;

	args->m_matrix_final->getValue()[args->i][args->j] += args->m_matrix0->getValue()[args->i][args->k] * args->m_matrix1->getValue()[args->k][args->j];

	pthread_exit(arguments);
	return args;
}

Helper::Helper(std::shared_ptr<Matrix> m0, std::shared_ptr<Matrix> m1, std::shared_ptr<Matrix> mf)
{
	m_matrix0 = m0;
	m_matrix1 = m1;
	m_matrix_final = mf;
}


void Helper::multiplyMatrix()
{
	// set timer
	printf("<<Starting no thread mutliplication>>\n");
	masterDelta.addStartTime(TIME_MEASURMENT::Milli, "no thread: ");
	pthread_t thread; // create own single thread for whole multiplication

	// data to pass into single thread
	std::shared_ptr<threadArgs> args = std::make_shared<threadArgs>();
	args->m_matrix0 = m_matrix0;
	args->m_matrix1 = m_matrix1;
	args->m_matrix_final = m_matrix_final;

	if (pthread_create(&thread, NULL, &Matrix_Multiplication_noThread, (void *)args.get()))
		printf("Uh-oh!\n");

	printf("Starting 1 thread Join...\n");
	pthread_join(thread, NULL);
	masterDelta.addFinishTime("no thread: ");
	masterDelta.printFinalTimeSheet();
}

void Helper::multiplyMatrix_thread0()
{
	// set thread creation counter
	unsigned long threadCount = 0;
	std::vector<std::shared_ptr<threadArgs>> prtList;
	printf("<<Starting first loop thread mutliplication>>\n");
	masterDelta.addStartTime(TIME_MEASURMENT::Milli, "thread in first loop");
	for (USHORT i = 0; i < m_matrix0->getRowCount(); ++i)
	{
		// create thread for loop
		pthread_t thread;
		// increment thread counter
		threadCount++;
		// create thread specific data
		std::shared_ptr<threadArgs> args = std::make_shared<threadArgs>();
		args->i = i;
		args->m_matrix0 = m_matrix0;
		args->m_matrix1 = m_matrix1;
		args->m_matrix_final = m_matrix_final;
		// place data in list so it isnt removed when scope run out
		prtList.push_back(args);

		if (pthread_create(&thread, NULL, &Matrix_Multiplication_stage_0, (void *)args.get()) != 0) {
			printf("Uh-oh!\n");
		}
		m_threads0.push_back(thread);
	}
	
	// wait for threads to finish
	printf("Starting first loop thread Join with %d threads...\n", threadCount);
	for (auto obj : m_threads0) {
		pthread_join(obj, NULL);
	}

	masterDelta.addFinishTime("thread in first loop");
	masterDelta.printFinalTimeSheet();

}

void Helper::multiplyMatrix_thread1()
{
	//set thread counter
	unsigned long threadCount = 0;
	// create list of threads 
	std::vector<std::shared_ptr<threadArgs>> prtList;
	printf("<<Starting second loop thread mutliplication>>\n");
	masterDelta.addStartTime(TIME_MEASURMENT::Milli, "thread in second loop");
	for (USHORT i = 0; i < m_matrix0->getRowCount(); ++i)
	{
		for (USHORT j = 0; j < m_matrix1->getColCount(); ++j) {
			// create thread
			pthread_t thread;
			// increment thread count counter
			threadCount++;
			// create data needed in this thread
			std::shared_ptr<threadArgs> args = std::make_shared<threadArgs>();
			args->j = i;
			args->i = j;
			args->m_matrix0 = m_matrix0;
			args->m_matrix1 = m_matrix1;
			args->m_matrix_final = m_matrix_final;
			prtList.push_back(args);
			// make thread and send off
			if (pthread_create(&thread, NULL, &Matrix_Multiplication_stage_1, (void *)args.get()) != 0) {
				printf("Uh-oh!\n");
			}
			// add thread to list
			m_threads1.push_back(thread);
		}
	}
	// wait for threads to completee
	printf("Starting second loop thread Join with %d threads...\n", threadCount);
	for (auto obj : m_threads1) {
		pthread_join(obj, NULL);
	}
	masterDelta.addFinishTime("thread in second loop");
	masterDelta.printFinalTimeSheet();

}

void Helper::multiplyMatrix_thread2()
{
	// create thread counter
	unsigned long threadCount = 0;
	// create thread list
	std::vector<std::shared_ptr<threadArgs>> prtList;
	printf("<<Starting last loop thread mutliplication>>\n");
	masterDelta.addStartTime(TIME_MEASURMENT::Milli, "thread in last loop ");
	for (USHORT i = 0; i < m_matrix0->getRowCount(); ++i) {
		for (USHORT j = 0; j < m_matrix1->getColCount(); ++j) {
			for (USHORT k = 0; k < m_matrix0->getColCount(); ++k)
			{
				// create thread
				pthread_t thread;
				// increment thread counter
				threadCount++;
				// create thread specific data
				std::shared_ptr<threadArgs> args = std::make_shared<threadArgs>();
				args->j = i;
				args->i = j;
				args->k = k;
				args->m_matrix0 = m_matrix0;
				args->m_matrix1 = m_matrix1;
				args->m_matrix_final = m_matrix_final;
				prtList.push_back(args);
				//create  thread and send off
				if (pthread_create(&thread, NULL, &Matrix_Multiplication_stage_2, (void *)args.get()) != 0) {
					printf("Uh-oh!\n");
				}
				// add thrread to list
				m_threads2.push_back(thread);
			}
		}
	}

	// wait for threads to finish
	printf("Starting last loop thread Join with %d threads...\n", threadCount);
	for (auto obj : m_threads2) {
		pthread_join(obj, NULL);
	}
	masterDelta.addFinishTime("thread in last loop ");
	masterDelta.printFinalTimeSheet();

}
