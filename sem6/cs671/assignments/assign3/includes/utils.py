import numpy as np
from conllu import parse, parse_tree


def load_data_file(file_name, tree=False):
    with open(file_name) as f:
        text = f.read()

    if tree:
        return (parse(text), parse_tree(text))
    else:
        return parse(text)


def load_vectors(file_name):
    with open(file_name, "r") as f:
        lines = [line.strip() for line in f.read().split("\n")]

    vectors = dict()

    for line in lines:
        line = line.split(' ')
        vectors[line[0]] = np.array(
            [float(field) for field in line[1:]]
        )

    return vectors
