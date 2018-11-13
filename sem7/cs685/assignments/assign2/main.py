import os
import pandas as pd

from optparse import OptionParser
from sklearn.externals import joblib

parser = OptionParser()
parser.add_option("-t", "--training",
                  action="store_true",
                  default=False)

(options, _) = parser.parse_args()

if options.training:
    from sklearn.svm import SVC
    from sklearn.naive_bayes import GaussianNB
    from sklearn.ensemble import VotingClassifier
    from sklearn.tree import DecisionTreeClassifier
    from sklearn.neural_network import MLPClassifier
    from sklearn.ensemble import AdaBoostClassifier, RandomForestClassifier
    from sklearn.ensemble import GradientBoostingClassifier, ExtraTreesClassifier

    if not os.path.exists("data/data.csv"):
        print "Training data file not found"
        raise IOError

    data = pd.read_csv("data/data.csv")

    data["c"] = data["class"].astype("category")

    X = data[["x1", "x2", "x3"]]
    Y = data["c"]

    mlp_classifier = MLPClassifier(
        hidden_layer_sizes=(20, 50, 100),
        activation="relu",
        batch_size=min(100, len(data)),
        max_iter=5000
    )
    svc_classifier = SVC(
        C=1000.0,
        gamma="auto"
    )
    dt_classifier = DecisionTreeClassifier(
        max_depth=5
    )
    rf_classifier = RandomForestClassifier(
        max_depth=5,
        n_estimators=100
    )
    ada_classifier = AdaBoostClassifier(
        base_estimator=DecisionTreeClassifier(max_depth=3),
        n_estimators=100
    )
    gnb_classifier = GaussianNB()
    gb_classifier = GradientBoostingClassifier(
        n_estimators=100
    )
    et_classifier = ExtraTreesClassifier(n_estimators=500)

    classifier = VotingClassifier(
        estimators=[
            ("mlp", mlp_classifier),
            ("svc", svc_classifier),
            ("ada", ada_classifier),
            ("gnb", gnb_classifier),
            ("gb", gb_classifier),
            ("et", et_classifier),
            ("dt", dt_classifier),
            ("rf", rf_classifier),
        ],
        voting="hard"
    )

    classifier.fit(X, Y)

    joblib.dump(classifier, "model.joblib")

    print "Model saved to model.joblib"

else:
    if not os.path.exists("model.joblib"):
        print "File not found: model.joblib"
        raise IOError
    if not os.path.exists("input.csv"):
        print "File not found: input.csv"
        raise IOError

    data = pd.read_csv("input.csv", header=None)

    classifier = joblib.load("model.joblib")
    labels = classifier.predict(data)

    for label in labels:
        print label
