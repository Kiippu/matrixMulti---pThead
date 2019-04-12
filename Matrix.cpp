#pragma once
#include "Matrix.h"
#include <iostream>


/// Accessor to matrix
std::vector< std::vector<USHORT>> & Matrix::getValue() { return m_matrix; };
/// Add value into matrix
bool Matrix::addValue(USHORT row, USHORT col, USHORT val)
{
	USHORT origSize = m_matrix.size();
	return (m_matrix[row][col] = val);
	return (origSize < m_matrix.size());
};

// accessor andmutators
bool Matrix::setRowCount(USHORT k) { return (m_rowCount = k); };
const USHORT & Matrix::getRowCount() { return m_rowCount; };
bool Matrix::setColCount(USHORT k) { return(m_colCount = k); };
const USHORT & Matrix::getColCount() { return m_colCount; };

bool Matrix::autoFillMatrix(USHORT min, USHORT max)
{
	//clear matrix before oporating on it.
	this->clear();
	for (size_t i = 0; i < m_rowCount; i++)
	{
		// row num
		// create vector of vectors as matrix
		std::vector<USHORT> tempVec;
		for (size_t k = 0; k < m_colCount; k++)
		{
			// add random valueinto matrix
			auto count = USHORT(rand() % max + min);
			tempVec.push_back(count);
			if (tempVec[k] > max || tempVec[k] < min)
				return false;
		}
		// add vector
		m_matrix.push_back(tempVec);
	}
	return true;
};
// clear all contents from matrix
void Matrix::clear() { m_matrix.clear(); }
