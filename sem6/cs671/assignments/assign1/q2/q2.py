import sys

import numpy as np

from sklearn import svm
from sklearn.metrics import accuracy_score
from sklearn.neural_network import MLPClassifier
from sklearn.model_selection import train_test_split


character_dict = dict()
data = ""


def parse_data(k=1):
    global character_dict, data

    punctuations = ['.', '!', '?']

    text = list(data.replace("<s>", "").replace("</s>", "$"))

    train_data = [[], []]

    for index, char in enumerate(text):
        if char in punctuations:
            label = 0
            word_vec = np.zeros(len(character_dict))

            if index < len(text) - 1 and text[index + 1] == '$':
                label = 1

            # Window of 2*k characters excluding the punctuation symbol
            w = 0
            for p in range(index - 1, 0, -1):
                if w == k:
                    break
                if text[p] != '$':
                    word_vec[character_dict[text[p]]] += 1
                    w += 1

            w = 0
            for p in range(index + 1, len(text)):
                if w == k:
                    break
                if text[p] != '$':
                    word_vec[character_dict[text[p]]] += 1
                    w += 1

            train_data[0].append(word_vec)
            train_data[1].append(label)

    train_data[0] = np.array(train_data[0])
    train_data[1] = np.array(train_data[1])
    return train_data


def main():
    global character_dict, data

    if len(sys.argv) < 2:
        print "Invalid argument ... File name missing"
        return

    k = 2

    with open(sys.argv[1], "r") as f:
        train_data = f.read()

    test_data = ""
    if len(sys.argv) > 2:
        with open(sys.argv[2], "r") as f:
            test_data = f.read()

    character_dict = dict([
        (char, i) for i, char in enumerate(list(set(train_data + test_data)))
    ])

    data = train_data
    train_data = parse_data(k)

    if len(sys.argv) == 2:
        X_train, X_test, y_train, y_test = train_test_split(
            train_data[0], train_data[1], test_size=0.3
        )

    else:
        data = test_data
        test_data = parse_data(k)

        X_train, y_train = train_data
        X_test, y_test = test_data

    clf = svm.SVC(kernel='linear', C=1)
    clf.fit(X_train, y_train)

    y_pred = clf.predict(X_test)
    print "Accuracy on test data:", "%f" % (accuracy_score(y_test, y_pred) * 100)


if __name__ == "__main__":
    main()
