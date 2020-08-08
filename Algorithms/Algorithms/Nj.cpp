#include "Nj.h"
#include "normal-NW.h"


#include <fstream>
#include <iostream>
#include <utility>
#include "Input.h"


void NJ::merge(Cluster& a, Cluster& b, int dist_between)
{
	auto& cluster = a;

	cluster.size = a.size + b.size;
	cluster.sum_distances = a.sum_distances + b.sum_distances + dist_between;

	this->Tree.push(cluster);


}

void NJ::run(vector<string>& sequences, vector<vector<int>>& dist_matrix, vector<vector<string>>& profiles)
{

	char* query = new char[1400];
	char* target = new char[1400];
	string s1;
	string s2;
	vector<int> result;


	while (this->Tree.size() != 1)
	{

		//cout << "while\n";
		if (this->Tree.size() >= 2)
		{
			auto& c1 = this->Tree.front();
			this->Tree.pop();
			auto& c2 = this->Tree.front();
			this->Tree.pop();


			s1 = sequences[c1.index_i];
			s2 = sequences[c2.index_i];


			if (c1.size == 1 && c2.size == 1)
			{


				/*move(s1.begin(), s1.end(), query);
				query[s1.size()] = '\0';
				move(s2.begin(), s2.end(), target);
				target[s2.size()] = '\0';*/

				//result = edlibAlign(query, sequences[c1.index_i].size(), target, sequences[c2.index_i].size(), edlibNewAlignConfig(-1, EDLIB_MODE_NW, EDLIB_TASK_PATH, nullptr, 0));

				result = parallel_needleman_wunsch(s1, s2, s1.size(), s2.size(), 1, -1, -1);

				//cout << "Muero en Merge \n";
				merge(c1, c2, dist_matrix[c1.index_i][c2.index_i]);

				//cout << "Muero en update dist matrix\n";
				update_dist_matrix(dist_matrix, c1.index_i, dist_matrix[c1.index_i][c2.index_i]);

				//cout << "Muero en update profile\n";
				//cout << c1.index_i << "-" << s1 << " " << s2 << "\n";
				update_profile(profiles, c1.index_i, s1, s2, result, result.size());

				//edlibFreeAlignResult(result);

			}
			else
			{
				//cout << "Muero en merge multi\n";
				merge(c1, c2, dist_matrix[c1.index_i][c2.index_i]);

				//cout << "Muero en update dist matrix multi\n";
				update_dist_matrix(dist_matrix, c1.index_i, dist_matrix[c1.index_i][c2.index_i]);

				//cout << "Muero en update profile multi\n";
				update_profile_multiple(profiles, c1.index_i, c2.index_i);
			}
		}

	}

	delete[] query;
	delete[] target;


}

void NJ::init_clusters(size_t size)
{
	auto c1 = Cluster();


	for (int i = 0; i < size; i++)
	{
		c1.sum_distances = 0;
		c1.size = 1;
		c1.index_i = i;
		c1.index_j = i;

		this->Tree.push(c1);
	}
}

void NJ::update_profile2(vector<vector<string>>& profiles, int id_profile, string s1, string s2,
	const unsigned char* alignment, int length_alignment)
{
	vector<string> profile;

	string query;
	string target;
	int s1_index = 0;
	int s2_index = 0;



	for (auto i = 0; i < length_alignment; i++)
	{
		switch (alignment[i])
		{
		case 1:
		{
			query.push_back(s1[s1_index]);
			target.push_back('-');
			s1_index++;
			break;
		}
		case 2:
		{
			target.push_back(s2[s2_index]);
			query.push_back('-');
			s2_index++;
			break;
		}
		default:
		{
			query.push_back(s1[s1_index]);
			target.push_back(s2[s2_index]);
			s1_index++;
			s2_index++;
		}
		}
	}

	profile.push_back(target);
	profile.push_back(query);

	profiles[id_profile] = profile;

}

void NJ::update_profile(vector<vector<string>>& profiles, int id_profile, string s1, string s2,
	vector<int> alignment, int length_alignment)
{
	vector<string> profile;

	string query;
	string target;
	int s1_index = 0;
	int s2_index = 0;



	for (auto i = 0; i < length_alignment; i++)
	{
		switch (alignment[i])
		{
		case 1:
		{
			query.push_back(s1[s1_index]);
			target.push_back('-');
			s1_index++;
			break;
		}
		case 2:
		{
			target.push_back(s2[s2_index]);
			query.push_back('-');
			s2_index++;
			break;
		}
		default:
		{
			query.push_back(s1[s1_index]);
			target.push_back(s2[s2_index]);
			s1_index++;
			s2_index++;
		}
		}
	}

	profile.push_back(target);
	profile.push_back(query);

	profiles[id_profile] = profile;

}

void NJ::update_profile_multiple(vector<vector<string>>& profiles, int id_profile1, int id_profile2)
{
	vector<string> profile = profiles[id_profile2][0].size() > profiles[id_profile1][0].size()
		? align_alignments(profiles[id_profile2], profiles[id_profile1])
		: align_alignments(profiles[id_profile1], profiles[id_profile2]);

	profiles[id_profile1] = profile;


}

void NJ::update_dist_matrix(vector<vector<int>>& dist_matrix, int i_id, int value)
{
	for (int i = 0; i < dist_matrix[i_id].size(); i++)
	{
		if (i != i_id)
		{
			dist_matrix[i][i_id] = dist_matrix[i][i_id] + value;
			dist_matrix[i_id][i] = dist_matrix[i_id][i] + value;
		}
	}
}

int NJ::score(vector<string> prof1, vector<string> prof2, int row, int col)
{
	int score = 0;

	if (prof1.size() > 2 && prof2.size() > 2)
	{
		score += prof1[0][row] == prof2[0][col] ? 1 : -2;
		score += prof1[0][row] == prof2[static_cast<int>(prof2.size() / 2)][col] ? 1 : -2;
		score += prof1[0][row] == prof2[prof2.size() - 1][col] ? 1 : -2;

		score += prof1[prof1.size() - 1][row] == prof2[0][col] ? 1 : -2;
		score += prof1[prof1.size() - 1][row] == prof2[static_cast<int>(prof2.size() / 2)][col] ? 1 : -2;
		score += prof1[prof1.size() - 1][row] == prof2[prof2.size() - 1][col] ? 1 : -2;

		score += prof1[static_cast<int>((prof1.size() - 1) / 2)][row] == prof2[0][col] ? 1 : -2;
		score += prof1[static_cast<int>((prof1.size() - 1) / 2)][row] == prof2[static_cast<int>(prof2.size() / 2)][col] ? 1 : -2;
		score += prof1[static_cast<int>((prof1.size() - 1) / 2)][row] == prof2[prof2.size() - 1][col] ? 1 : -2;


	}
	else
	{
		score += prof1[0][row] == prof2[0][col] ? 1 : -1;
		score += prof1[0][row] == prof2[prof2.size() - 1][col] ? 1 : -1;

		score += prof1[static_cast<int>((prof1.size() - 1) / 2)][row] == prof2[0][col] ? 1 : -1;
		score += prof1[static_cast<int>((prof1.size() - 1) / 2)][row] == prof2[prof2.size() - 1][col] ? 1 : -1;

	}
	return score;
}

vector<string> NJ::align_alignments(vector<string> profil1, vector<string> profile2)
{



	auto size1 = profil1[0].size();
	auto size2 = profile2[0].size();

	vector<string> result(profile2.size() + profil1.size());

	vector<vector<int>> matrix(size1 + 1);

	vector<vector<int>> step_trace_back(size1 + 1);

	vector<int>trace_back;


	// ReSharper disable once CppUseAuto
	for (size_t i = 0; i < size1 + 1; i++)
	{
		matrix[i].resize(size2 + 1);
		step_trace_back[i].resize(size2 + 1);
	}

	matrix[0][0] = 0;
	step_trace_back[0][0] = 0;

	for (size_t i = 1; i < size1 + 1; i++)
	{
		matrix[i][0] = i * -1;
		step_trace_back[i][0] = 1;

		if (i <= size2)
		{
			matrix[0][i] = i * -1;
			step_trace_back[0][i] = 2;
		}
	}



	int aux[3];

	for (size_t i = 1; i < size1 + 1; i++)
		for (size_t j = 1; j < size2 + 1; j++)
		{


			aux[0] = score(profil1, profile2, i - 1, j);
			aux[1] = score(profil1, profile2, i, j - 1);
			aux[2] = score(profil1, profile2, i - 1, j - 1);


			if (aux[2] >= aux[1])
			{
				if (aux[2] >= aux[0])
				{
					matrix[i][j] = matrix[i - 1][j - 1] + aux[2];

					step_trace_back[i][j] = 0;
				}
				else
				{
					matrix[i][j] = matrix[i - 1][j] + aux[0];

					step_trace_back[i][j] = 1;
				}
			}
			else
			{
				if (aux[1] >= aux[0])
				{
					matrix[i][j] = matrix[i][j - 1] + aux[1];

					step_trace_back[i][j] = 2;
				}
				else
				{
					matrix[i][j] = matrix[i - 1][j] + aux[0];

					step_trace_back[i][j] = 1;
				}
			}




		}

	trace_back = build_trace_back(step_trace_back);

	int x = 0;
	int y = 0;


	for (size_t i = 0; i < trace_back.size(); i++)
	{
		switch (trace_back[i])
		{
		case 1:
		{

			for (size_t j = 0; j < result.size(); j++)
			{
				if (j < profil1.size())
				{
					result[j].push_back(profil1[j][x]);
				}
				else
				{

					result[j].push_back('-');
				}

			}
			x++;
			break;
		}

		case 2:
		{

			for (size_t j = 0; j < result.size(); j++)
			{
				if (j < profil1.size())
				{
					result[j].push_back('-');
				}
				else
				{

					result[j].push_back(profile2[j - profil1.size()][y]);
				}

			}
			y++;
			break;;
		}

		default:
		{
			for (size_t j = 0; j < result.size(); j++)
			{
				if (j < profil1.size())
				{
					result[j].push_back(profil1[j][x]);
				}
				else
				{

					result[j].push_back(profile2[j - profil1.size()][y]);
				}

			}
			x++;
			y++;

		}
		}
	}

	return  result;
}


vector<int> NJ::build_trace_back(vector<vector<int>> steps_matrix)
{
	vector<int> trace;

	size_t i = steps_matrix.size() - 1;
	size_t j = steps_matrix[0].size() - 1;

	while (i != 0 || j != 0)
	{
		switch (steps_matrix[i][j])
		{
		case 1:
		{

			trace.push_back(steps_matrix[i][j]);
			i--;
			break;
		}
		case 2:
		{
			trace.push_back(steps_matrix[i][j]);
			j--;
			break;
		}
		default:
		{
			trace.push_back(steps_matrix[i][j]);
			i--;
			j--;
		}
		}
	}

	reverse(trace.begin(), trace.end());

	return trace;
}

void NJ::init_data(vector<string>& sequences, vector<vector<string>>& profiles, string path)
{

	auto& text = sequences;
	auto& profile = profiles;

	auto size = load_data(text, path);

	profile.resize(size);

#pragma omp parallel for
	for (int i = 0; i < size; i++)
		profile[i].push_back(text[i]);




}

size_t NJ::load_data(vector<string>& sequences, string file_name)
{

	istream* in;
	ifstream infile;

	if (file_name == "STDIN") {
		in = &cin;
	}
	else {
		infile.open(file_name.c_str(), ios_base::in);
		if (!infile.good())
			return 0;
		in = &infile;
	}

	string s;
	string seq;

	while (in->good())
	{
		getline(*in, s);
		while (!s.empty() && (s[s.length() - 1] == '\n' || s[s.length() - 1] == '\r'))
			s.pop_back();
		if (s.empty())
			continue;

		seq += s;
		sequences.push_back(seq);
		seq.clear();
	}

	if (!seq.empty())
		sequences.push_back(seq);

	return sequences.size();
}