#pragma once
#include "Tools.h"
#include "edlib.h"

using namespace std;

struct Cluster
{
	int index_i;
	int index_j;
	int sum_distances;
	int size;

};

using tree_structure = queue<Cluster>;

class NJ
{
public:

	tree_structure& raw() { return Tree; }

	void merge(Cluster& a, Cluster& b, int dist_between);

	void run(vector<string>& sequences, vector<vector<int>>& dist_matrix, vector<vector<string>>& profiles);

	void init_clusters(size_t size);

	static void update_profile(vector<vector<string>>& profiles, int id_profile, string s1, string s2, const unsigned char* alignment, int length_alignment);

	static void update_profile_multiple(vector<vector<string>>& profiles, int id_profile1, int id_profile2);

	static void update_dist_matrix(vector<vector<int>>& dist_matrix, int i_id, int value);

	static int score(vector<string> prof1, vector<string> prof2, int row, int col);

	static vector<string> align_alignments(vector<string> profil1, vector<string> profile2);

	static vector<int> build_trace_back(vector<vector<int>> steps_matrix);

	void init_data(vector<string>& sequences, vector<vector<string>>& profiles, string path);

	static size_t load_data(vector<string>& sequences, string file_name);




protected:

	tree_structure Tree;

};



