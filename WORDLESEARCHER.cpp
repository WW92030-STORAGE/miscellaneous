#include <bits/stdc++.h> 
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

// DISCLAIMER - This program is in no way, shape, or form, a cheat engine. It is not designed to give you the right answer each time.
// DISCLAIMER - Its only purpose is to tell you which words fit the criteria you have received. There is no information theory here, only brute force + randomization.
// DISCLAIMER - Additionally, the program's output is not necessarily the best route. You might still have to think outside the box.
// DISCLAIMER - For example: if the program outputs "DERNY FERLY FERNY JERKY KERKY NERDY NERVY PERDY PERKY PERVY [10]" after your 3rd guess
// DISCLAIMER - The strategy is not to use one of the words on the list.
// DISCLAIMER - Instead, since all words share several letters, such as E, R, and Y, try words containing the more unique ones.
// DISCLAIMER - Inputting "PRODS" results in the output "21000" which narrows the 10 words down to 2: "PERKY PERVY [2]". You are now guaranteed to win this round.

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
            // the invariant is (yellows + greens in letters) <= (nongreens + greens in word)
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

vector<string> strings;
string guess, mask;
vector<string> outputs;
vector<string> temp;
vector<string> guesses;
vector<string> masks;
set<string> guessset;

void run() {
    mask = guess = "";
    outputs.clear();
    temp.clear();
    guesses.clear();
    masks.clear();
    guessset.clear();
    
    for (int i = 0; i < strings.size(); i++) {
        cin >> guess;
        guess = uppercase(guess);
        
        if (guess == "/UNDO") {
            assert(guesses.size() > 0);
            string guess2 = guesses[guesses.size() - 1];
            cout << "UNDO - REMOVED GUESS " << guess2 << endl;
            guesses.pop_back();
            masks.pop_back();
            guessset.erase(guessset.find(guess2));
            i -= 2;
            continue;
        }
        else if (guess == "/RESET") {
            cout << "GAME RESET" << endl;
            return;
        }
        
        cin >> mask;
        
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
            break;
        }
        
        outputs.clear();
        for (auto i : strings) outputs.pb(i);
        sort(outputs.begin(), outputs.end());
        
        for (int index = 0; index <= i; index++) {
            temp.erase(temp.begin(), temp.end());
            for (auto ii : outputs) {
                if (check(ii, guesses[index], masks[index])) temp.push_back(ii);
            }
            swap(outputs, temp);
        }
        
        if (!RESTRICT) {
            for (auto i : outputs) cout << i << " ";
        }
        else {
            set<int> numset;
            int on2 = outputs.size();
            while (numset.size() < min(on2, nums)) {
                int picked = (int)(rng() * on2);
                numset.insert(picked);
            }
            for (auto i : numset) cout << outputs[i] << " ";
        }
        cout << "[" << outputs.size() << "]" << endl;
    }
}

int main()
{
    ifstream wordlist("LIST.TXT");
    string s;
    
    while (getline(wordlist, s)) strings.pb(uppercase(s.substr(0, 5)));
    wordlist.close();
    
    cout << strings.size() << endl;
    
    ifstream wordlist2("LIST2.TXT");
    while (getline(wordlist2, s)) strings.pb(uppercase(s.substr(0, 5)));
    wordlist2.close();
    
    cout << strings.size() << endl;
    
    cout << "INPUT FORMAT -- [GUESS] [MASK]" << "\n" << "0 = GREY | 1 = YELLOW | 2 = GREEN" << endl;
    cout << "COMMANDS - /UNDO TO UNDO AND /RESET TO RESET" << endl;
    
    while (true) run();
    
   	return 0;
}
