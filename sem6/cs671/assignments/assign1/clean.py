import sys

with open("t", "w") as fw:
    with open(sys.argv[1], "r") as fr:
        for para in fr.read().split("\n\n"):
            fw.write(para.replace("\n", " ") + "\n\n")
