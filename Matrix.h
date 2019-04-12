#pragma once
#include <vector>


/*

class: Matrix
Description: a matric obj with lla related methods and data relevet to a matrix structure

*/

/// typedef for long types
using USHORT = unsigned short;

class Matrix
{
public:
	Matrix() {};
	~Matrix() {};
	/// Accessor to matrix
	std::vector< std::vector<USHORT>> & getValue();
	/// Add value into matrix
	bool addValue(USHORT row, USHORT col, USHORT val);

	/// accessor mutator for rows
	bool setRowCount(USHORT k);
	const USHORT & getRowCount();

	/// accessor mutator for columes
	bool setColCount(USHORT k);
	const USHORT & getColCount();

	/// auto fill matric with parameters random
	bool autoFillMatrix(USHORT min, USHORT max);
	/// clear all values in  matrix
	void clear();

private:
	std::vector< std::vector<USHORT>> m_matrix;
	USHORT m_rowCount;
	USHORT m_colCount;
};
