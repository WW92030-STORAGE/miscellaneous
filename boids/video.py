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

def generate_video(INPUT_DIR, OUTPUT, LEN, RESOLUTION = 256):
	print("GENERATING", OUTPUT + "...")
	meta = render(INPUT_DIR + "/FRAME_0", RESOLUTION)
	video = cv2.VideoWriter(OUTPUT, cv2.VideoWriter_fourcc(*'DIVX'), 30, (meta.width, meta.height))
	for i in range(LEN):
		ren = render(INPUT_DIR + "/FRAME_" + str(i), RESOLUTION)
		video.write(cv2.cvtColor(np.array(ren), cv2.COLOR_RGB2BGR))

	video.release()
	cv2.destroyAllWindows()

	print(OUTPUT, "GENERATED")

if __name__ == "__main__":
	parser = argparse.ArgumentParser()
	parser.add_argument('--resolution', '-r', type = int, default = 256, help="size of each pixel in the video")
	args = parser.parse_args()
	RESOLUTION = args.resolution

	INPUT_DIR = "video"
	OUTPUT = "VIDEO"
	lenfile = open(INPUT_DIR + "/LEN", 'r')

	LEN = int(lenfile.read())
	generate_video(INPUT_DIR, OUTPUT + ".mov", LEN, RESOLUTION)
	# generate_video(INPUT_DIR, OUTPUT + ".mp4", LEN, RESOLUTION)
	# generate_video(INPUT_DIR, OUTPUT + ".mpeg", LEN)