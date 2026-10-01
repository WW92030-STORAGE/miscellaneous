# A program to render the output of Scene::buffer_data() to an image

from PIL import Image
import numpy as np
import cv2
import argparse

# Recommended to use a venv
# e.g. source ~/Documents/VSCODE/PYTHON_VSC/bin/activate

COLOR = (255, 0, 0)

def render(INPUT, RESOLUTION = 16):
	file = open(INPUT, 'r')
	bufdat = file.read()
	
	ket = bufdat.find("]")
	dims = [int(i) for i in bufdat[1:ket].split(",")]
	BANDS = dims[0]
	W = dims[1]
	H = dims[2]
	
	im = Image.new(mode = "RGB", size = (W * RESOLUTION, H * RESOLUTION))

	restofdat = "".join(bufdat[ket+1:].splitlines())
	restofdat = [int(i) for i in restofdat.split(",")[:-1]]

	index = 0
	for y in range(H):
		for x in range(W):
			c = restofdat[index]
			VALUE = c / BANDS
			r = int(COLOR[0] * VALUE)
			g = int(COLOR[1] * VALUE)
			b = int(COLOR[2] * VALUE)
			a = 255
			for i in range(RESOLUTION):
				for j in range(RESOLUTION):
					im.putpixel((x * RESOLUTION + i, y * RESOLUTION + j), (r, g, b, a))
			index += 1

	im.save("RENDER.PNG")

	file.close()
	return im

def generate_video(INPUT_DIR, OUTPUT, LEN, RESOLUTION = 16):
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
	parser.add_argument('--resolution', '-r', type = int, default = 8, help="size of each pixel in the video")
	args = parser.parse_args()
	RESOLUTION = args.resolution

	INPUT_DIR = "video"
	OUTPUT = "VIDEO"
	lenfile = open(INPUT_DIR + "/LEN", 'r')

	LEN = int(lenfile.read())
	generate_video(INPUT_DIR, OUTPUT + ".mov", LEN, RESOLUTION)
	# generate_video(INPUT_DIR, OUTPUT + ".mp4", LEN, RESOLUTION)
	# generate_video(INPUT_DIR, OUTPUT + ".mpeg", LEN)