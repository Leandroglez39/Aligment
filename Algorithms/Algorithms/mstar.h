#pragma once

#include "edlib.h"

#include <string>
#include <vector>

using namespace std;

void build_distance_matrix(vector<string>& sequences, vector<vector<unsigned char*>>& alignments, int& piv);

