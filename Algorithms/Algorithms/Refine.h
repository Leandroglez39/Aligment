
#include <map>
#include <string>
#include <vector>

using namespace std;
void refine(vector<string>& alignment, map<char, pair<int, vector<int>>>& dic);

void update_alignment(vector<string>& alignment, map<char, pair<int, vector<int>>>& dic, int col);

void erase_gaps(vector<string>& alignments);


void block_shift_refinement(vector<string>& alignment);

bool col_to_swap(vector<char>& col_asign, int init, int end, char val, int& pos);