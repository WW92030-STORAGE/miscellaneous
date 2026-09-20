#ifndef HILBERT_H
#define HILBERT_H

#include <array>
#include <cstdint>
#include <algorithm>
#include <string>

/*

Generate and act upon a hilbert matrix. The hilbert matrix is a (2^N)x(2^N) square array and will always remain as such.
The Hilbert matrix represents a Hilbert curve, so that each number in a cell represents how far along the curve you are.
Moving from one number to the next moves you along the curve.
Coordinates are right-up, so that mat[x][y] is x right and y up. 

*/
typedef uint32_t cellType;
typedef std::vector<std::vector<cellType>> hilbertMatrix;

// Utility methods
// All supplemental methods are in place

// Flip the matrix along a vertical (x = ...) line. This flips the rows of v.
void flipX(hilbertMatrix& v) {
	int n = v.size();
	for (int i = 0; i < (n>>1); i++) {
		for (int j = 0; j < v[i].size(); j++) std::swap(v[i][j], v[n - i - 1][j]);
	}
}

// Transpose the matrix along the main diagonal
void trans(hilbertMatrix& v) {
	int n = std::min(v.size(), v[0].size());
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < i; j++) std::swap(v[i][j], v[j][i]);
	}
}

// Transpose the matrix along the main bisector (the other large diagonal)
void bisector(hilbertMatrix& v) {
	int n = std::min(v.size(), v[0].size());
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n - i; j++) std::swap(v[i][j], v[n - j - 1][n - i - 1]);
	}
}

// Rotate the matrix 90 degrees counterclockwise. (In coordinate systems where x is down and y right, this rotates clockwise.)
void rotate(hilbertMatrix& v) {
	trans(v);
	flipX(v);
}

// RECURSIVELY CONSTRUCT A HILBERT MATRIX

hilbertMatrix hilbert(int8_t ord) {
	if (ord <= 0) ord = 1;
	if (ord >= 15) ord = 12;

	int n = (1<<ord);

	hilbertMatrix res(n, std::vector<cellType>(n, 0));

	if (ord == 1) {
		res[0][0] = 0;
		res[0][1] = 1;
		res[1][1] = 2;
		res[1][0] = 3;
		return res;
	}

	// You will need around 1/3 as much extra area as will have the final matrix (get it?)

	hilbertMatrix mat = hilbert(ord - 1);
	int half = (1<<(ord - 1));
	cellType quadrant = (1<<((ord-1)<<1));

	// Each quadrant of the final matrix is a copy of mat, transformed, and then cell values offset by the areas of what came before them. (i.e. quadrant i is a copy of mat + i * quadrant)

	// 1. top left (quadrant 1) and top right (quadrant 2) are mat as is
	
	for (int x = 0; x < half; x++) {
		for (int y = 0; y < half; y++) {
			res[x][y + half] = mat[x][y] + quadrant;
			res[x + half][y + half] = mat[x][y] + (quadrant<<1);
		}
	}

	// 2. bottom right (quadrant 3) is mat bisectored.
	bisector(mat);
	cellType triple = quadrant * 3;
	for (int x = 0; x < half; x++) {
		for (int y = 0; y < half; y++) res[x + half][y] = mat[x][y] + triple;
	}

	// 3. bottom left (quadrant 0) is mat rotated twice from what it was in the bottom right
	rotate(mat);
	rotate(mat);
	for (int x = 0; x < half; x++) {
		for (int y = 0; y < half; y++) res[x][y] = mat[x][y];
	}


	return res;
}

std::string disp(hilbertMatrix& v) {
	uint64_t area = v.size() * v[0].size();
	int slen = std::to_string(area).length();

	int X = v.size();
	int Y = v[0].size();
	for (auto i : v) Y = std::min(Y, int(i.size()));

	std::string res = std::to_string(v.size());
	for (int y = Y - 1; y >= 0; y--) {
		res.push_back('\n');
		for (int x = 0; x < X; x++) {
			std::string num = std::to_string(v[x][y]);
			for (int i = slen - num.size(); i > 0; i--) num = "0" + num;
			res += num;
			res.push_back(' ');
		}
	}
	return res;
}

/*

int main() {
	// your code here
	auto thing = hilbert(4);
	cout << disp(thing);

	return 0;
}

Output:

16
085 086 089 090 101 102 105 106 149 150 153 154 165 166 169 170 
084 087 088 091 100 103 104 107 148 151 152 155 164 167 168 171 
083 082 093 092 099 098 109 108 147 146 157 156 163 162 173 172 
080 081 094 095 096 097 110 111 144 145 158 159 160 161 174 175 
079 076 075 074 117 116 115 112 143 140 139 138 181 180 179 176 
078 077 072 073 118 119 114 113 142 141 136 137 182 183 178 177 
065 066 071 070 121 120 125 126 129 130 135 134 185 184 189 190 
064 067 068 069 122 123 124 127 128 131 132 133 186 187 188 191 
063 062 049 048 047 044 043 042 213 212 211 208 207 206 193 192 
060 061 050 051 046 045 040 041 214 215 210 209 204 205 194 195 
059 056 055 052 033 034 039 038 217 216 221 222 203 200 199 196 
058 057 054 053 032 035 036 037 218 219 220 223 202 201 198 197 
005 006 009 010 031 028 027 026 229 228 227 224 245 246 249 250 
004 007 008 011 030 029 024 025 230 231 226 225 244 247 248 251 
003 002 013 012 017 018 023 022 233 232 237 238 243 242 253 252 
000 001 014 015 016 019 020 021 234 235 236 239 240 241 254 255 


*/

#endif
