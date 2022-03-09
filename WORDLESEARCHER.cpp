#include <bits/stdc++.h> 
#include <stdio.h>
#include <math.h>
#include <iostream>
#include <fstream>
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

// NOTE FOR THE READER - LIST.TXT and LIST2.TXT are comprised of the plaintext files listed below in any order:
// https://gist.github.com/cfreshman/cdcdf777450c5b5301e439061d29694c
// https://gist.github.com/cfreshman/a03ef2cba789d8cf00c08f767e0fad7b
// All credit goes to their respective owners ... I merely wrote this code.

double rng() {
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<> dis(0.0, 1.0);
    return dis(gen);
}

bool check(string word, string letters, string mask) { // 0 grey 1 yellow 2 green
    // word is what we are checking for, letters is the input and mask is the result
    assert(word.length() == 5);
    assert(letters.length() == 5);
    assert(mask.length() == 5);
    
    int freq1[256];
    int freq2[256];
    for (int i = 0; i < 256; i++) freq1[i] = freq2[i] = 0;
    for (int i = 0; i < 5; i++) {
        freq1[word[i]]++;
        freq2[letters[i]]++;
    }
    
    for (int i = 0; i < 5; i++) {
        if (mask[i] == '2') {
            // if a letter is green
            // then it obviously must match its corresponding character
            if (letters[i] != word[i]) return false;
        }
        else if (mask[i] == '0') {
            // if a letter is grey
            // then it does not exist any more in the word
            // (yellows + greens in letters) >= (freq in word)
            int greenfreq = 0;
            for (int index = 0; index < 5; index++) {
                if (letters[index] == letters[i] && (mask[index] == '2' || mask[index] == '1')) greenfreq++;
            }
            if (freq1[letters[i]] > greenfreq) return false;
        }
        else {
            // if a letter is yellow
            // then it must exist in word
            // but not in the position in letters
            // the invariant is (yellows in letters) + (greens in letters) <= (nongreens in word) + (greens in word)
            // in other words -- (yellows in letters) <= (nongreens in word)
            if (letters[i] == word[i]) return false; // not same position
            int yellowfreq = 0;
            int nongreenfreq = 0;
            for (int index = 0; index < 5; index++) {
                if (mask[index] == '1' && letters[index] == letters[i]) yellowfreq++;
                if ((mask[index] == '0' || mask[index] == '1') && word[index] == letters[i]) nongreenfreq++;
            }
            if (yellowfreq > nongreenfreq) return false;
        }
    }
    return true;
}

string uppercase(string s) {
    string res = "";
    for (int i = 0; i < s.length(); i++) {
        if (s[i] >= 'a' && s[i] <= 'z') res = res + (char)(s[i] - 'a' + 'A');
        else res = res + s[i];
    }
    return res;
}

bool RESTRICT = true;
int nums = 16;

int main()
{
    ifstream wordlist("LIST.TXT");
    string s;
    
    vector<string> strings;
    while (getline(wordlist, s)) strings.pb(uppercase(s.substr(0, 5)));
    wordlist.close();
    
    ifstream wordlist2("LIST2.TXT");
    while (getline(wordlist, s)) strings.pb(uppercase(s.substr(0, 5)));
    wordlist.close();
    
    cout << "INPUT FORMAT -- GUESS MASK_" << "\n" << "0 = GREY | 1 = YELLOW | 2 = GREEN" << endl;
    
    string guess, mask;
    vector<string> outputs;
    vector<string> temp;
    vector<string> guesses;
    vector<string> masks;
    set<string> guessset;
    
    for (auto ii : strings) outputs.push_back(ii);
    sort(outputs.begin(), outputs.end());
    
    for (int i = 0; i < 9000; i++) {
        cin >> guess >> mask;
        guess = uppercase(guess);
        
        if (guessset.find(guess) != guessset.end()) {
            cout << "DUPLICATE ATTEMPT" << endl;
            i--;
            continue;
        }    
        
        guesses.pb(guess);
        masks.pb(mask);
        guessset.insert(guess);
        
        if (mask == "22222") {
            cout << "SOLVED IN " << (i + 1) << " ATTEMPTS" << endl;
            return 0;
        }
        
        for (int index = 0; index <= i; index++) {
            temp.erase(temp.begin(), temp.end());
            for (auto ii : outputs) {
                if (check(ii, guesses[index], masks[index])) temp.push_back(ii);
            }
            swap(outputs, temp);
        }
        
        if (!RESTRICT) {
            for (auto i : outputs) cout << i << " ";
            cout << endl;
        }
        else {
            set<int> numset;
            int on2 = outputs.size();
            while (numset.size() < min(on2, nums)) {
                int picked = (int)(rng() * on2);
                numset.insert(picked);
            }
            for (auto i : numset) cout << outputs[i] << " ";
            cout << endl;
        }
    }
    
   	return 0;
} 
