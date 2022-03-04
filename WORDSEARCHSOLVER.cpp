#include <bits/stdc++.h> 
#include <stdio.h>
#include <math.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
#include <iterator>
#include <set>
#include <string>
#include <queue>
#include <stack>
#include <functional>
#include <ext/pb_ds/assoc_container.hpp>
using namespace std;
using namespace __gnu_pbds;
#define ll long long
#define pii pair<ll, ll>
#define pb push_back

vector<ll> protogen(string s) { // creates the z array
    ll n = s.length();
    vector<ll> res;
    for (ll i = 0; i < n; i++) res.pb(0);
    res[0] = -1;
    ll x = 0;
    ll y = 0;
    
    for (ll i = 1; i < n; i++) {
        res[i] = max(0LL, min(res[i - x], y - i + 1));
        while (i + res[i] < n && s[res[i]] == s[res[i] + i]) {
            x = i;
            y = i + res[i];
            res[i]++;
        }
    }
    
    return res;
}

vector<ll> search(string s, string p) { // searches for all instances of string p inside string s
    ll n = s.length();
    string spectrum = p + "#" + s;
    vector<ll> z = protogen(spectrum);
    vector<ll> res;
    for (ll i = 0; i < n; i++) {
        if (z[i + p.length() + 1] == p.length()) res.pb(i);
    }
    return res;
}

pair<pii, int> solve(vector<vector<char>>& board, string s1) {
    string s2 = "";
    int k = s1.length();
    int n = board.size();
    int m = board[0].size();
    
    for (int i = k - 1; i >= 0; i--) s2 = s2 + "" + s1[i];
    
    string s;
    for (int i = 0; i < n; i++) {
        s = "";
        for (int j = 0; j < m; j++) s = s + "" + board[i][j];
        vector<ll> searched = search(s, s1);
        if (searched.size() > 0) return {{i, searched[0]}, 0};
    }
    
    for (int i = 0; i < n; i++) {
        s = "";
        for (int j = 0; j < m; j++) s = s + "" + board[i][j];
        vector<ll> searched = search(s, s2);
        if (searched.size() > 0) return {{i, searched[0]}, 4};
    }
    
    for (int i = 0; i < m; i++) {
        s = "";
        for (int j = 0; j < n; j++) s = s + "" + board[j][i];
        vector<ll> searched = search(s, s1);
        if (searched.size() > 0) return {{i, searched[0] + k - 1}, 2};
    }
    
    for (int i = 0; i < m; i++) {
        s = "";
        for (int j = 0; j < n; j++) s = s + "" + board[j][i];
        vector<ll> searched = search(s, s1);
        if (searched.size() > 0) return {{searched[0], i}, 2};
    }
    
    for (int i = 0; i < m; i++) {
        s = "";
        for (int j = 0; j < n; j++) s = s + "" + board[j][i];
        vector<ll> searched = search(s, s2);
        if (searched.size() > 0) return {{searched[0] + k - 1, i}, 6};
    }
    
    // diagonals on the left wall, part 1
    for (int i = 0; i < n; i++) {
        s = "";
        for (int j = 0; ; j++) {
            if (j >= m || i + j >= n) break;
            s = s + "" + board[i + j][j];
        }
        vector<ll> searched = search(s, s1);
        if (searched.size() > 0) return {{i + searched[0], searched[0]}, 1};
    }
    
    for (int i = 0; i < n; i++) {
        s = "";
        for (int j = 0; ; j++) {
            if (j >= m || i + j >= n) break;
            s = s + "" + board[i + j][j];
        }
        vector<ll> searched = search(s, s2);
        if (searched.size() > 0) return {{i + searched[0] + k - 1, searched[0] + k - 1}, 5};
    }
    
    // diagonals on the top wall
    for (int i = 0; i < m; i++) {
        s = "";
        for (int j = 0; ; j++) {
            if (j >= n || i + j >= m) break;
            s = s + "" + board[j][i + j];
        }
        vector<ll> searched = search(s, s1);
        if (searched.size() > 0) return {{searched[0], i + searched[0]}, 1};
    }
    
    for (int i = 0; i < m; i++) {
        s = "";
        for (int j = 0; ; j++) {
            if (j >= n || i + j >= m) break;
            s = s + "" + board[j][i + j];
        }
        vector<ll> searched = search(s, s2);
        if (searched.size() > 0) return {{searched[0] + k - 1, i + searched[0] + k - 1}, 5};
    }
    
    // diagonals on the left wall, part 2
    for (int i = 0; i < n; i++) {
        s = "";
        for (int j = 0; ; j++) {
            if (j >= m || i - j < 0) break;
            s = s + "" + board[i - j][j];
        }
        vector<ll> searched = search(s, s1);
        if (searched.size() > 0) return {{i - searched[0], searched[0]}, 7};
    }
    
    for (int i = 0; i < n; i++) {
        s = "";
        for (int j = 0; ; j++) {
            if (j >= m || i - j < 0) break;
            s = s + "" + board[i - j][j];
        }
        vector<ll> searched = search(s, s2);
        if (searched.size() > 0) return {{i - searched[0] - k + 1, searched[0] + k - 1}, 3};
    }
    
    // diagonals on the bottom wall
    
    for (int i = 0; i < m; i++) {
        s = "";
        for (int j = 0; ; j++) {
            if (j >= n || i + j >= m) break;
            s = s + "" + board[n - j - 1][i + j];
        }
        vector<ll> searched = search(s, s1);
        if (searched.size() > 0) return {{n - searched[0] - 1, i + searched[0]}, 7};
    }
    
    for (int i = 0; i < m; i++) {
        s = "";
        for (int j = 0; ; j++) {
            if (j >= n || i + j >= m) break;
            s = s + "" + board[n - j - 1][i + j];
        }
        vector<ll> searched = search(s, s2);
        if (searched.size() > 0) return {{n - searched[0] - 1 - k + 1, i + searched[0] + k - 1}, 3};
    }
    
    return {{-1, -1}, -1};
}

int main()
{
	string s;
	int n, m, k;
	cin >> n >> m >> k;
	vector<vector<char>> board(n, vector<char>(m));
	
	for (int i = 0; i < n; i++) {
	    cin >> s;
	    assert(s.length() >= m);
	    for (int j = 0; j < m; j++) board[i][j] = s[j];
	}
	
	vector<string> words;
	for (int i = 0; i < k; i++) {
	    cin >> s;
	    words.pb(s);
	}
	
	for (int i = 0; i < n; i++) {
	    for (int j = 0; j < m; j++) cout << board[i][j];
	    cout << endl;
	}
	
	cout << endl;
	
	for (auto i : words) {
	    pair<pii, int> res = solve(board, i);
	    cout << i << " = " << res.first.first << " " << res.first.second << " | " << res.second << endl;
	}
	
   	return 0;
}



/* EXAMPLE INPUT (Source - Undertale)

6 16 12
GIASFCLFUBREHBER
NPBAVVVJJCSEOMEO
IWLSNOTELEKSTMFB
RLXETMONSTERMNGO
PMDIAMREMAUUJHIT
SCIGARSVXRSOUDCW

FALL WINTER SPRING SUMMER
MONSTER SKELETONS MERMAID ROBOT
CIGARS CIG GIASFCLFUBREHBER HOT

*/

/* EXAMPLE OUTPUT (POSITIONS ARE 0-INDEXED)

GIASFCLFUBREHBER
NPBAVVVJJCSEOMEO
IWLSNOTELEKSTMFB
RLXETMONSTERMNGO
PMDIAMREMAUUJHIT
SCIGARSVXRSOUDCW

FALL = 0 4 | 3
WINTER = 5 15 | 5
SPRING = 5 0 | 6
SUMMER = 5 10 | 7
MONSTER = 3 5 | 0
SKELETONS = 2 3 | 4
MERMAID = 4 2 | 4
ROBOT = 15 4 | 2
CIGARS = 5 1 | 0
CIG = 5 1 | 0
GIASFCLFUBREHBER = 0 0 | 0
HOT = 12 2 | 2

*/
