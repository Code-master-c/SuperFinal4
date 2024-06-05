#include <vector>
#include <stdint.h>
#include <iostream>
#include <bitset>
#include <sstream>
#include <stack>

enum Cell {
	Wall = 1,
	Empty = 0
};

std::vector<std::pair<int, int>> movements = {
	{1,0},
	{-1,0},
	{0,1},
	{-1,0}
};

// Обход по часовой
std::vector<std::pair<int, int>> all_dir = {
	{0,1},
	{1,1},
	{1, 0},
	{1, -1},
	{0,-1},
	{-1,-1},
	{-1,0},
	{-1,1},
};

class Field {
public:
	Field(size_t rows, size_t cols):
	m_rows_num(rows), m_cols_num(cols) {
		m_field.resize(m_rows_num);
		for (auto& row : m_field) {
			row.resize(m_cols_num);
			for (auto& el : row)
				el = Wall;
		}

		generate(rand() % (m_rows_num - 2), 0);
		generate(rand() % (m_rows_num - 2), m_cols_num - 1);

	}
	
	bool valid_coord(int i, int j) {
		if (i <= 0 || j <= 0 || i >= m_rows_num -1  || j >= m_cols_num - 1)
			return false;
	}

	bool available(size_t i, size_t j) {
		if (!valid_coord(i,j))
			return false;
		size_t empty = 0; // сколько пустых поряд при обходе против часовой вокруг (i,j)
		if (!m_field[i][j])
			return false;
		for (auto& [dx, dy] : all_dir) {
			if (valid_coord(i + dx, j + dy) && !m_field[i + dx][j + dy])
				++empty;
			if (empty >= 3)
				return false;
		}
		return true;
		
	}

	std::vector<std::pair<int, int>> get_available(size_t i, size_t j) {
		std::vector<std::pair<int, int>> res;
		for (auto& m : movements) {
			auto p = std::make_pair(m.first + i, m.second + j);
			if (available(p.first, p.second))
				res.push_back(p);
		}
		return res;
	}

	void generate(size_t i, size_t j) {
		m_field[i][j] = Empty;
		size_t ci = i;
		size_t cj = j;

		std::stack<std::pair<size_t, size_t>> path;

		path.push({ ci, cj });
		while (!path.empty()) {
			std::tie(ci, cj) = path.top();
			auto av = get_available(ci, cj);
			while (!av.empty()) {
				std::tie(ci, cj) = av[rand() % av.size()];
				path.push({ ci, cj });
				m_field[ci][cj] = Empty;
				av = get_available(ci, cj);
			}
			path.pop();
		}
	}

	void diplay() {
		for (auto& r : m_field) {
			for (auto& l : r) {
				if (l == Cell::Wall)
					std::cout << "#";
				else
					std::cout << " ";
			}
			std::cout << std::endl;
		}
	}



private:
	std::vector<std::vector<Cell>> m_field;
	size_t m_rows_num;
	size_t m_cols_num;
};

int main(int argc, char* argv[]) {

	if (argc == 3) {
		std::stringstream ss;
		ss << argv[1] << " " << argv[2];
		int n, m;
		ss >> n;
		ss >> m;
		Field f(n, m);
		f.diplay();
	}
	else {
		Field f(20, 25);
		f.diplay();
	}


}