#pragma once
#include <memory>
#include "GlobalTimer.h"
#include "Matrix.h"
#include <atomic>

/*

class: Helper
Description: threading and multiplication helper obj

*/

class Helper
{
public:
	Helper(std::shared_ptr<Matrix> m0, std::shared_ptr<Matrix> m1, std::shared_ptr<Matrix> mf);
	~Helper() {};
	/// no threads multiplication
	void multiplyMatrix();
	/// thread each first loop
	void multiplyMatrix_thread0();
	/// thread each secont loop
	void multiplyMatrix_thread1();
	/// thread each last loop
	void multiplyMatrix_thread2();

private:

	std::shared_ptr<Matrix> m_matrix0;
	std::shared_ptr<Matrix> m_matrix1;
	std::shared_ptr<Matrix> m_matrix_final;

	GlobalTimer masterDelta;

	unsigned m_threadCount;
};

// struct to pass in with nessecary data
struct threadArgs {
	std::shared_ptr<Matrix> m_matrix0;
	std::shared_ptr<Matrix> m_matrix1;
	std::shared_ptr<Matrix> m_matrix_final;
	unsigned long i;
	unsigned long j;
	unsigned long k;
	std::atomic<bool> m_completeFlag = false;

	bool checkAtomic() { return m_completeFlag; }
};