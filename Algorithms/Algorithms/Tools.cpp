#include "Tools.h"
#include <algorithm>
#include <omp.h>
#include <chrono>
#include <iostream>
#include <xlocmon>
#include <queue>






#include <sstream>
#include <cstdint>

#include "edlib.h"
#include "_editdistance.h"



string* Tools::trazas;

string* Tools::trazas_ordenadas;

priority_queue<pair<int, pair<int, int>>> Tools::heap;

/**
 * \brief
 * Tamaño de la lista de trazas
 */
int Tools::trazas_size;

int Tools::minimum(int a, int b, int c) {
	int min = a;

	if (b < min) min = b;
	if (c < min) min = c;

	return min;
}



// initialize matrix to dimension x dimension, fill with zeros
void init_matrix(matrix* m, const unsigned dimension) {
	m->dimension = dimension;
	m->matrix_d.resize(dimension);
	for (unsigned i = 0; i < dimension; i++) {
		m->matrix_d[i].resize(dimension);
	}
}

int** Tools::llenar_matriz()
{/*
	// ReSharper disable once CppUseAuto
	const int n = trazas_size_;

	// ReSharper disable once CppUseAuto
	int** matriz_distancias = new int* [n];
	int d_temp;

	// ReSharper disable once CppUseAuto
	matrix* M = new matrix();  // NOLINT(hicpp-use-auto)
	init_matrix(M, n);

	for (int i = 0; i < n; i++) {

		M->matrix_d[i][i] = 0;
	}

	// ReSharper disable once CppUseAuto
	for (int i = 0; i < n; i++)
		// ReSharper disable once CppUseAuto
		for (int j = i; j < n && j != i; j++)
		{

			//	// se calcula la distancia entre las trazas usando la distancia de levenshtein
			d_temp = levenshtein_parallel(trazas_[i], trazas_[j], static_cast<int>(i), static_cast<int>(j));
			//	// se van agregando a la matriz las distancias
			M->matrix_d[i][j] = d_temp;
			M->matrix_d[j][i] = d_temp;
		}

	for (int i = 0; i < n; i++) {

		// se inicializa la fila de la matriza que se va a llenar
		matriz_distancias[i] = new int[n];
		matriz_distancias[i][i] = 0;
	}
	//#pragma omp parrallel for collapse(2) private(i,j,t) shared(n,matriz_distancias,D_temp) schedule(static)
	for (int i = 0; i < n; i++)
		for (int j = i; j < n && j != i; j++)
		{

			//	// se calcula la distancia entre las trazas usando la distancia de levenshtein
			d_temp = levenshtein_parallel(trazas_[i], trazas_[j], static_cast<int>(i), static_cast<int>(j));
			//	// se van agregando a la matriz las distancias
			matriz_distancias[i][j] = d_temp;
			matriz_distancias[j][i] = d_temp;
		}

	return matriz_distancias;*/
	return nullptr;
}

matrix* Tools::llenar_matriz2(string* data, const unsigned size)
{



	matrix* M = new matrix();
	init_matrix(M, size);
	int size2 = static_cast<int>(size);

	for (unsigned i = 0; i < size; i++) {

		M->matrix_d[i][i] = 0;
	}
	//private(i,j,t) shared(size,matrix_d,d_temp) schedule(static)
#pragma omp parallel for
	for (int i = 0; i < size2; i++)
		// ReSharper disable once CppUseAuto
#pragma omp parallel for
		for (int j = i + 1; j < size2; j++)
		{

			//	// se calcula la distancia entre las trazas usando la distancia de levenshtein
			const auto d_temp = levenshtein_parallel(data[i], data[j], i, j);
			//	// se van agregando a la matriz las distancias
			M->matrix_d[i][j] = d_temp;
			M->matrix_d[j][i] = d_temp;
		}

	return M;


}

void Tools::print_matrix(matrix& a) {
	for (unsigned i = 0; i < a.dimension; i++) {
		for (unsigned j = 0; j < a.dimension; j++) {
			cout << a.matrix_d[i][j] << " ";
		}
		cout << endl;
	}
	cout << endl;
}

void Tools::ordenamiento()
{
	clock_t star = clock();
	int i;
	int j;
	//private(i,j) shared(trazas_size) schedule(static)
#pragma omp parallel for
	// ReSharper disable once CppUseAuto
	for (i = 0; i < trazas_size; i++)
		// ReSharper disable once CppUseAuto
#pragma omp parallel for
		for (j = i + 1; j < trazas_size; j++)
		{

			//	// se calcula la distancia entre las trazas usando la distancia de levenshtein
			levenshtein_parallel(trazas[i], trazas[j], i, j);
			//	// se van agregando a la matriz las distancias

		}


	// ReSharper disable once CppUseAuto
	// ReSharper disable once CppLocalVariableMayBeConst
	// 
	cout << "Matriz de distancia: ";
	cout << double(clock() - star) / (double)CLOCKS_PER_SEC << " seconds." << endl;

	auto checked = new string[trazas_size];

	for (int i = 0; i < trazas_size; i++)
		checked[i] = "-";

	star = clock();
	i = 0;
	while (i < trazas_size)
	{
		const auto temp = heap.top();
		heap.pop();
		if (checked[temp.second.first] == "-")
		{
			checked[temp.second.first] = "*";
			trazas_ordenadas[i] = trazas[temp.second.first];
			i++;

			if (i < trazas_size && checked[temp.second.second] == "-")
			{
				checked[temp.second.second] = "*";
				trazas_ordenadas[i] = trazas[temp.second.second];
				i++;
			}
		}
		else if (i < trazas_size && checked[temp.second.second] == "-")
		{
			checked[temp.second.second] = "*";
			trazas_ordenadas[i] = trazas[temp.second.second];
			i++;
		}

	}

	cout << "Ordenamiento: ";
	cout << double(clock() - star) / (double)CLOCKS_PER_SEC << " seconds." << endl;






}

// ReSharper disable once CppParameterNamesMismatch

void Tools::inicio(string* list_trazas, const int size)
{


	trazas = list_trazas;

	trazas_ordenadas = new string[size];

	trazas_size = size;


	ordenamiento();

}

void Tools::create_trazas(const int size)
{
	//trazas_ = new string[size];
}

vector<vector<int>> Tools::calculate_dist_matrix(vector<string>& sequences, int& init)
{
	omp_set_num_threads(4);

	string s1;
	string s2;

	int64_t* query = new int64_t[1400];
	int64_t* target = new int64_t[1400];


	vector<vector<int>> aux_matrix(sequences.size(), vector<int>(sequences.size(), 0));

	unsigned max_distance = 1400;

	unsigned t;

#pragma omp parallel for private(s1, s2) collapse(2) schedule(static)
	for (int i = 0; i < sequences.size(); i++)
		for (int j_size = i + 1; j_size < sequences.size(); j_size++)
		{




			s1 = sequences[i];
			s2 = sequences[j_size];

#pragma omp critical
			{

				move(s1.begin(), s1.end(), query);
				query[s1.size()] = '\0';
				move(s2.begin(), s2.end(), target);
				target[s2.size()] = '\0';
			}




			t = edit_distance(query, s1.size(), target, s2.size());



			if (max_distance > t)
			{
				max_distance = t;
				init = i;

			}
			//const auto t = edlibAlign(query, s1.size(), target, s2.size(), edlibNewAlignConfig(s1.size()+1, EDLIB_MODE_NW, EDLIB_TASK_PATH, nullptr, 0)).editDistance;


			aux_matrix[i][j_size] = t;
			aux_matrix[j_size][i] = t;


		}

	delete[] query;
	delete[] target;


	//edlibFreeAlignResult(result);

	return aux_matrix;
}


void Tools::set_lista_traza(string traza[])
{
	trazas = traza;
}

int Tools::levenshtein_secuencial(const string& s1, const string& s2)
{



	const int N1 = static_cast<int>(s1.size());
	const int N2 = static_cast<int>(s2.size());
	int i;
	vector<int> T(static_cast<__int64>(N2) + 1);

	for (i = 0; i <= N2; i++)
		T[i] = i;

	for (i = 0; i < N1; i++)
	{
		T[0] = i + 1;
		auto corner = i;
		for (auto j = 0; j < N2; j++)
		{
			auto upper = T[static_cast<__int64>(j) + 1];
			if (s1[i] == s2[j])
				T[static_cast<__int64>(j) + 1] = corner;
			else
				T[static_cast<__int64>(j) + 1] = min(T[j], min(upper, corner)) + 1;
			corner = upper;
		}
	}




	return T[N2];
}

int Tools::levenshtein_parallel(const string& s1, const string& s2, int index1, int index2)
{



	omp_set_num_threads(omp_get_max_threads());

	struct value
	{
		int distance;
		long long time;
	};

	const int n1 = static_cast<int>(s1.size());
	const int n2 = static_cast<int>(s2.size());
	int i;
	int j;
	vector<int> T(static_cast<__int64>(n2) + 1);

	//#pragma omp parallel for  // Puede ser usada esta linea no presupone mejoras visibles.
	for (i = 0; i <= n2; i++)
		T[i] = i;

	i = 0;
#pragma omp parallel for shared(n1, n2, s2, T, s1) schedule(static)	
	for (i = 0; i < n1; i++)
	{
		T[0] = i + 1;
		auto corner = i;
#pragma omp parallel for private(corner, j) shared(n1, n2, s2, T, s1) schedule(static)
		// ReSharper disable once CppUseAuto
		for (j = 0; j < n2; j++)
		{
			auto upper = T[1 + static_cast<__int64>(j)];
			if (s1[i] == s2[j])
				T[1 + static_cast<__int64>(j)] = corner;
			else
				T[1 + static_cast<__int64>(j)] = min(T[j], min(upper, corner)) + 1;
			corner = upper;
		}
	}
	const auto value = T[n2];
	auto tuple = pair<int, int>(index1, index2);

	heap.push(pair<int, pair<int, int>>(value * -1, tuple));
	return value;
}


void Tools::seq_sort(vector<string>& sequences, vector<int>& check_seq, int& init, vector<string>& sequences_sorted, vector<vector<int>>& matrix)
{
	sequences_sorted.push_back(sequences[init]);

	check_seq[init] = 1;

	int index = init;
	int max = 1400;
	int index_j = init;

	while (sequences_sorted.size() < sequences.size())
	{


		for (int j = 0; j < sequences.size(); j++)
		{

			if (j != index && check_seq[j] == 0 && matrix[index][j] < max)
			{
				max = matrix[index][j];
				index_j = j;
			}

		}
		sequences_sorted.push_back(sequences[index_j]);
		check_seq[index_j] = 1;
		index = index_j;

		max = 1400;

	}

}