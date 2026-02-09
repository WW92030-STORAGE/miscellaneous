import math

"""

Program to estimate locations of strongholds based on known stronghold location and stronghold count in the corresponding ring.
WARNING - Rings are very thick, locations are not exact.

"""

def round(x):
    return math.floor(x + 0.5)

# point = (x, z) of a known stronghold,  count = number of SH's in the ring (so 3 for the first ring, 6 for the next, etc.)
def obtain(point, count):
    r = math.sqrt(point[0] * point[0] + point[1] * point[1])
    theta = math.atan2(point[1], point[0])  # From +x, clockwise when looking down overhead
    
    points = [(point[0], point[1])]
    for i in range(1, count):
        theta += math.pi * 2 / count
        points.append((round(r * math.cos(theta)), round(r * math.sin(theta)) ) )
        
    return points
    
    
if __name__ == "__main__":
    point = [int(i) for i in input("known stronghold x z - ").split()]
    count = int(input("number of sh's in ring - "))
    
    print(obtain(point, count))
