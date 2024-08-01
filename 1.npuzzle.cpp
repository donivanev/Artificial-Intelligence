#include <iostream>
#include <chrono>
#include <vector>
#include <cmath>
#include <map>
#include <stack>
#include <climits>

using namespace std;

typedef vector<vector<int>> matrix;

size_t _SIZE, _ZEROINDEX;

template<class T>
struct Node {
    T data;
    vector<Node<T>*> children;
    Node<T>* prev = nullptr;
    int zeroI, zeroJ;
    char direction;

    Node(T _data, int _zeroI, int _zeroJ, char _direction) : data(_data), zeroI(_zeroI), zeroJ(_zeroJ), direction(_direction) {}

    void printData();
    void printNode();
};

template<class T>
void Node<T>::printData() {
    for (size_t i = 0; i < _SIZE; ++i) {
        for (size_t j = 0; j < _SIZE; ++j) {
            cout << this->data[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl;
}

template<class T>
void Node<T>::printNode() {
    for (size_t i = 0; i < _SIZE; ++i) {
        for (size_t j = 0; j < _SIZE; ++j) {
            cout << this->data[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl;
    
    for (size_t i = 0; i < this->children.size(); ++i) { 
        this->children[i]->printNode();
    }
}

template<class T>
void clear(Node<T> *&node) {
    if (!node) {
        return;
    }

    for (size_t i = 0; i < node->children.size(); ++i) {
        clear(node->children[i]);
    }

    node->prev = nullptr;
    delete node;
    node = nullptr;
}

template<class T>
void buildChildren(Node<T> *node) {
    if (node->zeroI - 1 >= 0) {       // up
        T up = node->data;
        up[node->zeroI][node->zeroJ] = up[node->zeroI - 1][node->zeroJ];
        up[node->zeroI - 1][node->zeroJ] = 0;

        Node<T> *upNode = new Node<T>(up, node->zeroI - 1, node->zeroJ, 'd');

        if (node->prev == nullptr || node->prev->data != upNode->data) {
            upNode->prev = node;
            node->children.push_back(upNode);
        }
    }

    if (node->zeroI + 1 < _SIZE) {    // down
        T down = node->data;
        down[node->zeroI][node->zeroJ] = down[node->zeroI + 1][node->zeroJ];
        down[node->zeroI + 1][node->zeroJ] = 0;

        Node<T> *downNode = new Node<T>(down, node->zeroI + 1, node->zeroJ, 'u');

        if(node->prev == nullptr || node->prev->data != downNode->data) {
            downNode->prev = node;
            node->children.push_back(downNode);
        }
    }

    if (node->zeroJ - 1 >= 0) {       // left
        T left = node->data;
        left[node->zeroI][node->zeroJ] = left[node->zeroI][node->zeroJ - 1];
        left[node->zeroI][node->zeroJ - 1] = 0;

        Node<T> *leftNode = new Node<T>(left, node->zeroI, node->zeroJ - 1, 'r');

        if(node->prev == nullptr || node->prev->data != leftNode->data) {
            leftNode->prev = node;
            node->children.push_back(leftNode);
        }
    }

    if (node->zeroJ + 1 < _SIZE) {     // right
        T right = node->data;
        right[node->zeroI][node->zeroJ] = right[node->zeroI][node->zeroJ + 1];
        right[node->zeroI][node->zeroJ + 1] = 0;

        Node<T> *rightNode = new Node<T>(right, node->zeroI, node->zeroJ + 1, 'l');

        if(node->prev == nullptr || node->prev->data != rightNode->data) {
            rightNode->prev = node;
            node->children.push_back(rightNode);
        }
    }
}

template<class T>
int manhattan(Node<T> *node) {
    int distance = 0;

    for (int i = 0; i < _SIZE; ++i) {
        for (int j = 0; j < _SIZE; ++j) {
            if (node->data[i][j] != 0) {
                int boardVal = node->data[i][j];

                if (i * _SIZE + j <= _ZEROINDEX) {
                    boardVal -= 1;
                }

                int rowVal = (boardVal / _SIZE) - i;
                int colVal = (boardVal % _SIZE) - j;
                distance += abs(rowVal) + abs(colVal);
            }
        }
    }

    return distance;
}

template<class T>
T goalNode() {
    T goalNode;
    int count = 1;
    int index = _ZEROINDEX;

    for (size_t i = 0; i < _SIZE; ++i) {
        vector<int> row;

        for (size_t j = 0; j < _SIZE; ++j) {
            if (index == 0) {
                row.push_back(0);
            }
            else {
                row.push_back(count);
                ++count;
            }
            --index;
        }

        goalNode.push_back(row);
    }
    
    return goalNode;
}

template<class T>
int iterative_deepening_a_star(Node<T> *node, const T& goal, int distance, const int& threshold, bool& flag) {
    int estimate = distance + manhattan(node);

    if (estimate > threshold) {
        return estimate;
    }

    if (node->data == goal) {
        flag = true;

        stack<char> s;
        Node<T> *crr = node;

        while (crr != nullptr) {
            s.push(crr->direction);
            crr = crr->prev;
        }

        s.pop(); // remove root
        cout << distance << endl;

        while (!s.empty()) {
            char direction = s.top();
            s.pop();
            
            if (direction == 'u') {
                cout << "up" << " ";
            }
            else if (direction == 'd') {
                cout << "down" << " ";
            }
            else if (direction == 'l') {
                cout << "left" << " ";
            }
            else if (direction == 'r') {
                cout << "right" << " ";
            }
        }
        cout << endl;

        return distance;
    }

    int min = INT_MAX;
    buildChildren(node);

    for (size_t i = 0; i < node->children.size(); ++i) {
        int t = iterative_deepening_a_star(node->children[i], goal, distance + 1, threshold, flag);

        if (flag) {
            return t;
        }
        else if (t < min) {
            min = t;
        }
    }

    return min;
}

int main() {
  /*
    // 8 -1 7 2 4 5 0 6 8 3 1 -> 20
    // 8 -1 1 2 3 4 5 6 7 0 8 -> 1
    // 8 -1 1 2 3 4 5 6 0 7 8 -> 2
    // 8 -1 1 8 2 0 4 3 7 6 5 -> 9
    // 15 -1 1 2 3 4 5 6 0 8 9 10 7 11 13 14 15 12 -> 3
    // 8 -1 0 1 3 4 2 5 7 8 6 -> 4
    // 8 -1 6 5 3 2 4 8 7 0 1 -> 21
    // 15 -1 0 1 2 3 5 6 7 4 9 10 11 8 13 14 15 12 -> 6
    // 15 12 0 1 2 3 6 7 8 4 5 9 10 11 13 14 15 12 -> 15
    // 8 -1 5 6 2 7 4 0 8 1 3 -> 21
    // 8 -1 2 0 1 8 4 3 7 6 5 -> 15
    // 8 -1 6 8 7 5 3 2 4 0 1 -> 27
  */

    size_t N;
    cin >> N; 
    if (!cin) { return -1; }

    _SIZE = sqrt(N + 1);
    
    cin >> _ZEROINDEX; // from 0 to n

    if (!cin) { return -1; }
    if (_ZEROINDEX == -1) { _ZEROINDEX = N; }

    matrix board;
    int zeroI, zeroJ;

    for (size_t i = 0; i < _SIZE; ++i) {
        vector<int> row;

        for (size_t j = 0; j < _SIZE; ++j) {
            int value;
            cin >> value;

            if (!cin) { return -1; }

            if (value == 0) {
                zeroI = i;
                zeroJ = j;
            }

            row.push_back(value);
        }

        board.push_back(row);
    }
    
    Node<matrix> *root = new Node<matrix>(board, zeroI, zeroJ, 'o');

    // ida*
    int threshold = manhattan<matrix>(root);
    int tmp = 0;
    bool flag = false;

    auto start = chrono::high_resolution_clock::now();

    while (!flag && tmp != INT_MAX) {
        tmp = iterative_deepening_a_star<matrix>(root, goalNode<matrix>(), 0, threshold, flag);
        threshold = tmp;
    }

    auto stop = chrono::high_resolution_clock::now();
    cout << chrono::duration<double>(stop - start).count() << " seconds" << endl;

    clear<matrix>(root);

    return 0;
}