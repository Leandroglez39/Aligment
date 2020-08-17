#pragma once

#include "edlib.h"

#include <string>
#include <vector>

using namespace std;

void build_distance_matrix(vector<string>& sequences, vector<vector<pair<string, string>>>& alignments, int& piv);

void msa(vector<vector<pair<string, string>>>& pair_alignments, vector<string>& multi_align, int piv);

string merge_star(string s1, string s2);

pair<string, string> convert_to_vector(int length, const unsigned char* alignment, vector<string>& sequences, int index_q, int index_t);

void parse_alignment(vector<string>& multi_align, int piv, vector<string>& sequences, vector<string>& alig_result);