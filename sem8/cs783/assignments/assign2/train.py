import os
import math
# import models
import argparse
import warnings
import base_models
import numpy as np
import tensorflow as tf

from tqdm import tqdm

from includes.utils import load_data
import includes.visualization as visualization

tf.logging.set_verbosity(tf.logging.ERROR)
warnings.simplefilter(action='ignore', category=FutureWarning)

parser = argparse.ArgumentParser(
    description="Training file for CS783: Assignment 2"
)


parser.add_argument("--model_name", type=str, default="",
                    help="Name of the model")

parser.add_argument("--n_clusters", type=int, default=-1,
                    help="Number of clusters to use")
parser.add_argument("--n_experts", type=int, default=5,
                    help="Number of experts to use for MoE part")

parser.add_argument("--n_epochs", type=int, default=500,
                    help="Number of epochs for training the model")
parser.add_argument("--pretrain_epochs", type=int, default=200,
                    help="Number of epochs for pretraining the model")

parser.add_argument("--init_lr", type=float, default=0.002,
                    help="Initial learning rate for training")
parser.add_argument("--decay_rate", type=float, default=0.9,
                    help="Decay rate for exponentially decaying learning rate (< 1.0)")
parser.add_argument("--decay_epochs", type=int, default=25,
                    help="Number of epochs between exponentially decay of learning rate")

parser.add_argument("--pretrain", action="store_true", default=False,
                    help="Whether to pretrain the model or not")

parser.add_argument("--pretrain_lr", type=float, default=0.0005,
                    help="Initial learning rate for pretraining the vae")
parser.add_argument("--pretrain_decay_rate", type=float, default=0.9,
                    help="Decay rate for exponentially decaying learning rate (< 1.0) for pretraining")
parser.add_argument("--pretrain_decay_epochs", type=int, default=25,
                    help="Number of epochs between exponentially decay of learning rate for pretraining")

parser.add_argument("--kl_annealing", action="store_true", default=False,
                    help="Whether to anneal the KL term while training or not")
parser.add_argument("--anneal_step", type=float, default=0.1,
                    help="Step size for annealing")
parser.add_argument("--anneal_epochs", type=int, default=1000,
                    help="Number of epochs before annealing the KL term")

parser.add_argument("--plot_epochs", type=int, default=10,
                    help="Nummber of epochs before generating plots")

parser.add_argument("--save_epochs", type=int, default=10,
                    help="Nummber of epochs before saving model")


def main(argv):
    model_name = argv.model_name

    save_epochs = argv.save_epochs

    init_lr = argv.init_lr
    decay_rate = argv.decay_rate
    decay_epochs = argv.decay_epochs

    n_epochs = argv.n_epochs

    pretrain = argv.pretrain
    pretrain_epochs = argv.pretrain_epochs

    pretrain_lr = argv.pretrain_lr
    pretrain_decay_rate = argv.pretrain_decay_rate
    pretrain_decay_epochs = argv.pretrain_decay_epochs

    kl_annealing = args.kl_annealing
    anneal_step = args.anneal_step
    anneal_epochs = args.anneal_epochs

    dataset = load_data()

    if model_name == "":
        model_name = "imsat"

    n_clusters = argv.n_clusters
    if n_clusters < 1:
        n_clusters = dataset.n_course_classes

    from includes.utils import Dataset

    model = base_models.IMSAT(
        model_name, dataset.input_dim, dataset.input_shape, n_clusters, lam=0.2, xi=10,
        activation=tf.nn.relu, initializer=tf.contrib.layers.xavier_initializer
    ).build_graph()

    val_data = (dataset.val_data, dataset.val_course_labels)
    train_data = (dataset.train_data, dataset.train_course_labels)

    val_data = Dataset(val_data, batch_size=100)
    train_data = Dataset(train_data, batch_size=100)

    model.define_train_step(
        init_lr, train_data.epoch_len * decay_epochs, decay_rate
    )

    if pretrain:
        model.define_pretrain_step(
            pretrain_lr, train_data.epoch_len *
            pretrain_decay_epochs, pretrain_decay_rate
        )

    model.path = "saved-models/%s" % (model.name)
    print("Model Path:", model.path)
    if not os.path.exists(model.path):
        os.makedirs(model.path)

    sess = tf.Session()
    tf.global_variables_initializer().run(session=sess)

    if pretrain:
        model.pretrain(
            sess, train_data, pretrain_epochs
        )

    var_list = tf.get_collection(tf.GraphKeys.TRAINABLE_VARIABLES)
    saver = tf.train.Saver(var_list)
    ckpt_path = model.path + "/model/parameters.ckpt"

    try:
        saver.restore(sess, ckpt_path)
    except:
        print("Could not load trained model")

    with tqdm(range(n_epochs)) as bar:
        bar.set_postfix({
            "max_acc": "%.4f" % 0.0,
            "val_acc": "%.4f" % 0.0,
            "train_acc": "%.4f" % 0.0,
            "loss": "%.4f" % float("inf")
        })

        max_acc = float("-inf")
        anneal_term = 0.0 if kl_annealing else 1.0

        for epoch in bar:
            if kl_annealing and (epoch + 1) % anneal_epochs == 0:
                anneal_term = min(anneal_term + anneal_step, 1.0)

            if epoch % save_epochs == 0:
                val_acc = model.get_accuracy(sess, val_data)
                train_acc = model.get_accuracy(sess, train_data)

                if val_acc > max_acc:
                    max_acc = val_acc
                    saver.save(sess, ckpt_path)

            loss = model.train_op(sess, train_data)

            bar.set_postfix({
                "loss": "%.4f" % loss,
                "max_acc": "%.4f" % max_acc,
                "val_acc": "%.4f" % val_acc,
                "train_acc": "%.4f" % train_acc
            })

    fl = open("logs/%s.txt" % model_name, "a+")
    fl.write("\n%s\n------\n" % str(argv))
    fl.write("Max Accuracy        %.4f\n============" % max_acc)


if __name__ == "__main__":
    args = parser.parse_args()

    if False:
        from pprint import PrettyPrinter

        pp = PrettyPrinter(indent=2)
        pp.pprint(vars(args))

    main(args)
