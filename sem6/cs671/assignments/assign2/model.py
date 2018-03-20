import representation

import time
import numpy as np
from tqdm import tqdm

from sklearn import linear_model
from sklearn.neural_network import MLPClassifier
from sklearn.naive_bayes import BernoulliNB, MultinomialNB

from keras.preprocessing import sequence
from keras.models import Sequential
from keras.layers import Dense, Embedding
from keras.layers import LSTM as kLSTM
from keras import regularizers


train_data = None
test_data = None

len_data = 0


def sgd_train(rep_model, model, n_iters, batch_size, test_size):
    indices = np.arange(len_data)

    for i in tqdm(range(n_iters)):
        np.random.shuffle(indices)

        batch_data = rep_model(train_data["data"][indices[:batch_size]])
        batch_labels = train_data["labels"][indices[:batch_size]]

        model.partial_fit(batch_data, batch_labels, classes=[0, 1])

    np.random.shuffle(indices)

    ts_data = rep_model(test_data["data"][indices[:test_size]])
    ts_labels = test_data["labels"][indices[:test_size]]

    return model.score(ts_data, ts_labels)


def logistic(rep_model, n_iters, rep, batch_size=200, test_size=5000):
    model = linear_model.SGDClassifier(
        loss='log',
        penalty='l2',
        alpha=0.0001,
        n_jobs=-1,
        max_iter=n_iters
    )

    return sgd_train(rep_model, model, n_iters, batch_size, test_size)


def naive_bayes(rep_model, n_iters, rep, batch_size=200, test_size=5000):
    if rep in ["tfidf", "normalizedtf"]:
        model = MultinomialNB()
    else:
        model = BernoulliNB()

    return sgd_train(rep_model, model, n_iters, batch_size, test_size)


def SVM(rep_model, n_iters, rep, batch_size=200, test_size=5000):
    model = linear_model.SGDClassifier(
        loss='hinge',
        penalty='l2',
        alpha=0.0001,
        n_jobs=-1,
        max_iter=n_iters
    )

    return sgd_train(rep_model, model, n_iters, batch_size, test_size)


def NN(rep_model, n_iters, rep, batch_size=200, test_size=5000):
    model = MLPClassifier(
        hidden_layer_sizes=(100, 100),
        activation='tanh',
        max_iter=n_iters
    )

    return sgd_train(rep_model, model, n_iters, batch_size, test_size)


def LSTM(rep_model, n_iters, rep, batch_size=64, test_size=5000):
    # https://github.com/keras-team/keras/tree/master/examples

    X_train = representation.data_vectors(
        train_data["data"],
        format=rep,
        max_len=50
    )
    y_train = train_data["labels"]

    X_test = representation.data_vectors(
        test_data["data"],
        format=rep,
        max_len=50
    )
    y_test = test_data["labels"]

    indices = np.arange(len_data)
    np.random.shuffle(indices)
    X_test, y_test = X_test[indices[:test_size]], y_test[indices[:test_size]]

    model = Sequential()

    model.add(kLSTM(
        100,
        input_shape=(50, 300),
        dropout=0.2,
        recurrent_dropout=0.2
    ))
    model.add(Dense(
        1,
        activation='sigmoid',
        kernel_regularizer=regularizers.l2(0.5)
    ))

    model.compile(
        loss='binary_crossentropy',
        optimizer='adam',
        metrics=['accuracy']
    )

    model.fit(
        X_train,
        y_train,
        batch_size=batch_size,
        epochs=n_iters
    )

    return model.evaluate(X_test, y_test, batch_size=batch_size)[1]


def ELSTM(rep_model, n_iters, rep, batch_size=64, test_size=5000):
    X_train = sequence.pad_sequences(
        representation.data_index(train_data["data"]),
        maxlen=50
    )
    y_train = train_data["labels"]

    X_test = sequence.pad_sequences(
        representation.data_index(test_data["data"]),
        maxlen=50
    )
    y_test = test_data["labels"]

    indices = np.arange(len_data)
    np.random.shuffle(indices)

    X_test, y_test = X_test[indices[:test_size]], y_test[indices[:test_size]]

    model = Sequential()

    model.add(Embedding(
        representation.vocab_size,
        300,
        weights=representation.get_embedding_matrix(rep)
    ))
    model.add(kLSTM(
        100,
        input_shape=(50, 300),
        dropout=0.2,
        recurrent_dropout=0.2
    ))
    model.add(Dense(
        1,
        activation='sigmoid',
        kernel_regularizer=regularizers.l2(0.5)
    ))

    model.compile(
        loss='binary_crossentropy',
        optimizer='adam',
        metrics=['accuracy']
    )

    model.fit(
        X_train,
        y_train,
        batch_size=batch_size,
        epochs=n_iters
    )

    return model.evaluate(X_test, y_test, batch_size=batch_size)[1]


def train_model(model, label, n_iters):
    print "\n\n" + ("#" * 33)
    print "MODEL: %26s" % (label.upper())
    print

    if "bow" in n_iters:
        print "Training for BOW Representation ..."
        time.sleep(0.5)
        bow_score = model(
            representation.bow, n_iters=n_iters["bow"], rep="bow"
        )
        print

    if "glove" in n_iters:
        print "Training for GLOVE Representation ..."
        time.sleep(0.5)
        glove_score = model(
            representation.glove, n_iters=n_iters["glove"], rep="glove"
        )
        print

    if "word2vec" in n_iters:
        print "Training for WORD2VEC Representation ..."
        time.sleep(0.5)
        word2vec_score = model(
            representation.word2vec, n_iters=n_iters["word2vec"], rep="word2vec"
        )
        print

    if "tfidf" in n_iters:
        print "Training for TF-IDF Representation ..."
        time.sleep(0.5)
        tfidf_score = model(
            representation.tfidf, n_iters=n_iters["tfidf"], rep="tfidf"
        )
        print

    if "normalizedtf" in n_iters:
        print "Training for NORMALIZED TF Representation ..."
        time.sleep(0.5)
        normalizedtf_score = model(
            representation.normalizedtf, n_iters=n_iters["normalizedtf"], rep="normalizedtf"
        )
        print

    print "SCORES FOR " + label.upper() + ":"

    if "bow" in n_iters:
        print "%17s: %6f" % ("BOW", bow_score)
    if "glove" in n_iters:
        print "%17s: %6f" % ("GLOVE", glove_score)
    if "word2vec" in n_iters:
        print "%17s: %6f" % ("WORD2VEC", word2vec_score)
    if "tfidf" in n_iters:
        print "%17s: %6f" % ("TF-IDF", tfidf_score)
    if "normalizedtf" in n_iters:
        print "%17s: %6f" % ("NORMALIZED TF", normalizedtf_score)
