#pragma once
#include <queue>
#include <string>
#include <vector>


using namespace std;

vector<vector<int>> needleman_wunsch(const string& s1, const string& s2, const int size, const int match, const int dismatch, const int gap);

vector<string> back_trace(const string& s1, const string& s2, vector<vector<int>> matrix, int size);

vector<vector<int>> needleman_wunsch_parallel(const string& s1, const string& s2, const int size, const int match, const int dismatch, const int gap);

vector<vector<pair<int, int>>> diagonal_order(int row, int col);

vector<int> parallel_needleman_wunsch(const string& s1, const string& s2, const int size_r, const int size_c, const int match, const int dismatch, const int gap);