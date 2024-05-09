#ifndef SNEK_H
#define SNEK_H

#include <vector>
#include <set>
#include <iostream>
#include <map>
#include <deque>
#include <iterator>
#include <climits>
#include <algorithm>
#include <cfloat>
#include <string>

class State {
    public:
    int danger = 0; // Binary encoding (front, left, right);
    int dir = 0; // Direction
    int food = 0; // Binary encoding (front, left, right)
    int foodlarge = 0;
    int proximity = 0;
    
    State(int da2, int d2, int f2, int fl, int p) {
        dir = d2;
        danger = da2;
        food = f2;
        foodlarge = fl;
        proximity = p;
    }
    
    bool operator<(const State& b) const {
        std::vector<int> v1 = {danger, dir, food, foodlarge, proximity};
        std::vector<int> v2 = {b.danger, b.dir, b.food, b.foodlarge, b.proximity};
        return v1 < v2;
    }
};

class SnakeGame {
    public:
    int width, height;
    std::vector<std::vector<int>> grid;
    
    std::deque<std::pair<int, int>> snake; // Front is head of the snake, back is tail of the snake
    std::set<std::pair<int, int>> walls;
    
    std::map<State, std::vector<double>> QTable;
    
    double epsilon = 0.1; // Probability of exploration
    double gamma = 0.7; // Discount factor
    double alpha = 0.7; // Learning rate when updating the QTable.
    
    const int FOOD = 1;
    const int WALL = 2;
    
    int dx[4] = {01, 00, -1, 00};
    int dy[4] = {00, 01, 00, -1};
    std::pair<int, int> dz[4] = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}};
    int FLR[3] = {0, 1, 3};
    
    int prevmove = 0; // Previous movement direction
    int score = 0;
    
    // constants
    
    const int DANGER = -20;
    const int EAT = 10;
    
    SnakeGame(int n, int m) {
        height = n;
        width = m;
        grid = std::vector<std::vector<int>>(n, std::vector<int>(m, 0));
        snake.push_front({n>>1, m>>1});
        
        prevmove = rand() % 4;
    }
    
    SnakeGame() {
        height = 16;
        width = 32;
        grid = std::vector<std::vector<int>>(height, std::vector<int>(width, 0));
        snake.push_front({8, 16});
        
        prevmove = rand() % 4;
    }
    
    double getQ(State s, int a) {
        if (QTable.find(s) != QTable.end()) return QTable.at(s)[a];
        return 0;
    }
    
    void setQ(State s, int a, double v) {
        if (QTable.find(s) == QTable.end()) QTable.insert({s, std::vector<double>(4, 0)});
        std::vector<double> value = QTable.at(s);
        QTable.erase(QTable.find(s));
        value[a % value.size()] = v;
        QTable.insert({s, value});
    }
    
    bool isInSnake(std::pair<int, int> value) {
        if (snake.size() <= 0) {
            return false;
        }
        auto itr = find(snake.begin(), snake.end(), value);
        return itr != snake.end();
    }
    
    bool inBounds(std::pair<int, int> p) {
        if (p.first < 0 || p.second < 0) return false;
        if (p.first >= height || p.second >= width) return false;
        return true;
    }
    
    bool danger(std::pair<int, int> p) {
        if (!inBounds(p)) return true;
        return isInSnake(p);
    }
    
    std::pair<int, int> getClosestFood(std::pair<int, int> p) {
        int mindist = INT_MAX;
        std::pair<int, int> closest = p;
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                if (grid[i][j] != FOOD) continue;
                int dist = abs(p.first - i) + abs(p.second - j);
                if (dist < mindist) {
                    closest = {i, j};
                    mindist = dist;
                }
            }
        }
        return closest;
    }
    
    int getClosestDist(std::pair<int, int> p) {
        int mindist = INT_MAX;
        std::pair<int, int> closest = p;
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                if (grid[i][j] != FOOD) continue;
                int dist = abs(p.first - i) + abs(p.second - j);
                if (dist < mindist) {
                    closest = {i, j};
                    mindist = dist;
                }
            }
        }
        if (mindist == INT_MAX) mindist = 0;
        return mindist;
    }
    
    void placeFood() {
        bool hasfood = false;
        std::vector<std::pair<int, int>> empties2 = std::vector<std::pair<int, int>>();
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                if (grid[i][j] == FOOD) hasfood = true;
                if (grid[i][j] == 0 && !danger({i, j})) {
                    empties2.push_back({i, j});
                }
            }
        }
        
        if (!hasfood) {
            std::pair<int, int> p = empties2[rand() % empties2.size()];
            grid[p.first][p.second] = FOOD;
        }
    }
    
    State getCurrentState() {
        int dir = prevmove;
        int dangers = 0;
        int foods = 0;
        std::pair<int, int> pos = snake.front();
        for (int i = 0; i < 3; i++) {
            int look = (dir + FLR[i]) % 4;
            std::pair<int, int> test = {pos.first + dx[look], pos.second + dy[look]};
            if (danger(test)) dangers += (1<<i);
            if (inBounds(test) && grid[test.first][test.second] == FOOD) foods += (1<<i); 
        }
        
        int proximity = getClosestDist(pos);
        
        int foodslarge = 0;
        std::pair<int, int> ph = getClosestFood(pos);
        if (dir == 0) {
            if (ph.first > pos.first) foodslarge += 1;
            // if (ph.first < pos.first) foodslarge += 2;
            if (ph.second > pos.second) foodslarge += 4;
            if (ph.second < pos.second) foodslarge += 8;
        }
        else if (dir == 1) {
            if (ph.second > pos.second) foodslarge += 1;
            // if (ph.second < pos.second) foodslarge += 2;
            if (ph.first < pos.first) foodslarge += 4;
            if (ph.first > pos.first) foodslarge += 8;
        }
        else if (dir == 2) {
            if (ph.first < pos.first) foodslarge += 1;
            // if (ph.first > pos.first) foodslarge += 2;
            if (ph.second < pos.second) foodslarge += 4;
            if (ph.second > pos.second) foodslarge += 8;
        }
        else if (dir == 3) {
            if (ph.second < pos.second) foodslarge += 1;
            // if (ph.second > pos.second) foodslarge += 2;
            if (ph.first > pos.first) foodslarge += 4;
            if (ph.first < pos.first) foodslarge += 8;
        }
        
        return State(dangers, dir, foods, foodslarge, 0);
    }
    
    double move(int dir, bool DEBUG = false) { // Only moves in the direction specified and returns the reward
        if (DEBUG) std::cout << "MOVING IN DIRECTION " << dir << "\n";
        placeFood();
        // get current State
        
        std::pair<int, int> pos = snake.front();
        std::pair<int, int> test = {pos.first + dx[dir], pos.second + dy[dir]};
        
        if (!inBounds(test)) return DANGER;
        
        prevmove = dir;
        if (grid[test.first][test.second] == FOOD) {
            if (danger(test)) return DANGER;
            snake.push_front(test);
            grid[test.first][test.second] = 0;
            score++;
            return EAT;
        }
        
        snake.pop_back();
        if (danger(test)) return DANGER;
        snake.push_front(test);
        
        if (getClosestDist(test) < getClosestDist(pos)) return 2;
        if (getClosestDist(test) > getClosestDist(pos)) return -2;
        return 0;
    }
    
    int act(State state, bool DEBUG = false) {
        double maxq = DBL_MIN;
        int chosen = rand() % 3; // front left or right
        for (int i = 0; i < 3; i++) {
            if (getQ(state, FLR[i]) > maxq) {
                maxq = getQ(state, FLR[i]);
                chosen = i;
            }
        }
        
        if ((double)(rand()) / RAND_MAX < epsilon) chosen = rand() % 3;
        chosen = FLR[chosen];
        
        // std::cout << "DIR CHOSEN " << chosen << "\n";
        
        int blip = move((state.dir + chosen) % 4, DEBUG);
        
        double curval = getQ(state, chosen);
        maxq = DBL_MIN;
        for (int i = 0; i < 3; i++) maxq = std::max(maxq, getQ(getCurrentState(), FLR[i]));
        
        double newQ = curval + alpha * (blip + gamma * maxq - curval);
        
        // std::cout << maxq << " " << newQ << "\n";
        
        setQ(state, chosen, newQ);
        
        return blip;
    }
    
    int iteration(bool DEBUG = false) {
        placeFood();
        
        return act(getCurrentState(), DEBUG);
    }
    
    void dispGrid() {
        for (int i = 0; i < height; i++) {
                for (int j = 0; j < width; j++) {
                    if (grid[i][j] == FOOD) std::cout << "#";
                    else if (isInSnake({i, j})) std::cout << "O";
                    else std::cout << ".";
                }
                std::cout << "\n";
            }
            std::cout << "\n";
    }
    
    void dispQ() {
        for (auto i : QTable) {
            std::cout << "[" << i.first.dir << " " << i.first.danger << " " << i.first.food << " " << i.first.foodlarge << " " << i.first.proximity << "]" << " ";
            for (auto j : i.second) std::cout << j << " ";
            std::cout << "\n";
        }
        
    }
    
     int death() {
        int score2 = score;
        score = 0;
        snake = std::deque<std::pair<int, int>>();
        snake.push_front({height>>1, width>>1});
        grid = std::vector<std::vector<int>>(height, std::vector<int>(width, 0));
        
        prevmove = rand() % 4;
        
        return score2;
    }
};

#endif
