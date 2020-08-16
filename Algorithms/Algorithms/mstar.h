#pragma once

#include "edlib.h"

#include <string>
#include <vector>

using namespace std;

void build_distance_matrix(vector<string>& sequences, vector<vector<pair<unsigned char*, int>>>& alignments, int& piv);

void msa(vector<vector<pair<unsigned char*, int>>>& pair_alignments, vector<string>& multi_align, int piv);

string merge_star(vector<vector<pair<unsigned char*, int>>>& pair_alignments, int piv);

