# A program to render the output of Scene::buffer_data() to an image

from PIL import Image, ImageDraw
import numpy as np
import cv2
import argparse

# Recommended to use a venv
# e.g. source ~/Documents/VSCODE/PYTHON_VSC/bin/activate

COLOR = (255, 0, 0)

def render(INPUT, RESOLUTION = 256, R = 4):
	file = open(INPUT, 'r')
	bufdat = file.read()
	
	ket = bufdat.find("]")
	dims = [float(i) for i in bufdat[1:ket].split(",")]
	N = int(dims[-1])

	BL = (dims[0], dims[1])
	TR = (dims[2], dims[3])
	W = int((TR[0] - BL[0]) * RESOLUTION)
	H = int((TR[1] - BL[1]) * RESOLUTION)
	
	im = Image.new(mode = "RGB", size = (W, H))
	draw = ImageDraw.Draw(im)

	restofdat = "".join(bufdat[ket+1:].splitlines())
	restofdat = [float(i) for i in restofdat.split(",")[:-1]]

	for i in range(N):
		x = restofdat[2 * i]
		y = restofdat[2 * i + 1]
		x = (x - BL[0]) * RESOLUTION
		y = (y - BL[1]) * RESOLUTION

		draw.ellipse((x - R, y - R, x + R, y + R), fill = 0xFFFFFF)

	file.close()
	return im


def make_gif(INPUT_DIR, OUTPUT, LEN, R = 256):
	frames = [render(INPUT_DIR + "/FRAME_" + str(i), R) for i in range(LEN)]

	output = frames[0]

	PERIOD = 50

	output.save(OUTPUT, format="GIF", append_images = frames[1:], save_all = True, duration = [PERIOD] * LEN, loop = 0)

if __name__ == "__main__":
	parser = argparse.ArgumentParser()
	parser.add_argument('--resolution', '-r', type = int, default = 256, help="size of each pixel in the video")
	args = parser.parse_args()
	RESOLUTION = args.resolution
	INPUT_DIR = "video"
	OUTPUT = "VIDEO"
	lenfile = open(INPUT_DIR + "/LEN", 'r')

	LEN = int(lenfile.read())
	make_gif(INPUT_DIR, OUTPUT + ".gif", LEN, RESOLUTION)