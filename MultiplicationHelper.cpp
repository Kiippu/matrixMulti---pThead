#pragma once
#include "MultiplicationHelper.h"
#define HAVE_STRUCT_TIMESPEC
#include "include/pthread.h"
#include "include/sched.h"
#include "Matrix.h"
#include <vector>
#include <map>

// thread lists
std::map<unsigned,pthread_t> m_threads0;
std::map<unsigned, pthread_t> m_threads1;
std::map<unsigned, pthread_t> m_threads2;

unsigned MAX_THREAD = 8;

//no threading loop
void *Matrix_Multiplication_noThread(void *arguments)
{
	struct threadArgs *args = (struct threadArgs *)arguments;
	const USHORT& count0 = args->m_matrix0->getColCount();
	const USHORT& count1 = args->m_matrix1->getColCount();
	const USHORT& count2 = args->m_matrix0->getColCount();
	auto& matrix_final = args->m_matrix_final->getValue();
	auto& matrix_0 = args->m_matrix0->getValue();
	auto& matrix_1 = args->m_matrix1->getValue();

	for (USHORT i = 0; i < count0; ++i) {
		for (USHORT j = 0; j < count1; ++j) {
			for (USHORT k = 0; k < count2; ++k)
			{
				matrix_final[i][j] += matrix_0[i][k] * matrix_1[k][j];
			}
		}
	}
	return arguments;
}

//Thread first loop of multiplication
void *Matrix_Multiplication_stage_0(void *arguments)
{
	struct threadArgs *args = (struct threadArgs *)arguments;
	const USHORT& count0 = args->m_matrix0->getColCount();
	const USHORT& count1 = args->m_matrix1->getColCount();
	auto& matrix_final = args->m_matrix_final->getValue();
	auto& matrix_0 = args->m_matrix0->getValue();
	auto& matrix_1 = args->m_matrix1->getValue();
	
	for (USHORT j = 0; j < count0; ++j) {
		for (USHORT k = 0; k < count1; ++k)
		{
			matrix_final[args->i][j] += matrix_0[args->i][k] * matrix_1[k][j];
		}
	}
	args->m_completeFlag = true;
	pthread_exit(arguments);
	return args;
}

//Thread second loop of multiplication
void *Matrix_Multiplication_stage_1(void *arguments)
{
	struct threadArgs *args = (struct threadArgs *)arguments;
	const USHORT& count0 = args->m_matrix0->getColCount();
	auto& matrix_final = args->m_matrix_final->getValue();
	auto& matrix_0 = args->m_matrix0->getValue();
	auto& matrix_1 = args->m_matrix1->getValue();

	for (USHORT k = 0; k < count0; ++k)
	{
		matrix_final[args->i][args->j] += matrix_0[args->i][k] * matrix_1[k][args->j];
	}
	args->m_completeFlag = true;
	pthread_exit(arguments);
	return args;
}

//Thread last loop of multiplication
void *Matrix_Multiplication_stage_2(void *arguments)
{
	struct threadArgs *args = (struct threadArgs *)arguments;

	args->m_matrix_final->getValue()[args->i][args->j] += args->m_matrix0->getValue()[args->i][args->k] * args->m_matrix1->getValue()[args->k][args->j];

	args->m_completeFlag = true;
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

	Matrix_Multiplication_noThread(args.get());

	//printf("Starting 1 thread Join...\n");
//	pthread_join(thread, NULL);
	masterDelta.addFinishTime("no thread: ");
	masterDelta.printFinalTimeSheet();
}

void Helper::multiplyMatrix_thread0()
{
	// set thread creation counter
	unsigned long threadIndex = 0;
	unsigned threadCount = 1;
	std::map<unsigned,std::shared_ptr<threadArgs>> prtList;
	printf("<<Starting first loop thread mutliplication>>\n");
	masterDelta.addStartTime(TIME_MEASURMENT::Milli, "thread in first loop");
	for (USHORT i = 0; i < m_matrix0->getRowCount(); ++i)
	{
		// create thread for loop
		pthread_t thread;

		short removeIndex = -1;
		while (MAX_THREAD == prtList.size())
		{
			for(int x = 0 ; x < prtList.size(); x++) 
			{
				if (prtList[x]->checkAtomic() == true)
				{
					if (threadCount < prtList.size())
						threadCount = prtList.size();
					threadIndex = x;
					prtList.erase(x);
					pthread_join(m_threads0[x], NULL);
					m_threads0.erase(x);
					break;
				}
					//prtList.erase(prtList.begin() + i);
			}
		}

		// create thread specific data
		std::shared_ptr<threadArgs> args = std::make_shared<threadArgs>();
		args->i = i;
		args->m_matrix0 = m_matrix0;
		args->m_matrix1 = m_matrix1;
		args->m_matrix_final = m_matrix_final;
		// place data in list so it isnt removed when scope run out
		prtList.insert(std::make_pair(threadIndex,args));
		

		if (pthread_create(&thread, NULL, &Matrix_Multiplication_stage_0, (void *)args.get()) != 0) {
			printf("Uh-oh!\n");
		}
		m_threads0.insert(std::make_pair((prtList.size()-1),thread));
		// increment thread counter
		threadIndex++;
	}
	
	// wait for threads to finish
	//printf("Starting first loop thread Join with %d threads...\n", threadCount);
	for (auto obj : m_threads0) {
		pthread_join(obj.second, NULL);
	}

	masterDelta.addFinishTime("thread in first loop");
	masterDelta.printFinalTimeSheet();

	printf("Starting first loop thread Joined using a maximum of %d threads...\n", threadCount);
	printf("\n\n");
	/*for (size_t i = 0; i < m_matrix0->getRowCount(); i++)
	{
		m_matrix_final->getValue().push_back(std::vector<USHORT>());
		for (size_t k = 0; k < m_matrix1->getColCount(); k++)
		{
			printf("%d ,", m_matrix_final->getValue()[i][k]);
		}
		printf("\n");
	}*/



}

void Helper::multiplyMatrix_thread1()
{
	unsigned long threadIndex = 0;
	unsigned threadCount = 1;
	std::map<unsigned, std::shared_ptr<threadArgs>> prtList;
	printf("<<Starting second loop thread mutliplication>>\n");
	masterDelta.addStartTime(TIME_MEASURMENT::Milli, "thread in second loop");
	for (USHORT i = 0; i < m_matrix0->getRowCount(); ++i)
	{
		for (USHORT j = 0; j < m_matrix1->getColCount(); ++j) {
			// create thread
			pthread_t thread;

			short removeIndex = -1;
			while (MAX_THREAD == prtList.size())
			{
				for (int x = 0; x < prtList.size(); x++)
				{
					if (prtList[x]->checkAtomic() == true)
					{
						if (threadCount < prtList.size())
							threadCount = prtList.size();
						threadIndex = x;
						prtList.erase(x);
						pthread_join(m_threads1[x], NULL);
						m_threads1.erase(x);
						break;
					}
					//prtList.erase(prtList.begin() + i);
				}
			}

			// create data needed in this thread
			std::shared_ptr<threadArgs> args = std::make_shared<threadArgs>();
			args->j = i;
			args->i = j;
			args->m_matrix0 = m_matrix0;
			args->m_matrix1 = m_matrix1;
			args->m_matrix_final = m_matrix_final;
			prtList.insert(std::make_pair(threadIndex, args));
			// make thread and send off
			if (pthread_create(&thread, NULL, &Matrix_Multiplication_stage_1, (void *)args.get()) != 0) {
				printf("Uh-oh!\n");
			}
			// add thread to list
			m_threads1.insert((std::make_pair((prtList.size() - 1), thread)));

			// increment thread count counter
			threadIndex++;
		}
	}
	// wait for threads to completee
	printf("Starting second loop thread Joining with %d threads...\n", threadCount);
	for (auto obj : m_threads1) {
		pthread_join(obj.second, NULL);
	}
	masterDelta.addFinishTime("thread in second loop");
	masterDelta.printFinalTimeSheet();

	printf("\n\n");
	/*for (size_t i = 0; i < m_matrix1->getRowCount(); i++)
	{
		m_matrix_final->getValue().push_back(std::vector<USHORT>());
		for (size_t k = 0; k < m_matrix1->getColCount(); k++)
		{
			printf("%d ,", m_matrix_final->getValue()[i][k]);
		}
		printf("\n");
	}*/

}

void Helper::multiplyMatrix_thread2()
{
	unsigned long threadIndex = 0;
	unsigned threadCount = 1;
	std::map<unsigned, std::shared_ptr<threadArgs>> prtList;
	printf("<<Starting last loop thread mutliplication>>\n");
	masterDelta.addStartTime(TIME_MEASURMENT::Milli, "thread in last loop ");
	for (USHORT i = 0; i < m_matrix0->getRowCount(); ++i) {
		for (USHORT j = 0; j < m_matrix1->getColCount(); ++j) {
			for (USHORT k = 0; k < m_matrix0->getColCount(); ++k)
			{
				// create thread
				pthread_t thread;

				short removeIndex = -1;
				while (MAX_THREAD == prtList.size())
				{
					for (int x = 0; x < prtList.size(); x++)
					{
						if (prtList[x]->checkAtomic() == true)
						{
							if (threadCount < prtList.size())
								threadCount = prtList.size();
							threadIndex = x;
							prtList.erase(x);
							pthread_join(m_threads2[x], NULL);
							m_threads2.erase(x);
							break;
						}
						//prtList.erase(prtList.begin() + i);
					}
				}
				// create thread specific data
				std::shared_ptr<threadArgs> args = std::make_shared<threadArgs>();
				args->j = i;
				args->i = j;
				args->k = k;
				args->m_matrix0 = m_matrix0;
				args->m_matrix1 = m_matrix1;
				args->m_matrix_final = m_matrix_final;
				prtList.insert(std::make_pair(threadIndex, args));
				//create  thread and send off
				if (pthread_create(&thread, NULL, &Matrix_Multiplication_stage_2, (void *)args.get()) != 0) {
					printf("Uh-oh!\n");
				}
				// add thread to list
				m_threads2.insert((std::make_pair((prtList.size() - 1), thread)));

				// increment thread count counter
				threadIndex++;
			}
		}
	}

	// wait for threads to finish
	printf("Starting last loop thread Joined using %d threads...\n", threadCount);
	for (auto obj : m_threads2) {
		pthread_join(obj.second, NULL);
	}
	masterDelta.addFinishTime("thread in last loop ");
	masterDelta.printFinalTimeSheet();


	/*printf("\n\n");
	for (size_t i = 0; i < m_matrix1->getRowCount(); i++)
	{
		m_matrix_final->getValue().push_back(std::vector<USHORT>());
		for (size_t k = 0; k < m_matrix1->getColCount(); k++)
		{
			printf("%d ,", m_matrix_final->getValue()[i][k]);
		}
		printf("\n");
	}*/

}
