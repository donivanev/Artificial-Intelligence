#include <iostream>
#include <vector>
#include <ctime>
#include <chrono>
#include <climits>
#include <cmath>
#include <iomanip>

using namespace std;

vector<int> queens;			// 0 to N-1
vector<int> queensPerRow;	// queens[col] = row
vector<int> queensPerD1;	// '\'
vector<int> queensPerD2;    // '/'

size_t N;

void printQueens() {
	for (size_t i = 0; i < N; ++i) {
		for (size_t j = 0; j < queens[i]; ++j) {
			cout << "- ";
		}

		cout << "* ";

		for (size_t j = 0; j < N - queens[i] - 1; ++j) {
			cout << "- ";
		}

		cout << endl;
	}
}

void removeQueen(int col) {
	queensPerRow[queens[col]] -= 1;
	queensPerD1[(N - 1) + (col - queens[col])] -= 1;
	queensPerD2[col + queens[col]] -= 1;
}

void placeQueen(int col, int row) {
	queens[col] = row;
	queensPerRow[queens[col]] += 1;
	queensPerD1[(N - 1) + (col - queens[col])] += 1;
	queensPerD2[col + queens[col]] += 1;
}

// each queen to every other -> (N * (N-1)) / 2
int countConflictsCol(int col) {
	return  (((queensPerRow[queens[col]] - 1) * queensPerRow[queens[col]]) / 2) + 
			(((queensPerD1[N - 1 + (col - queens[col])] - 1) * queensPerD1[N - 1 + (col - queens[col])]) / 2) + 
			(((queensPerD2[col + queens[col]] - 1) * queensPerD2[col + queens[col]]) / 2);
}

// each queen to every other -> (N * (N-1)) / 2
int countConflictsRow(int row, int maxConfCol) {
	return (((queensPerRow[row] + 1) * queensPerRow[row]) / 2) +
		   (((queensPerD1[(N - 1) + (maxConfCol - row)] + 1) * queensPerD1[(N - 1) + (maxConfCol - row)]) / 2) + 
		   (((queensPerD2[row + maxConfCol] + 1) * queensPerD2[row + maxConfCol]) / 2);
}

int maxConflictsCol() {
	int max = 0, tmp = 0;
	vector<int> maxConflictsCol;

	for (size_t i = 0; i < N; ++i) {
		tmp = countConflictsCol(i);

		if (tmp > max) {
			max = tmp;
			maxConflictsCol.clear();
			maxConflictsCol.push_back(i);
		}
		else if (tmp == max) {
			maxConflictsCol.push_back(i);
		}
	}

	return maxConflictsCol[rand() % maxConflictsCol.size()];
}

int minConflictsRow(int maxConfCol) {
	int min = INT_MAX, tmp = 0;
	vector<int> minConflictsRow;

	removeQueen(maxConfCol);

	for (size_t i = 0; i < N; ++i) {
		tmp = countConflictsRow(i, maxConfCol);

		if (tmp < min) {
			min = tmp;
			minConflictsRow.clear();
			minConflictsRow.push_back(i);
		}
		else if (tmp == min) {
			minConflictsRow.push_back(i);
		}
	}

	return minConflictsRow[rand() % minConflictsRow.size()];
}

void initialise() {
	queens.resize(N);
	queensPerRow.resize(N);
	queensPerD1.resize(2 * N - 1);
	queensPerD2.resize(2 * N - 1);

	for (size_t i = 0; i < 2 * N - 1; ++i)  {
		if (i < N) {
			queensPerRow[i] = 0;
		}
		queensPerD1[i] = 0;
		queensPerD2[i] = 0;
	}

	vector<int> minConflictsRow;
	int min = INT_MAX, tmp = 0;

	// place first queen randomly
	placeQueen(0, rand() % N);

	// i -> col
	for (size_t i = 1; i < N; ++i) {
		minConflictsRow.clear();
		min = INT_MAX;

		// j -> row
		for (size_t j = 0; j < N; ++j) {
			tmp = countConflictsRow(j, i);

			if (tmp < min) {
				min = tmp;
				minConflictsRow.clear();
				minConflictsRow.push_back(j);
			}
			else if (tmp == min) {
				minConflictsRow.push_back(j);
			}
		}

		placeQueen(i, minConflictsRow[rand() % minConflictsRow.size()]);
	}
}

bool hasConflicts() {
	return countConflictsCol(maxConflictsCol()) != 0;
}

int main() {

	cin >> N;

	if (!cin || (N < 4 && N != 1)) {
		std::cout << -1 << std::endl;
		return 0;
	}
	
	auto start = chrono::high_resolution_clock::now();

	srand(time(0));
	initialise();

	auto stop = chrono::high_resolution_clock::now();
    cout << chrono::duration<double>(stop - start).count() << " seconds" << endl;

	if (N <= 100) {
		cout << "Initial board:" << endl;
		printQueens();
	}

	int count = 0, col = 0, row = 0;

	start = chrono::high_resolution_clock::now();

	while (hasConflicts()) {
		col = maxConflictsCol();
		row = minConflictsRow(col);
		placeQueen(col, row);

		++count;

		if (count >= N) {
			int resetCol = rand() % N;
			removeQueen(resetCol);

			int resetRow = rand() % N;
			placeQueen(resetCol, resetRow);
			
			count = 0;
		}
	}

	stop = chrono::high_resolution_clock::now();
    cout << chrono::duration<double>(stop - start).count() << " seconds" << endl;
	//cout << setprecision(2) << chrono::duration<double>(stop - start).count();
	cout << ceil(chrono::duration<double>(stop - start).count() * 100.0) / 100.0;

	if (N <= 100) {
		cout << "result:" << endl;
		printQueens();
	}

	return 0;
}