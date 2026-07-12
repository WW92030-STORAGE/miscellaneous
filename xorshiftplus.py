def int32tobits(value):
    thing = bin(value)[2:]
    thing = thing[:32]
    while len(thing) < 32:
        thing = "0" + thing
    return thing

def lshift32(value, k):
    return (value << k) & ((1<<32) - 1)

def xorshift32(value):
    value = value ^ lshift32(value, 13)
    value = value ^ (value >> 17)
    value = value ^ lshift32(value, 5)
    return value
    
def xorshiftplus(value):
    a1 = xorshift32(value)
    return (a1 + xorshift32(a1)) & ((1<<32) - 1)
    
    
if __name__ == "__main__":
    # put value here
    value = 0b0010000100101110
    
    for i in range(1<<16):
        vv = (value << 16) | i
        var = int32tobits(xorshiftplus(vv))
        # print(i, int32tobits(vv), int32tobits(xorshift32(vv)))
        # print(i, int32tobits(vv), var)
        if var[:12] == "0" * 12:
            print(i, int32tobits(i))
            print(int32tobits(vv), var)
            break
