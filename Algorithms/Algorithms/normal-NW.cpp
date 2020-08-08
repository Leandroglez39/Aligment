#include <algorithm>
#include <fstream>
#include <iostream>
#include "normal-NW.h"
#include "omp.h"
#include "Nj.h"



vector<vector<int>> needleman_wunsch(const string& s1, const string& s2, const int size1, const int size2, const int match, const int dismatch, const int gap)
{

	vector<vector<int>> matrix(size1 + 1);




	// ReSharper disable once CppUseAuto
	for (int i = 0; i < size1 + 1; i++)
		matrix[i].resize(size2 + 1);

	matrix[0][0] = 0;

	auto a = size1 >= size2 ? size1 : size2;


#pragma omp parallel for
	for (int i = 1; i < a + 1; i++)
	{

		if (i <= size1)
		{
			matrix[i][0] = i * gap;
		}

		if (i <= size2)
		{
			matrix[0][i] = i * gap;
		}





	}


	int aux[3];

	for (int i = 1; i < size1 + 1; i++)
		for (int j = 1; j < size2 + 1; j++)
		{




			if (s1[i - 1] == s2[j - 1]) {

				matrix[i][j] = matrix[i - 1][j - 1] + match;


			}
			else
			{
				aux[0] = matrix[i - 1][j] + gap;
				aux[1] = matrix[i][j - 1] + gap;
				aux[2] = matrix[i - 1][j - 1] + dismatch;

				matrix[i][j] = max(max(aux[0], aux[1]), aux[2]);

			}

		}



	return matrix;



}

vector<string>  back_trace(const string& s1, const string& s2, vector<vector<int>> matrix)
{
	vector<string> alignment(2);

	alignment[0] = "";
	alignment[1] = "";


	int i = matrix.size() - 1;
	int j = matrix[0].size() - 1;
	int gap_value = -1;

	while (i != 0 && j != 0)
	{
		gap_value = -1;

		if (s1[i - 1] == s2[j - 1])
			gap_value = 1;

		if (matrix[i][j] == (matrix[i - 1][j - 1] + gap_value))
		{
			alignment[0] = s1[i - 1] + alignment[0];
			alignment[1] = s2[j - 1] + alignment[1];
			i--;
			j--;
		}
		else if (matrix[i][j] == (matrix[i - 1][j] + gap_value))
		{

			alignment[0] = s1[i - 1] + alignment[0];
			alignment[1] = "-" + alignment[1];
			i--;
		}
		else
		{
			alignment[0] = "-" + alignment[0];
			alignment[1] = s2[j - 1] + alignment[1];
			j--;
		}
	}



	return alignment;
}

vector<vector<int>> needleman_wunsch_parallel(const string& s1, const string& s2, const int size, const int match, const int dismatch, const int gap)
{
	vector<vector<int>> matrix(size + 1);



	omp_set_num_threads(4);
	long count_operation = 0;

	// ReSharper disable once CppUseAuto
#pragma omp parallel for
	for (int i = 0; i < size + 1; i++)
		matrix[i].resize(size + 1);

	matrix[0][0] = 0;

#pragma omp parallel for
	for (int i = 1; i < size + 1; i++)
	{
		matrix[0][i] = i * gap;
		matrix[i][0] = i * gap;
	}

	auto diagonal_matrix = diagonal_order(size, size);

	const int sizeof_diagonal_matrix = diagonal_matrix.size();

	int aux[3];

	int index_i, index_j;
	vector<pair<int, int>> current;
	int j;

	for (int i = 0; i < sizeof_diagonal_matrix; i++)
	{
		current = diagonal_matrix[i];


#pragma omp parallel for shared(matrix) schedule(static)
		for (j = 0; j < size; j++)
		{
			index_i = current[j].first;
			index_j = current[j].second;

			if (index_i != 0 && index_j != 0)
			{
				if (s1[index_i - 1] == s2[index_j - 1])
				{


					matrix[index_i][index_j] = matrix[index_i - 1][index_j - 1] + match;
					count_operation++;

				}
				else
				{
					aux[0] = matrix[index_i - 1][index_j] + gap;
					aux[1] = matrix[index_i][index_j - 1] + gap;
					aux[2] = matrix[index_i - 1][index_j - 1] + dismatch;

					matrix[index_i][index_j] = max(max(aux[0], aux[1]), aux[2]);
					count_operation++;
				}
			}
			else
			{
				break;
			}



		}
	}








	cout << count_operation << "\n";

	return matrix;

}

// The main function that prints given matrix in diagonal order 
vector<vector<pair<int, int>>> diagonal_order(int row, int col)
{
	vector<vector<pair<int, int>>> matrix(row + col - 1);

	omp_set_num_threads(4);

#pragma omp parallel for	
	for (int i = 0; i < row + col - 1; i++)
		matrix[i].resize(col);


	int k = 0;
	// There will be ROW+COL-1 lines in the output

#pragma omp parallel for 
	for (int line = 1; line <= (row + col - 1); line++)
	{
		/* Get column index of the first element in this line of output.
		   The index is 0 for first ROW lines and line - ROW for remaining
		   lines  */
		int start_col = max(0, line - row);

		/* Get count of elements in this line. The count of elements is
		   equal to minimum of line number, COL-start_col and ROW */
		int count = min(line, min((col - start_col), row));

		/* elements of this line */
#pragma omp parallel for
		for (int j = 0; j < count; j++)
			matrix[k][j] = (make_pair((min(row, line) - j - 1) + 1, (start_col + j) + 1));

		k++;


	}

	return matrix;


}

vector<int> parallel_needleman_wunsch(const string& s1, const string& s2, const int size_row, const int size_col, const int match, const int dismatch, const int gap)
{

	omp_set_num_threads(omp_get_max_threads());

	vector<vector<int>> matrix(size_row + 1);



	vector<vector<int>> step_trace_back(size_row + 1);

	vector<int>trace_back;

#pragma omp parallel for
	for (int i = 0; i < size_row + 1; i++)
	{
		matrix[i].resize(size_col + 1);
		step_trace_back[i].resize(size_col + 1);
	}
	matrix[0][0] = 0;
	step_trace_back[0][0] = 0;

#pragma omp parallel for	
	for (int i = 1; i < size_row + 1; i++)
	{
		if (i <= size_col)
		{
			matrix[0][i] = i * gap;
			matrix[i][0] = i * gap;
			step_trace_back[i][0] = 1;
		}
		else
		{
			matrix[i][0] = i * gap;
			step_trace_back[0][i] = 2;
		}
	}


	int aux[3];

	int z = 1;
	int j = 0;

	for (int i = 1; i < size_row + 1; i++)
	{
#pragma omp parallel for private(aux) shared(i, matrix, step_trace_back)
		for (j = i; j < 0; j--)
		{
			if (z >= size_col + 1)
				break;


			if (s1[j - 1] == s2[z - 1]) {

				matrix[j][z] = matrix[j - 1][z - 1] + match;
				step_trace_back[i][j] = 0;

			}
			else
			{
				aux[0] = matrix[j - 1][z] + gap;
				aux[1] = matrix[j][z - 1] + gap;
				aux[2] = matrix[j - 1][z - 1] + dismatch;


				if (aux[2] >= aux[1])
				{
					if (aux[2] >= aux[0])
					{
						matrix[j][z] = aux[2];

						step_trace_back[j][z] = 0;
					}
					else
					{
						matrix[j][z] = aux[0];

						step_trace_back[j][z] = 1;
					}
				}
				else
				{
					if (aux[1] >= aux[0])
					{
						matrix[j][z] = aux[1];

						step_trace_back[j][z] = 2;
					}
					else
					{
						matrix[j][z] = aux[0];

						step_trace_back[j][z] = 1;
					}
				}




			}
			z++;
		}


	}

	j = size_row;

	for (int i = 2; i < size_row + 1; i++)
	{
#pragma omp parallel for private(aux) shared(i, matrix, step_trace_back)
		for (z = i; z < size_col + 1; z++)
		{
			if (j >= 0)
				break;

			if (s1[j - 1] == s2[z - 1]) {

				matrix[j][z] = matrix[j - 1][z - 1] + match;
				step_trace_back[j][z] = 0;

			}
			else
			{
				aux[0] = matrix[j - 1][z] + gap;
				aux[1] = matrix[j][z - 1] + gap;
				aux[2] = matrix[j - 1][z - 1] + dismatch;


				if (aux[2] >= aux[1])
				{
					if (aux[2] >= aux[0])
					{
						matrix[j][z] = aux[2];

						step_trace_back[j][z] = 0;
					}
					else
					{
						matrix[j][z] = aux[0];

						step_trace_back[j][z] = 1;
					}
				}
				else
				{
					if (aux[1] >= aux[0])
					{
						matrix[j][z] = aux[1];

						step_trace_back[j][z] = 2;
					}
					else
					{
						matrix[j][z] = aux[0];

						step_trace_back[j][z] = 1;
					}
				}

			}
			j--;
		}


	}

	trace_back = NJ::build_trace_back(step_trace_back);


	return trace_back;



}