import re
import sys


def main():
    if len(sys.argv) < 2:
        print "Invalid argument ... File name missing"
        return

    with open(sys.argv[1], "r") as f:
        text = f.read().replace('"', "'")

    # ASSUMPTIONS
    #   1. There should be either a whitespace before the start of a quote, or it should be the start of a new paragraph
    #   2. There should be a punctuation before the quote of the main dialogue (not necessary for nested dialogues)
    #   3. There should be a whitespace (or EOF) after the end of a quote

    conversation_regex_i = re.compile(
        r"(?P<prefix>^|\s|--)\'(?P<main>((((\s+\'(\w\'|[^\']|\s)+?(\'|\w\'))+(\s|[,.;:!?-]))|.|\s)+?)[,;:.!?-])\'(?P<suffix>\s|[,;:.!?-])"
    )

    # REGEX EXPLANATION
    #   1. The prefix part defines the requirements before the start of the main dialogue (Assumption 1)
    #   2. The main part defines the requirements for the main dialogue, and for ignoring nested dialogues (Assumption 2)
    #   3. The suffix part defines the requirements for the text after the end of a quote (Assumption 3

    text = text + "\n\n''''"
    text = re.sub(r"([,;:.!?]\s)\'(\s)", r"\1 <MARKER>.'\2", text)
    text = text.replace("''", "\' <MARKER>.\'")
    text = conversation_regex_i.sub(r'\g<prefix>"\g<main>"\g<suffix>', text)
    text = text.replace(" <MARKER>.", "")

    with open("generated.txt", "w") as f:
        f.write(text[:-6])


if __name__ == "__main__":
    main()
