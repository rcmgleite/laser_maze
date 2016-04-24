#include <iostream>
#include <vector>
#include <list>
#include <map>
#include <set>

using namespace std;
using maze_t = vector<vector<char>>;

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
	{'<', '^'},
	{'^', '>'},
	{'>', 'v'},
	{'v', '<'}
};

/*
 *	Simple point struct
 */
struct point_t {
	point_t(int x, int y) : _x(x), _y(y) {}
	point_t() = default;

	point_t& operator=(const point_t& rhs) {
		_x = rhs._x;
		_y = rhs._y;
	}
	point_t(const point_t& p) = default;

	int _x;
	int _y;

	bool operator==(const point_t& rhs) const {
		return (_x == rhs._x && _y == rhs._y);
	}

	bool operator<(const point_t& rhs) const {
		return _x < rhs._x;
	}

};

/*
 *	Main algorithm function
 */
void solve(maze_t& maze, point_t& start, point_t& end, const vector<point_t>& turrets) {
	list<pair<point_t, maze_t>> execution_queue;
	execution_queue.push_back({start, maze});
	
//	set<pair<point_t, maze_t>> already_computed;
	
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
		 *	Condição atual do mapa
		 */
		maze_t current_maze = curr_state.second;

		/*
		 *	Se ja computado, skip
		 */
		cout << my_position._x << ", " << my_position._y << endl;
	//	if(already_computed.find({my_position, current_maze}) != already_computed.end()) {
		//	cout << "already computed" << endl;
			//continue;
		//}

		/*
		 *	Already Computed state
		 */
		//already_computed.insert({my_position, current_maze});
		
		/*
		 *	Out of bounds
		 */
		if(my_position._x >= current_maze.size() || my_position._x < 0 || my_position._y >= current_maze[0].size() || my_position._y < 0) {
			continue;
		}

		/*
		 *	Wall/turret overlap
		 */
		if(current_maze[my_position._x][my_position._y] == WALL || current_maze[my_position._x][my_position._y] == TURRET_UP ||
				current_maze[my_position._x][my_position._y] == TURRET_DOWN || current_maze[my_position._x][my_position._y] == TURRET_LEFT ||
				current_maze[my_position._x][my_position._y] == TURRET_RIGHT) {
			continue;
		}

		if(my_position == end) {
			cout << "DEU, MANOLO!" << endl;
			return;
		}

		/*
		 *	Se está na mira de um turret, fodeu!
		 */ 
		bool dead = false;
		for(auto& t : turrets) {
			auto turret_face = current_maze[t._x][t._y];

			switch(turret_face) {
				case TURRET_LEFT:
					if(my_position._x < t._x && my_position._y == t._y)
						dead = true;
					break;

				case TURRET_RIGHT:
					if(my_position._x > t._x && my_position._y == t._y)
						dead = true;
					break;

				case TURRET_DOWN:
					if(my_position._y < t._y && my_position._x == t._x)
						dead = true;
					break;

				case TURRET_UP:
					if(my_position._y > t._y && my_position._x == t._x)
						dead = true;
					break;
			}

			/*
			 *	Se nao morreu, precisamos tentar colocar todas as outras possibilidades de mapa... atualizando turrets e a posição
			 *		do personagem
			 */
			if(!dead) {
				/*
				 *	Próximo maze
				 */
				for(auto&t : turrets) {
					current_maze[t._x][t._y] = next_face[current_maze[t._x][t._y]];
				}

				/*
				 *	Empilha todos os possiveis movimentos
				 */
				execution_queue.push_back({point_t(my_position._x + 1, my_position._y), current_maze});
				execution_queue.push_back({point_t(my_position._x - 1, my_position._y), current_maze});
				execution_queue.push_back({point_t(my_position._x, my_position._y + 1), current_maze});
				execution_queue.push_back({point_t(my_position._x, my_position._y - 1), current_maze});
			} 
		}
	}
	cout << "Impossível..." << endl;
}

int main() {
	int T;
	cin >> T;
	while(T) {
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
					start._x = i;
					start._y = j;
				} else if(maze[i][j] == G) {
					end._x = i;
					end._y = j;
				} else if(maze[i][j] == TURRET_UP || maze[i][j] == TURRET_RIGHT || maze[i][j] == TURRET_DOWN || maze[i][j] == TURRET_LEFT) {
					turrets.push_back({i, j});
				}
			}
		}

		solve(maze, start, end, turrets);
		T--;
	}

	return 0;
}
