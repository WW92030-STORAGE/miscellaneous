with open("words.txt", "r") as FILE:
    for line in FILE:
        if (len(line) < 3):
            continue
        if line[0:2] != "za":
            continue
        print(line)