import re
import sys
import textwrap


def main():
    if len(sys.argv) < 2:
        print "Invalid argument ... File name missing"
        return

    wrapper = textwrap.TextWrapper()

    with open(sys.argv[1], "r") as f:
        text = [line.strip(" \t\n\r") for line in f.readlines()]

    paragraphs = [""]
    for line in text:
        if line == "":
            paragraphs.append("")
        else:
            paragraphs[-1] += " " + line

    conversation_regex_i = re.compile(
        r"((^|[,.?!;]|--)[ \t]*)\'(((([ \t]+\'.*?\')|.)+?)([.,?!;]|--))(\'|([ \t]*)\'$|\'$)")

    index = 0
    while index < len(paragraphs):
        paragraphs[index] = conversation_regex_i.sub(
            r'\1"\3"', paragraphs[index])
        index += 1

    conversation_regex_e = re.compile(
        r"^(([^\"]|(\"[^\"]*\"))*?)\'([^\'\"]*?[,.!;?][ \t]*)\'((([^\"])|(\"[^\"]*\"))*)")

    with open("t", "w") as f:
        index = 0
        while index < len(paragraphs):
            # while conversation_regex_e.findall(paragraphs[index]) != []:
            #     paragraphs[index] = conversation_regex_e.sub(
            #         r'\1"\4"\5', paragraphs[index])
            f.write(paragraphs[index] + "\n\n\n")
            index += 1


if __name__ == "__main__":
    main()
