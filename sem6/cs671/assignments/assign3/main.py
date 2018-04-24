import os
import sys
import pickle
import numpy as np

from includes import config
from includes import actions
from includes.utils import load_data_file, load_vectors

from sklearn.svm import SVC
from sklearn.neural_network import MLPClassifier
from sklearn.linear_model import LogisticRegression

reload(sys)
sys.setdefaultencoding('utf8')

train_features, train_outputs = None, None
test_features, test_outputs = None, None


def load_data(type):
    if os.path.exists("data/" + type + "-outputs.npy") and os.path.exists("data/" + type + "-features.npy"):
        print "Loading " + type + "ing data ..."
        features = np.load("data/" + type + "-features.npy")
        outputs = np.load("data/" + type + "-outputs.npy")
        print "Done"

    else:
        if actions.glove_vectors is None:
            print "Loading GLoVE ..."
            actions.glove_vectors = load_vectors(
                'data/glove.txt'
            )
            print "Done"

        print "Loading " + type + "ing data ..."
        data = load_data_file("data/" + type + ".conllu")
        print "Done"

        print "Processing " + type + "ing data ..."
        outputs = []
        features = []

        for sentence in data:
            sentence_outputs = []
            sentence_features = []

            stack = [0]
            buffer = [i for i in range(len(sentence), 0, -1)]

            dt = {}

            while len(buffer) != 0:
                sentence_features.append(actions.features(
                    stack, buffer, dt, sentence
                ))

                stack, buffer, dt, step, relation = actions.step(
                    stack, buffer, dt, sentence
                )

                sentence_outputs.append(actions.output(step, relation))

            features.append(sentence_features)
            outputs.append(sentence_outputs)

        outputs = np.array(outputs)
        features = np.array(features)

        np.save("data/" + type + "-outputs.npy", outputs)
        np.save("data/" + type + "-features.npy", features)

        print "Done"

    return features, outputs


def main(model, name, train=False):
    global train_features, train_outputs
    global test_features, test_outputs

    if train:
        if train_features == None:
            train_features, train_outputs = load_data("train")

            train_outputs = np.concatenate(train_outputs, axis=0)
            train_features = np.concatenate(train_features, axis=0)

        if name == "mlp":
            model.fit(train_features, train_outputs)
        else:
            model.fit(train_features, np.argmax(train_outputs, axis=1))

        with open("model-" + name + ".pkl", "wb") as f:
            pickle.dump(model, f)

    if test_features == None:
        test_features, test_outputs = load_data("test")

    correct = 0
    predictions = 0
    if name == "mlp":
        for features, outputs in zip(test_features, test_outputs):
            for i, prediction in enumerate(model.predict(features)):
                predictions += 1
                if np.all(prediction == outputs[i]):
                    correct += 1
    else:
        for features, outputs in zip(test_features, np.argmax(test_outputs), axis=1):
            for i, prediction in enumerate(model.predict(features)):
                predictions += 1
                if np.all(prediction == outputs[i]):
                    correct += 1

    print name.upper() + ": Test Accuracy: %f" % (float(correct) / float(predictions))


if __name__ == "__main__":
    if os.path.exists("model-mlp.pkl"):
        with open("model-mlp.pkl", "rb") as f:
            model = pickle.load(f)
            train = False
    else:
        model = MLPClassifier(
            hidden_layer_sizes=config.model_size,
            random_state=1,
            tol=0.0005,
            verbose=True
        )
        train = True

    main(model, "mlp", train)

    if os.path.exists("model-svc.pkl"):
        with open("model-svc.pkl", "rb") as f:
            model = pickle.load(f)
            train = False
    else:
        model = SVC(verbose=True)
        train = True

    main(model, "svc", train)

    if os.path.exists("model-log.pkl"):
        with open("model-log.pkl", "rb") as f:
            model = pickle.load(f)
            train = False
    else:
        model = LogisticRegression(verbose=True)
        train = True

    main(model, "log", train)
