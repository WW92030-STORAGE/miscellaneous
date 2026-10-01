# A program to render the output of Scene::buffer_data() to an image

from PIL import Image
# Important constants

INPUT = "FIRE.out" # Where to read the buffer from

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


if __name__ == "__main__":
	render()