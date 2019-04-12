#include <vector>
#include <map>
/// user classes
#include "MultiplicationHelper.h"
#include "Matrix.h"

// global man min and row col values used for testing
USHORT MAX_VALUE = 1;
USHORT MIN_VALUE = 1;
USHORT ROW_LENGTH = 500;
USHORT COL_LENGTH = 500;

// all matrix objects
std::shared_ptr<Matrix> matrix_final0 = std::make_shared<Matrix>();
std::shared_ptr<Matrix> matrix_final1 = std::make_shared<Matrix>();
std::shared_ptr<Matrix> matrix_final2 = std::make_shared<Matrix>();
std::shared_ptr<Matrix> matrix_final3 = std::make_shared<Matrix>();

std::shared_ptr<Matrix> matrix_00 = std::make_shared<Matrix>();
std::shared_ptr<Matrix> matrix_01 = std::make_shared<Matrix>();
std::shared_ptr<Matrix> matrix_10 = std::make_shared<Matrix>();
std::shared_ptr<Matrix> matrix_11 = std::make_shared<Matrix>();
std::shared_ptr<Matrix> matrix_20 = std::make_shared<Matrix>();
std::shared_ptr<Matrix> matrix_21 = std::make_shared<Matrix>();
std::shared_ptr<Matrix> matrix_30 = std::make_shared<Matrix>();
std::shared_ptr<Matrix> matrix_31 = std::make_shared<Matrix>();

int main() 
{
	// helper obj for each matrix
	Helper mathHelper0(matrix_00, matrix_01, matrix_final0);
	Helper mathHelper1(matrix_10, matrix_11, matrix_final1);
	Helper mathHelper2(matrix_20, matrix_21, matrix_final2);
	Helper mathHelper3(matrix_30, matrix_31, matrix_final3);

	// settign row and col values foe each matrix
	matrix_00->setRowCount(ROW_LENGTH);
	matrix_00->setColCount(COL_LENGTH);
	matrix_01->setRowCount(ROW_LENGTH);
	matrix_01->setColCount(COL_LENGTH);

	matrix_10->setRowCount(ROW_LENGTH);
	matrix_10->setColCount(COL_LENGTH);
	matrix_11->setRowCount(ROW_LENGTH);
	matrix_11->setColCount(COL_LENGTH);

	matrix_20->setRowCount(ROW_LENGTH);
	matrix_20->setColCount(COL_LENGTH);
	matrix_21->setRowCount(ROW_LENGTH);
	matrix_21->setColCount(COL_LENGTH);

	matrix_30->setRowCount(ROW_LENGTH);
	matrix_30->setColCount(COL_LENGTH);
	matrix_31->setRowCount(ROW_LENGTH);
	matrix_31->setColCount(COL_LENGTH);

	// auto fillign matricies with global values
	matrix_00->autoFillMatrix(MIN_VALUE, MAX_VALUE);
	matrix_01->autoFillMatrix(MIN_VALUE, MAX_VALUE);

	matrix_10->autoFillMatrix(MIN_VALUE, MAX_VALUE);
	matrix_11->autoFillMatrix(MIN_VALUE, MAX_VALUE);

	matrix_20->autoFillMatrix(MIN_VALUE, MAX_VALUE);
	matrix_21->autoFillMatrix(MIN_VALUE, MAX_VALUE);

	matrix_30->autoFillMatrix(MIN_VALUE, MAX_VALUE);
	matrix_31->autoFillMatrix(MIN_VALUE, MAX_VALUE);

	// populate matrix_final0 with value 0;
	matrix_final0->setRowCount(matrix_00->getRowCount());
	matrix_final0->setColCount(matrix_01->getColCount());
	for (size_t i = 0; i < matrix_00->getRowCount(); i++)
	{
		// add vector into vector
		matrix_final0->getValue().push_back(std::vector<USHORT>());
		auto count = matrix_01->getColCount();
		// iterate over vector and add default value 0
		for (size_t k = 0; k < matrix_01->getColCount(); k++)
		{
			matrix_final0->getValue()[i].push_back(0);
		}
	}

	// populate matrix_final1 with value 0;
	matrix_final1->setRowCount(matrix_10->getRowCount());
	matrix_final1->setColCount(matrix_11->getColCount());
	for (size_t i = 0; i < matrix_10->getRowCount(); i++)
	{
		// add vector into vector
		matrix_final1->getValue().push_back(std::vector<USHORT>());
		// iterate over vector and add default value 0
		for (size_t k = 0; k < matrix_11->getColCount(); k++)
		{
			matrix_final1->getValue()[i].push_back(0);
		}
	}

	// populate matrix_final2 with value 0;
	matrix_final2->setRowCount(matrix_20->getRowCount());
	matrix_final2->setColCount(matrix_21->getColCount());
	for (size_t i = 0; i < matrix_20->getRowCount(); i++)
	{
		// add vector into vector
		matrix_final2->getValue().push_back(std::vector<USHORT>());
		// iterate over vector and add default value 0
		for (size_t k = 0; k < matrix_21->getColCount(); k++)
		{
			matrix_final2->getValue()[i].push_back(0);
		}
	}

	// populate matrix_final3 with value 0;
	matrix_final3->setRowCount(matrix_30->getRowCount());
	matrix_final3->setColCount(matrix_31->getColCount());
	for (size_t i = 0; i < matrix_30->getRowCount(); i++)
	{
		// add vector into vector
		matrix_final3->getValue().push_back(std::vector<USHORT>());
		// iterate over vector and add default value 0
		for (size_t k = 0; k < matrix_31->getColCount(); k++)
		{
			matrix_final3->getValue()[i].push_back(0);
		}
	}

	// call Helpers with associated matrix multiplication
	/// UNCOMMENT to test each with glboal values at top of file
	mathHelper0.multiplyMatrix();
	//mathHelper1.multiplyMatrix_thread0();
	//mathHelper2.multiplyMatrix_thread1();
	//mathHelper3.multiplyMatrix_thread2();

	// pause application
	int pausePlease;
	std::cin >> pausePlease;

	return 0;
}
