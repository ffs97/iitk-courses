import re
import sys


def main():
    if len(sys.argv) < 2:
        print "Invalid argument ... File name missing"
        return

    with open(sys.argv[1], "r") as f:
        text = f.read()

    non_terminators = {
        r"(?P<prefix>\s)(?P<main>[A-Za-z])[.]":    r"\g<prefix>\g<main><MARKER-DOT>",
        r"(?P<prefix>\s)(?P<main>Mrs)[.]":    r"\g<prefix>\g<main><MARKER-DOT>",
        r"(?P<prefix>\s)(?P<main>Mr)[.]":    r"\g<prefix>\g<main><MARKER-DOT>",
        r"(?P<prefix>\s)(?P<main>Dr)[.]":    r"\g<prefix>\g<main><MARKER-DOT>",
    }

    replacements = {
        "<MARKER-DOT>": ".",
        "<MARKER>.": ""
    }

    # I have not kept sentence breaking within quotes, as it did not made sense in a lot of cases
    statement_regex = re.compile(
        r"(?P<prefix>[\s.!?]*)(?P<main>([^.!?\"]|\s)*(\"([^\"]|\s)*?\"([^.!?\"]|\s)*)*([.!?]|\n\n))"
    )

    # REGEX EXPLANATION
    #   1. The prefix part ignores all whitespaces or wasteful punnctuations before the start of the sentence
    #   2. The main part has three parts
    #       a. All non-punctuation characters or marked punctuations outside quotes
    #       b. All characters within quotes
    #       c. The terminating punctiation

    text = text + "\n\n"

    for non_terminator in non_terminators:
        text = re.sub(non_terminator, non_terminators[non_terminator], text)

    text = re.sub(r"([^.!?\n])(\n\n+)", r"\1<MARKER>.\2", text)

    text = statement_regex.sub(r"\1<s>\2</s>", text)
    text = re.sub(r"(\s*)</s>", r"</s>\1", text)

    for replacement in replacements:
        text = text.replace(replacement, replacements[replacement])

    text = text[:-2]

    out = "generated.txt"
    if len(sys.argv) > 2:
        out = sys.argv[2]

    with open(out, "w") as f:
        f.write(text)


if __name__ == "__main__":
    main()
