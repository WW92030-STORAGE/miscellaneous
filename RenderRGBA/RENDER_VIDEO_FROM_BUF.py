# A program to render the output of Scene::buffer_data() to an image

from PIL import Image
import numpy as np
import cv2

# Recommended to use a venv
# e.g. source ~/Documents/VSCODE/PYTHON_VSC/bin/activate

"""

The video format is a directory containing N + 1 files, where there are N frames.
One file is called /LEN and contains only the number N.
The rest are called /FRAME_i (/FRAME_0, /FRAME_1, etc.) and are buffers:

Each frame buffer is in the following format:
[WIDTH, HEIGHT]
<color>,<color>,<color> ... <color>,
<color>,<color>...
...
...

line 1 is the dimensions, brackets and commas requried
The next (HEIGHT) lines is a comma separated list of (WIDTH) integers, each representing a color hex code. 
Each line must have a trailing comma.

WARNING - The format is RGBA, from highest to lowest bits. 

"""

def render(INPUT):
	file = open(INPUT, 'r')
	bufdat = file.read()
	
	ket = bufdat.find("]")
	dims = [int(i) for i in bufdat[1:ket].split(",")]

	W = dims[0]
	H = dims[1]
	
	im = Image.new(mode = "RGBA", size = (W, H))

	restofdat = "".join(bufdat[ket+1:].splitlines())
	restofdat = [int(i) for i in restofdat.split(",")[:-1]]

	index = 0
	for y in range(H):
		for x in range(W):
			c = restofdat[index]
			r = (c>>24) % (1<<8)
			g = (c>>16) % (1<<8)
			b = (c>>8) % (1<<8)
			a = c % (1<<8)
			im.putpixel((x, y), (r, g, b, a))
			index += 1

	file.close()

	return im

def generate_video(INPUT_DIR, OUTPUT, LEN, LOOPS = 1):
	print("GENERATING", OUTPUT + "...")
	meta = render(INPUT_DIR + "/FRAME_0")
	video = cv2.VideoWriter(OUTPUT, cv2.VideoWriter_fourcc(*'DIVX'), 30, (meta.width, meta.height))
	for L in range(LOOPS):
		for i in range(LEN):
			ren = render(INPUT_DIR + "/FRAME_" + str(i))
			video.write(cv2.cvtColor(np.array(ren), cv2.COLOR_RGB2BGR))

	video.release()
	cv2.destroyAllWindows()

	print(OUTPUT, "GENERATED")

if __name__ == "__main__":
	
	INPUT_DIR = "video"
	OUTPUT = "VIDEO"
	lenfile = open(INPUT_DIR + "/LEN", 'r')

	LEN = int(lenfile.read())
	generate_video(INPUT_DIR, OUTPUT + ".mov", LEN)
	# generate_video(INPUT_DIR, OUTPUT + ".mp4", LEN)
	# generate_video(INPUT_DIR, OUTPUT + ".mpeg", LEN)