// This solves a variation of the missionaries and cannibals problem, in which all the missionaries can navigate the boat however only one cannibal can navigate the boat.
// The boat needs at least one navigator to function, and the rest of the rules are the same.
// Each state is represented by six numbers, and the outputted solution is in reverse order.

#include <bits/stdc++.h> 
#include <ext/pb_ds/assoc_container.hpp>
using namespace std;
using namespace __gnu_pbds;
#define ll long long
#define pii pair<ll, ll>
#define pb push_back
#define data tuple<int, int, int, int, int, int>
// (mis left) (can left) (mis right) (can right) (steer loc) (boat loc) (0 = left 1 = right)

int x(data p, int n) {
    switch(n) {
        case 0: return get<0>(p);
        case 1: return get<1>(p);
        case 2: return get<2>(p);
        case 3: return get<3>(p);
        case 4: return get<4>(p);
        case 5: return get<5>(p);
    }
    return 0;
}

data clone(data d) {
    int arr[6];
    for (int n = 0; n < 6; n++) arr[n] = x(d, n);
    return make_tuple(arr[0], arr[1], arr[2], arr[3], arr[4], arr[5]);
}

data dec(data d, int i) {
    int arr[6];
    for (int n = 0; n < 6; n++) arr[n] = x(d, n);
    arr[i]--;
    return make_tuple(arr[0], arr[1], arr[2], arr[3], arr[4], arr[5]);
}

data inc(data d, int i) {
    int arr[6];
    for (int n = 0; n < 6; n++) arr[n] = x(d, n);
    arr[i]++;
    return make_tuple(arr[0], arr[1], arr[2], arr[3], arr[4], arr[5]);
}

data protogen(data d, int a, int b) { // transfer an item from a to b
    data d1 = dec(d, a);
    return inc(d1, b);
}

bool primagen(data d) {
    if (x(d, 0) < x(d, 1) && x(d, 0) > 0) return false;
    if (x(d, 2) < x(d, 3) && x(d, 2) > 0) return false;
    for (int i = 0; i < 4; i++) {
        if (x(d, i) < 0) return false;
        if (x(d, i) > 3) return false;
    }
    if (x(d, 5) < 0 || x(d, 5) > 1) return false;
    if (x(d, 4) < 0 || x(d, 4) > 1) return false;
    return true;
}

void disp(data d) {
    for (int i = 0; i < 6; i++) cout << x(d, i) << " ";
    cout << endl;
}

data end1 = {3, 3, 0, 0, 0, 0};
data end2 = {3, 3, 0, 0, 0, 1};
data start = make_tuple(0, 0, 3, 3, 1, 1);

map<data, data> parent;

void bfs(data src) {
    parent.clear();
    parent.insert({src, src});
    set<data> s;
    queue<data> q;
    q.push(src);
    while (q.size() > 0) {
        data d = q.front();
        
        // cout << "d = ";
        // disp(d);
        
        q.pop();
        s.insert(d);
        if (d == end1 || d == end2) return;
        if (x(d, 5) == 0) {
            data d0, d1, d2, d3, d4, d5;
            d0 = d1 = d2 = d3 = d4 = d5 = clone(d);
            d0 = protogen(d0, 0, 2);
            d1 = protogen(d1, 0, 2);
            d1 = protogen(d1, 0, 2);
            if (x(d2, 4) == 0) {
                d2 = protogen(d2, 1, 3);
                d2 = inc(d2, 4);
                d2 = inc(d2, 5);
            }
            if (x(d3, 4) == 0) {
                d3 = protogen(d3, 1, 3);
                d3 = protogen(d3, 1, 3);
                d3 = inc(d3, 4);
                d3 = inc(d3, 5);
            }
            d4 = protogen(d4, 0, 2);
            d4 = protogen(d4, 1, 3);
            d5 = protogen(d5, 0, 2);
            d5 = protogen(d5, 1, 3);
            d5 = inc(d5, 4);
            
            d0 = inc(d0, 5);
            d1 = inc(d1, 5);
            d4 = inc(d4, 5);
            d5 = inc(d5, 5);
            
            data arr[6] = {d0, d1, d2, d3, d4, d5};
            for (int i = 0; i < 6; i++) {
                // disp(arr[i]);
                if (!primagen(arr[i])) continue;
                if (s.find(arr[i]) != s.end()) continue;
                s.insert(arr[i]);
                parent.insert({arr[i], d});
                q.push(arr[i]);
            }
        }
        else {
            data d0, d1, d2, d3, d4, d5;
            d0 = d1 = d2 = d3 = d4 = d5 = d;
            d0 = protogen(d0, 2, 0);
            d1 = protogen(d1, 2, 0);
            d1 = protogen(d1, 2, 0);
            if (x(d2, 4) == 1) {
                d2 = protogen(d2, 3, 1);
                d2 = dec(d2, 4);
                d2 = dec(d2, 5);
            }
            if (x(d3, 4) == 1) {
                d3 = protogen(d3, 3, 1);
                d3 = protogen(d3, 3, 1);
                d3 = dec(d3, 4);
                d3 = dec(d3, 5);
            }
            d4 = protogen(d4, 2, 0);
            d4 = protogen(d4, 3, 1);
            d5 = protogen(d5, 2, 0);
            d5 = protogen(d5, 3, 1);
            d5 = dec(d5, 4);
            
            d0 = dec(d0, 5);
            d1 = dec(d1, 5);
            d4 = dec(d4, 5);
            d5 = dec(d5, 5);
            
            data arr[6] = {d0, d1, d2, d3, d4, d5};
            for (int i = 0; i < 6; i++) {
                // disp(arr[i]);
                if (!primagen(arr[i])) continue;
                if (s.find(arr[i]) != s.end()) continue;
                s.insert(arr[i]);
                parent.insert({arr[i], d});
                q.push(arr[i]);
            }
        }
    }
}

int main()
{
	bfs(start);
	data d0 = end1;
	for (auto i : parent) {
	    // cout << "---------------------" << endl;
	    // disp(i.first);
	    // disp(i.second);
	}
	
	while (d0 != start) {
	    disp(d0);
	    d0 = parent.at(d0);
	}
	disp(d0);
	
   	return 0;
}

// SPOILERS BELOW















/* 
3 3 0 0 0 0 
2 2 1 1 0 1 
3 2 0 1 0 0 
3 0 0 3 1 1 
3 1 0 2 0 0 
1 1 2 2 0 1 
2 2 1 1 0 0 
1 1 2 2 1 1 
2 2 1 1 1 0 
0 2 3 1 1 1 
0 3 3 0 0 0 
0 1 3 2 1 1 
0 2 3 1 0 0 
0 0 3 3 1 1 
*/
