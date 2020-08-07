

#include <algorithm>
#include <chrono>
#include <iomanip>
#include <iostream>
#include <windows.h>
#include "edlib.h"
#include "Nj.h"
#include "Tools.h"
#include "_editdistance.h"
#include <omp.h>


using namespace std;

void printAlignment(const char* query, const char* target,
	const unsigned char* alignment, const int alignmentLength,
	const int position, const EdlibAlignMode modeCode);

long long foo(int64_t* a, int64_t* b, size_t i, size_t j);

struct tup
{
	int64_t* h;
	int64_t* q;

	tup(int64_t* h, int64_t* q)
	{
		this->h = h;
		this->q = q;
	}
};

int main()
{
	/*const char* query = "casa";
	const char* target = "carro";

	EdlibAlignResult result = edlibAlign(query, 4, target, 5, edlibNewAlignConfig(-1, EDLIB_MODE_NW, EDLIB_TASK_PATH, NULL, 0));

	auto endLocations = result.endLocations;


	auto alignment = result.alignment;
	auto alignmentLength = result.alignmentLength;

	printAlignment(query, target, alignment, alignmentLength,
		*(endLocations), EDLIB_MODE_NW);*/

	auto gt = NJ();

	/*gt.init_clusters(3);*/



	vector<string> sequences;
	vector<vector<string>> profiles;
	/*profiles[0].push_back("casa");
	profiles[1].push_back("atr");
	profiles[2].push_back("perro");


	sequences.push_back("casa");
	sequences.push_back("atr");
	sequences.push_back("perro");*/

	auto instance = Tools();

	auto start = clock();

	//gt.init_data(sequences, profiles, "data2.txt");

	size_t c = NJ::load_data(sequences, "data2.txt");

	//gt.init_clusters(c);

	std::cout << "Init Data OK en: ";
	std::cout << static_cast<double>(clock() - start) / static_cast<double>(CLOCKS_PER_SEC) << " seconds." << endl;




	/*string s1 = sequences[23];
	string s2 = sequences[23];*/




	/*char* query = new char[s1.size() + 1];
	char* target = new char[s2.size() + 1];

	copy(s1.begin(), s1.end(), query);
	query[s1.size()] = '\0';
	copy(s2.begin(), s2.end(), target);
	target[s2.size()] = '\0';

	EdlibAlignResult result;
	long cc = 0;*/

	/*string s1 = "holaholaholaholaholaholaholaholaholaholaholaholaholaholaholaholaholaholaholaholaholaholaholaholaholaholaholaholaholaholaholaholaholaholaholaholaholaholaholaholaholaholaholaholaholaholaholaholaholaholaholaholaholaholaholaholaholaholaholaholaholaholaholaholaholahola";
	string s2 = "holaholaholaholaholaholaholaholaholaholaholaholaholaholaholaholaholaholaholaholaholaholaholaholaholaholaholaholaholaholaholaholaholaholaholaholaholaholaholaholaholaholaholaholaholaholaholaholaholaholaholaholaholaholaholaholaholaholaholaholaholaholaholaholaholahola";*/


	/*string s1 = sequences[0];
	string s2 = sequences[1];

	int64_t* query = new int64_t[s1.size() + 1];
	int64_t* target = new int64_t[s2.size() + 1];

	copy(s1.begin(), s1.end(), query);
	query[s1.size()] = '\0';
	copy(s2.begin(), s2.end(), target);
	target[s2.size()] = '\0';*/


	vector<string> sequencess = { "hola","perro","casa" };

	vector<vector<int>> matrix(sequences.size());

	for (size_t i = 0; i < sequences.size(); i++)
		matrix[i].resize(sequences.size());

	omp_set_num_threads(4);

	start = clock();
	//auto cc = edit_distance(query, s1.size(), target, s2.size());;
	int64_t* query = new int64_t[1400];
	int64_t* target = new int64_t[1400];
	//auto* query = new char[52];
	//auto* target = new char[52];
	long cc = 0;
	string s1;
	string s2;
	size_t j_size;

#pragma omp parallel for collapse(2) private(s1,s2)
	for (int i = 0; i < sequences.size(); i++)
		for (j_size = i + 1; j_size < sequences.size(); j_size++)
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
			//cc += foo(query, target, s1.size(), s2.size());

			//int64_t* query = new int64_t[s1.size() + 1];
			//int64_t* target = new int64_t[s2.size() + 1];



			const auto t = edit_distance(query, s1.size(), target, s2.size());
			//const auto t = edlibAlign(query, s1.size(), target, s2.size(), edlibNewAlignConfig(s1.size(), EDLIB_MODE_NW, EDLIB_TASK_PATH, nullptr, 0)).editDistance;
			matrix[i][j_size] = t;
			matrix[j_size][i] = t;


			//cout << static_cast<double>(clock() - start) / static_cast<double>(CLOCKS_PER_SEC) << " seconds." << endl;

		}
	delete[] query;
	delete[] target;
	std::cout << "Distance Matrix OK en: ";
	std::cout << static_cast<double>(clock() - start) / static_cast<double>(CLOCKS_PER_SEC) << " seconds." << endl;


	/*for (size_t i = 0; i < sequencess.size(); i++)
	{
		for (size_t j = 0; j < sequencess.size(); j++)
		{
			cout << matrix[i][j] << " ";

		}
		cout << "\n";
	}*/

	/*auto* queryc = new char[10001];
	auto* targetc = new char[10001];

	move(s1.begin(), s1.end(), queryc);
	queryc[s1.size()] = '\0';
	move(s2.begin(), s2.end(), targetc);
	targetc[s2.size()] = '\0';

	std::cout << std::fixed << std::setprecision(10);
	auto begin = std::chrono::system_clock::now();

	auto dd = edlibAlign(queryc, s1.size(), targetc, s2.size(), edlibNewAlignConfig(s1.size(), EDLIB_MODE_NW, EDLIB_TASK_PATH, nullptr, 0)).editDistance;

	auto last = std::chrono::system_clock::now() - begin;
	auto copyTime = std::chrono::duration<double>(last).count();
	std::cout << "    Edlib = " << dd << " en: " << copyTime << " sec" << std::endl;
	delete[] queryc;
	delete[] targetc;

	s1 = sequences[0];
	s2 = sequences[1];

	move(s1.begin(), s1.end(), query);
	query[s1.size()] = '\0';
	move(s2.begin(), s2.end(), target);
	target[s2.size()] = '\0';

	begin = std::chrono::system_clock::now();

	dd = edit_distance(query, s1.size(), target, s2.size());

	last = std::chrono::system_clock::now() - begin;
	copyTime = std::chrono::duration<double>(last).count();
	std::cout << "    edit_dist = " << dd << " en: " << copyTime << " sec" << std::endl;*/

	//auto m = Tools::calculate_dist_matrix(sequences);

	/*delete[] query;
	delete[] target;*/

	/*
	start = clock();

	gt.run(sequences, m, profiles);

	cout << "Ordenamiento OK en: ";
	cout << static_cast<double>(clock() - start) / static_cast<double>(CLOCKS_PER_SEC) << " seconds." << endl;*/


	//auto a = myersCalcEditDistanceNW

	std::cout << " OK";

}



void printAlignment(const char* query, const char* target,
	const unsigned char* alignment, const int alignmentLength,
	const int position, const EdlibAlignMode modeCode) {
	int tIdx = -1;
	int qIdx = -1;
	if (modeCode == EDLIB_MODE_HW) {
		tIdx = position;
		for (int i = 0; i < alignmentLength; i++) {
			if (alignment[i] != EDLIB_EDOP_INSERT)
				tIdx--;
		}
	}
	for (int start = 0; start < alignmentLength; start += 50) {
		// target
		printf("T: ");
		int startTIdx = -1;
		for (int j = start; j < start + 50 && j < alignmentLength; j++) {
			if (alignment[j] == EDLIB_EDOP_INSERT)
				printf("-");
			else
				printf("%c", target[++tIdx]);
			if (j == start)
				startTIdx = tIdx;
		}
		printf(" (%d - %d)\n", max(startTIdx, 0), tIdx);

		// match / mismatch
		printf("   ");
		for (int j = start; j < start + 50 && j < alignmentLength; j++) {
			printf(alignment[j] == EDLIB_EDOP_MATCH ? "|" : " ");
		}
		printf("\n");

		// query
		printf("Q: ");
		int startQIdx = qIdx;
		for (int j = start; j < start + 50 && j < alignmentLength; j++) {
			if (alignment[j] == EDLIB_EDOP_DELETE)
				printf("-");
			else
				printf("%c", query[++qIdx]);
			if (j == start)
				startQIdx = qIdx;
		}
		printf(" (%d - %d)\n\n", max(startQIdx, 0), qIdx);
	}
}

long long foo(int64_t* a, int64_t* b, size_t ii, size_t jj)
{

	for (size_t i = 0; i < ii; i++)
		for (size_t j = 0; j < jj; j++)
			auto g = b[i];

	auto s = tup(a, b);

	return 1;
}