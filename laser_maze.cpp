#include <iostream>
#include <vector>
#include <list>
#include <map>
#include <set>

using namespace std;

using maze_t = vector<vector<char>>;
using point_t = pair<int, int>;

/*
 *	Pieces
 */
static const char EMPTY_SPACE = '.';
static const char WALL = '#';
static const char S = 'S';
static const char G = 'G';

/*
 *	Turrets
 */
static const char TURRET_LEFT = '<';
static const char TURRET_UP = '^';
static const char TURRET_RIGHT = '>';
static const char TURRET_DOWN = 'v';

/*
 *	Next turret position
 */
static map<char, char> next_face = {
	{TURRET_LEFT, TURRET_UP},
	{TURRET_UP, TURRET_RIGHT},
	{TURRET_RIGHT, TURRET_DOWN},
	{TURRET_DOWN, TURRET_LEFT}
};

/*
 *	Helpers
 */
void print_possible_mazes(const vector<maze_t>& pm) {
	for(auto& m : pm) {
		for(int i = 0; i < m.size();  i++) {
			for(int j = 0; j < m[i].size(); j++) {
				cout << m[i][j] << " " ;
			}
			cout << endl;
		}

		cout << endl << endl;
	}
}

/*
 *	Se o char enviado for TURRET retorna true
 */
bool is_turret(char c) {
	return (c == TURRET_UP || c == TURRET_DOWN || c == TURRET_RIGHT || c == TURRET_LEFT);
}

vector<maze_t> get_possible_mazes(const maze_t& original, const vector<point_t>& turrets) {
	vector<maze_t> possible_mazes;
	for(int i = 0; i < 4; i++) {
		auto maze_copy = original;
		for(auto&t : turrets) {
			char nf;
			if(i == 0)
				nf = maze_copy[t.first][t.second];
			else 
				nf = next_face[possible_mazes[i - 1][t.first][t.second]];
		
			maze_copy[t.first][t.second] = nf;

			switch(nf) {
				case TURRET_UP:
					for(int j = t.first; j >= 0; j--) {
						if(is_turret(maze_copy[j][t.second]))
							continue;

						if(maze_copy[j][t.second] == EMPTY_SPACE)
							maze_copy[j][t.second] = WALL;
					}
					break;

				case TURRET_DOWN:
					for(int j = t.first; j < maze_copy.size(); j++) {
						if(is_turret(maze_copy[j][t.second]))
							continue;

						if(maze_copy[j][t.second] == EMPTY_SPACE)
							maze_copy[j][t.second] = WALL;
					}

					break;

				case TURRET_RIGHT:
					for(int j = t.second; j < maze_copy[0].size(); j++) {
						if(is_turret(maze_copy[t.first][j]))
							continue;

						if(maze_copy[t.first][j] == EMPTY_SPACE)
							maze_copy[t.first][j] = WALL;
					}

					break;

				case TURRET_LEFT:
					for(int j = t.second; j >= 0; j--) {
						if(is_turret(maze_copy[t.first][j]))
							continue;

						if(maze_copy[t.first][j] == EMPTY_SPACE)
							maze_copy[t.first][j] = WALL;
					}

					break;
			}
		}
		possible_mazes.push_back(maze_copy);
	}
	return possible_mazes;
}

/*
 *	Main algorithm function
 */
int solve(maze_t& maze, point_t& start, point_t& end, const vector<point_t>& turrets) {
	list<pair<point_t, int>> execution_queue;
	execution_queue.push_back({start, 0});
	set<pair<point_t, maze_t>> already_computed;

	/*
	 *	Pré-calcula todos os possiveis mazes
	 */
	auto possible_mazes = get_possible_mazes(maze, turrets);

	while(!execution_queue.empty()) {
		/*
		 *	Recebe estado atual
		 */
		auto curr_state = execution_queue.front();
		execution_queue.pop_front();

		/*
		 *	Posição atual do personagem
		 */
		point_t my_position = curr_state.first;

		/*
		 *	Número de execucoes
		 */
		int n_exec = curr_state.second;

		/*
		 *	Se ja computado, skip
		 */

		if(already_computed.find({my_position, possible_mazes[(n_exec) % 4]}) != already_computed.end()) {
			continue;
		}

		/*
		 *	Already Computed state
		 */
		already_computed.insert({my_position, possible_mazes[(n_exec) % 4]});
		
		/*
		 *	Out of bounds
		 */
		if(my_position.first >= maze.size() || my_position.first < 0 || my_position.second >= maze[0].size() || my_position.second < 0) {
			continue;
		}

		/*
		 *	Wall/turret overlap
		 */
		if(possible_mazes[(n_exec) % 4][my_position.first][my_position.second] != EMPTY_SPACE && n_exec != 0) {
			continue;
		}

		/*
		 *	FINAL
		 */
		if(my_position == end) {
			return n_exec;
		}
	
		/*
		 *	Enqueue todos os possiveis movimentos
		 */
		execution_queue.push_back({point_t(my_position.first + 1, my_position.second), n_exec + 1});
		execution_queue.push_back({point_t(my_position.first - 1, my_position.second), n_exec + 1});
		execution_queue.push_back({point_t(my_position.first, my_position.second + 1), n_exec + 1});
		execution_queue.push_back({point_t(my_position.first, my_position.second - 1), n_exec + 1});
	}
	return -1;
}

int main() {
	int T;
	cin >> T;
	int i = 1;
	while(i <= T) {
		int M, N;
		cin >> M >> N;
		maze_t maze(M, vector<char>(N));
		point_t start;
		point_t end;
		vector<point_t> turrets;
		for(int i = 0; i < M; i++) {
			for(int j = 0; j < N; j++) {
				cin >> maze[i][j];
				if(maze[i][j] == S) {
					start.first = i;
					start.second = j;
					maze[i][j] = EMPTY_SPACE;
				} else if(maze[i][j] == G) {
					end.first = i;
					end.second = j;
					maze[i][j] = EMPTY_SPACE;
				} else if(maze[i][j] == TURRET_UP || maze[i][j] == TURRET_RIGHT || maze[i][j] == TURRET_DOWN || maze[i][j] == TURRET_LEFT) {
					turrets.push_back({i, j});
				}
			}
		}

		auto path_length = solve(maze, start, end, turrets);

		if(path_length == -1)
			cout << "Case #" << i << ": impossible" << endl;
		else 
			cout << "Case #" << i << ": " << path_length << endl;
		i++;
	}

	return 0;
}
