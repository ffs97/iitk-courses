import os

import model
import representation

import numpy as np

from nltk.corpus import stopwords


def load(root_dir):
    data = list()
    labels = list()

    for label, file_name in enumerate([root_dir + "/neg.txt", root_dir + "/pos.txt"]):
        with open(file_name, "r") as f:
            for line in f:
                data.append(line.strip())
                labels.append(label)

    data = np.array(data)
    labels = np.array(labels)

    return {
        "data": data,
        "labels": labels
    }


def load_vectors(file_name):
    with open(file_name, "r") as f:
        lines = [line.strip() for line in f.read().split("\n")]

    vectors = dict()

    for line in lines:
        line = line.split(' ')
        vectors[line[0]] = np.array(
            [float(field) for field in line[1:]]
        )

    print "Done"

    return vectors


def main():
    print
    print "Loading IMDB Vocabulary ..."

    with open("data/imdb.vocab", "r") as f:
        words = [line.strip() for line in f.read().split("\n")]

    representation.vocab = dict()

    id = 0
    stop_words = set(stopwords.words("english"))
    for word in words:
        word = word.lower()
        if word not in stop_words:
            representation.vocab[word] = id
            id += 1

    representation.vocab_size = len(representation.vocab.keys())

    print "Done"

    print
    print "Loading GloVe Vectors ..."

    representation.glove_vectors = load_vectors("data/glove.txt")

    print
    print "Loading Word2Vec Vectors ..."

    representation.word2vec_vectors = load_vectors("data/word2vec.txt")

    print
    print "Loading Dataset ..."

    model.train_data = load("data/train")
    model.test_data = load("data/test")

    model.len_data = len(model.train_data["labels"])

    print "Done"

    model.train_model(model.logistic, "Logistic Regression", n_iters={
        "bow": 1250,
        "glove": 5000,
        "word2vec": 3500,
        "tfidf": 1250,
        "normalizedtf": 1250
    })

    model.train_model(model.SVM, "Naive Bayes", n_iters={
        "bow": 1250,
        "glove": 5000,
        "word2vec": 3500,
        "tfidf": 1250,
        "normalizedtf": 1250
    })

    model.train_model(model.SVM, "Support Vector Machine", n_iters={
        "bow": 1250,
        "glove": 5000,
        "word2vec": 3500,
        "tfidf": 1250,
        "normalizedtf": 1250
    })

    model.train_model(model.NN, "Feedforwad Neural Network", n_iters={
        "bow": 200,
        "glove": 2500,
        "word2vec": 2500,
        "tfidf": 150,
        "normalizedtf": 150
    })

    model.train_model(model.LSTM, "LSTM Network", n_iters={
        "glove": 15,
        "word2vec": 15
    })

    model.train_model(model.ELSTM, "Embedded-LSTM Network", n_iters={
        "glove": 10,
        "word2vec": 10
    })


if __name__ == "__main__":
    main()
