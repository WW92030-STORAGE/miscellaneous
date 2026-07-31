import random
from PIL import Image, ImageDraw

# The first color in the colors array will be biased for.
def colornoise(w, h, output = "colornoise.png", seed = None):
    if seed is not None:
        random.seed(seed)
    image = Image.new("RGB", (w, h), (0, 0, 0))
    draw = ImageDraw.Draw(image)

    colors = [(255, 255, 255), (255, 0, 0), (0, 255, 0), (0, 0, 255), (0, 255, 255), (255, 0, 255), (255, 255, 0)]

    order = []
    pplap = (w * h) // len(colors)

    for i in range(pplap):
        for j in range(len(colors)):
            order.append(j)
    
    while len(order) < w * h:
        order.append(0)

    random.shuffle(order)

    i = 0
    for x in range(w):
        for y in range(h):
            draw.point((x, y), colors[order[i]])
            i += 1


    image.save(output)
    image.close()


if __name__ == "__main__":
    colornoise(16, 16, "sulfur_cube_goo.png", 1)