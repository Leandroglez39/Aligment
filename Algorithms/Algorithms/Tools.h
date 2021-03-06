#pragma once
#include <string>
#include <vector>
#include <queue>
#include <ctime>



using namespace std;



// typedef for matrix object
struct matrix {
	unsigned dimension{};
	vector<vector<int>> matrix_d;
};

// ReSharper disable once CppInconsistentNaming
struct Trazas
{
	string* trazas;
};
// ReSharper disable once CppInconsistentNaming
class Tools
{

public:
	static int levenshtein_secuencial(const string& s1, const string& s2);

	static int levenshtein_parallel(const string& s1, const string& s2, int index1, int index2);

	static int minimum(int a, int b, int c);

	static int** llenar_matriz();

	static matrix* llenar_matriz2(string* data, unsigned size);

	static void set_lista_traza(string traza[]);

	static void print_matrix(matrix& a);

	static  void ordenamiento();

	static void inicio(string* trazas, int trazas_size);

	static void create_trazas(const int size);

	static void AlinearTrazas(string* trazas);

	static string* trazas;

	static string* trazas_ordenadas;

	static priority_queue<pair<int, pair<int, int>>> heap;

	static int trazas_size;

	static vector<vector<int>> calculate_dist_matrix(vector<string>& sequences, int& init);

	static void seq_sort(vector<string>& sequences, vector<int>& check_seq, int& init, vector<string>& sequences_sorted, vector<vector<int>>& matrix);
};
