import os
import numpy as np
import tensorflow as tf

from tqdm import tqdm
from sklearn.mixture import GaussianMixture

from includes.utils import get_clustering_accuracy
from includes.layers import Convolution, MaxPooling
from includes.network import DeepNetwork, get_encoder_networks


class IMSAT:
    def __init__(self, name, input_dim, input_shape, n_classes, mu=4,
                 lam=0.2, xi=10, Ip=1, eps=1, activation=None, initializer=None):

        self.name = name

        self.input_dim = input_dim
        self.input_shape = input_shape

        self.activation = activation
        self.initializer = initializer

        self.n_classes = n_classes

        self.path = ""

        self.is_training = tf.placeholder_with_default(
            True, shape=None, name="is_training"
        )

        self.mu = mu
        self.lam = lam

        self.xi = xi
        self.Ip = Ip
        self.eps = eps

        self.is_training = tf.placeholder_with_default(
            False, shape=None, name="is_training"
        )

    def build_graph(self):
        with tf.variable_scope(self.name) as _:
            self.X = tf.placeholder(
                tf.float32, shape=(None, self.input_dim), name="X"
            )

            with tf.variable_scope("network"):
                network = DeepNetwork(
                    "layers",
                    [
                        ("fc", {
                            "input_dim": self.input_dim,
                            "output_dim": 1200
                        }),
                        ("bn", {
                            "input_dim": 1200, "is_training": self.is_training
                        }),
                        ("fc", {
                            "input_dim": 1200, "output_dim": 1200
                        }),
                        ("bn", {
                            "input_dim": 1200, "is_training": self.is_training
                        })
                    ],
                    [
                        ("fc", {"input_dim": 1200, "output_dim": self.n_classes})
                    ],
                    activation=self.activation, initializer=self.initializer
                )
                self.logits = network(self.X)

                self.cluster_probs = tf.nn.softmax(self.logits)

            with tf.variable_scope("adversary"):
                ul_logits = network(self.X)

                d = tf.random_normal(shape=tf.shape(self.X))
                d /= (tf.reshape(tf.sqrt(tf.reduce_sum(tf.pow(d, 2.0),
                                                       axis=1)), [-1, 1]) + 1e-16)
                for _ in range(self.Ip):
                    y1 = ul_logits
                    y2 = network(self.X + self.xi * d)
                    kl_loss = tf.reduce_mean(self.compute_kld(y1, y2))
                    grad = tf.gradients(kl_loss, [d])[0]
                    d = tf.stop_gradient(grad)
                    d /= (tf.reshape(tf.sqrt(tf.reduce_sum(tf.pow(d, 2.0),
                                                           axis=1)), [-1, 1]) + 1e-16)

                self.orig_example = tf.stop_gradient(ul_logits)
                self.adversary = network(self.X + d * self.eps)

        return self

    def define_train_loss(self):
        mean_probs = tf.reduce_mean(self.cluster_probs, axis=0)
        H_Y = -tf.reduce_sum(mean_probs * tf.log(mean_probs + 1e-16))
        H_Y_X = tf.reduce_mean(self.entropy(self.cluster_probs))
        self.entropy_loss = H_Y_X - self.mu * H_Y

        self.adversary_loss = tf.reduce_mean(
            self.compute_kld(self.orig_example, self.adversary))

        self.loss = self.adversary_loss + self.lam * self.entropy_loss

    def compute_kld(self, p_logit, q_logit):
        p = tf.nn.softmax(p_logit)
        q = tf.nn.softmax(q_logit)
        return tf.reduce_sum(p * (tf.log(p + 1e-16) - tf.log(q + 1e-16)), axis=1)

    def entropy(self, p):
        return -tf.reduce_sum(p * tf.log(p + 1e-16), axis=1)

    def define_train_step(self, init_lr, decay_steps, decay_rate=0.9):
        learning_rate = tf.train.exponential_decay(
            learning_rate=init_lr,
            global_step=0,
            decay_steps=decay_steps,
            decay_rate=decay_rate
        )

        self.define_train_loss()

        # for batchnorm
        update_ops = tf.get_collection(tf.GraphKeys.UPDATE_OPS)
        with tf.control_dependencies(update_ops):
            optimizer = tf.train.AdamOptimizer(learning_rate=learning_rate)
            self.train_step = optimizer.minimize(self.loss)

    def train_op(self, session, data):
        assert self.train_step is not None
        loss = 0.0
        for batch in data.get_batches():
            feed = {
                self.X: batch,
                self.is_training: True
            }
            batch_loss, _ = session.run(
                [self.loss, self.train_step],
                feed_dict=feed
            )
            loss += batch_loss / data.epoch_len
        return loss

    def get_accuracy(self, session, data):
        logits = []
        for batch in data.get_batches():
            logits.append(session.run(self.logits, feed_dict={self.X: batch}))

        logits = np.concatenate(logits, axis=0)

        return get_clustering_accuracy(logits, data.classes)
