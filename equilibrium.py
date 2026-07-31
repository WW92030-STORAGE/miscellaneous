from num2words import num2words

vowels = {'a', 'e', 'i', 'o', 'u', 'A', 'E', 'I', 'O', 'U'}
punctuation = {'.', ',', '-', '?', '!', '\'', '\"', '(', ')', '_', ':', ';', '[', ']', '/', '\\', '&'}

# How many variables are there?
def dataFormat():
    return [0] * 4

# How the count is defined
def getCount(value):
    res = dataFormat()
    for cc in value:
        if cc.isalpha():
            res[0] += 1
            if cc in vowels:
                res[1] += 1
            else:
                res[2] += 1
        if cc in punctuation:
            res[3] += 1
    return res

# Get count of a list of counts of the static portions, and the variables (which we count here)
def getCountByCounts(p_cnt, guess):
    res = dataFormat()
    for pc in p_cnt:
        for i in range(len(pc)):
            res[i] += pc[i]

    for element in guess:
        thing = getCount(num2words(element))
        for i in range(len(thing)):
            res[i] += thing[i]
    
    return res

def construct(p, guess = dataFormat()):
    res = p[0]
    for i in range(len(guess)):
        res += " " + num2words(guess[i]) + " " + p[i + 1]
    return res

def iterate(p_cnt, guess = dataFormat()):
    return getCountByCounts(p_cnt, guess)

def findEquilibrium(p, guess = None, max_iters = 1024):
    p_cnt = [getCount(i) for i in p]
    if guess is None:
        guess = dataFormat()
    
    fast = guess
    slow = guess
    for i in range(max_iters):
        fast = iterate(p_cnt, fast)
        fast = iterate(p_cnt, fast)
        slow = iterate(p_cnt, slow)
        if fast == slow:
            break
    
    return fast, (fast == iterate(p_cnt, fast))

def gridsearch(p, llim = None, ulim = None, length = 16, guess = None, ind = 0):
    while len(p) < 1 + len(dataFormat()):
        p.append("")
    if guess is None:
        guess = dataFormat()

    if llim is None or ulim is None:
        com = ""
        for x in p:
            com += x
        cc = getCount(com)
        llim = cc
        ulim = [i + length for i in llim]
        print("LIMITS", llim, ulim)
    
    if ind >= len(p) - 1:
        if guess[-2] == llim[-2] and guess[-1] == llim[-1]:
            print(guess)
        
        a, b = findEquilibrium(p, guess)
        if b:
            return a
        return None
    
    res = None
    for i in range(llim[ind], ulim[ind]):
        guess[ind] = i
        res = gridsearch(p, llim, ulim, length, guess, ind + 1)
        if res is not None:
            return res
    
    return res

def run():
    p1 = "Good evening furries, scalies, protogens, synths, and all of the B&WC. This message contains"
    p2 = "letters,"
    p3 = "vowels,"
    p4 = "consonants, and"
    p5 = "punctuation marks."
    p = [p1, p2, p3, p4, p5]

    value = gridsearch(p)
    if value is None:
        print("No solution...")
        return
    
    final = construct(p, value)
    print(final)
    print(getCount(final))

def true_run(OUT = "equilibrium.txt"):
    p0 = ""
    with open("equilibrium.in", 'r') as F:
        for line in F:
            p0 += line
    p1 = "Anyways, this message contains"
    p2 = "letters,"
    p3 = "vowels,"
    p4 = "consonants, and"
    p5 = "punctuation marks."

    if len(p0) > 0 and p0[-1].isspace():
        p1 = p0 + p1
    else:
        p1 = p0 + " " + p1

    p = [p1, p2, p3, p4, p5]

    value = gridsearch(p)
    if value is None:
        print("No solution...")
        return
    
    final = construct(p, value)
    print(getCount(final))
    with open(OUT, 'w') as F:
        F.write(final)

if __name__ == "__main__":
    true_run()