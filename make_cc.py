import argparse, subprocess, os

'''

Program to generate a new C++ project. 
Contains a main.cpp, a blank header, a README, a Makefile with options to build and run, and a bash script that builds, runs, and cleans up the binary.

'''

def run():
	parser = argparse.ArgumentParser()
	parser.add_argument("--name", "-n", help="name")
	args = parser.parse_args()
	
	if args.name is None:
		return
	
	name = args.name
	
	filename = "main"
	DIR = name + "/"
	SRC = filename + ".cpp"
	H = name.capitalize() + ".h"
	SH = name + ".sh"
	MD = "README.md"
	MAKE = "Makefile"
	
	try:
		os.mkdir(DIR)
	except Exception as e:
		print(e)
		return
	subprocess.run(["touch", DIR + SRC])
	subprocess.run(["touch", DIR + SH])
	subprocess.run(["touch", DIR + MD])
	subprocess.run(["touch", DIR + H])
	
	bashfile = f"""#!/bin/bash
	
g++ {filename}.cpp -o {filename} -O3
./{filename} > {name}.out
rm {filename}
"""

	with open(DIR + SH, 'w') as F:
		F.write(bashfile)
	
	srcfile = f'''#include <bits/stdc++.h>
using namespace std;
#include "{name.capitalize()}.h"
''' + '''

int main() {
	// your code here

	return 0;
}
'''

	with open(DIR + SRC, 'w') as F:
		F.write(srcfile)
	
	hfile = f'''#ifndef {name.upper()}_H
#define {name.upper()}_H

// your code here

#endif
'''
	with open(DIR + H, 'w') as F:
		F.write(hfile)

	mdfile = f"""# {name}
"""

	with open(DIR + MD, 'w') as F:
		F.write(mdfile)

	makefile = f"""all:
	g++ main.cpp -o main -O3
	./main

build:
	g++ main.cpp -o main -O3
	
run:
	./main
"""
	
	with open(DIR + MAKE, 'w') as F:
		F.write(makefile)
	

if __name__ == "__main__":
	run()
	
