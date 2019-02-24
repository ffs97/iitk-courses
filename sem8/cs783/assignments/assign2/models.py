import numpy as np
import tensorflow as tf

from tqdm import tqdm
from includes.utils import Dataset
from includes.network import DeepNetwork
from base_models import DeepMixtureVAE, VaDE, AdversarialDMVAE


class MoE:
    def __init__(self, name, input_type, input_dim, input_shape, latent_dim, output_dim, n_experts,
                 classification, activation=None, initializer=None, feature_learning=False, use_cnn=False):
        self.name = name

        self.input_dim = input_dim
        self.input_shape = input_shape

        self.latent_dim = latent_dim
        self.output_dim = output_dim

        self.input_type = input_type
        self.classification = classification

        self.n_experts = self.n_classes = n_experts

        self.activation = activation
        self.initializer = initializer

        self.feature_learning = feature_learning
        self.use_cnn = use_cnn

    def _define_vae(self):
        self.vae = None

    def _define_logits_network(self):
        raise NotImplementedError

    def define_vae(self):
        with tf.variable_scope("vae") as _:
            self._define_vae()

    def define_logits_network(self):
        with tf.variable_scope("logits_network") as _:
            if self.vae is not None:
                self.expert_probs = self.vae.cluster_probs
            else:
                self._define_logits_network()

    def build_graph(self):
        with tf.variable_scope(self.name) as _:
            self.define_vae()

            if self.vae is not None:
                self.X = self.vae.X
                self.Z = self.vae.Z

                self.reconstructed_X = self.vae.reconstructed_X

            else:
                self.X = tf.placeholder(
                    tf.float32, shape=(None, self.input_dim), name="X"
                )

            self.Y = tf.placeholder(
                tf.float32, shape=(None, self.output_dim), name="Y"
            )

            self.define_logits_network()

            # self.regression_biases = tf.get_variable(
            #     "regression_biases", dtype=tf.float32,
            #     initializer=tf.initializers.zeros,
            #     shape=(self.output_dim, self.n_experts)
            # )
            # if self.feature_learning and self.vae is not None:
            #     input_dim = self.latent_dim
            #     inp2cls = tf.nn.relu(self.vae.mean)
            # else:
            #     input_dim = self.input_dim
            #     inp2cls = self.X

            # self.regression_weights = tf.get_variable(
            #     "regression_weights", dtype=tf.float32,
            #     initializer=tf.initializers.random_normal,
            #     shape=(self.n_experts, self.output_dim, input_dim)
            # )

            # expert_predictions = tf.transpose(tf.matmul(
            #     self.regression_weights,
            #     tf.tile(
            #         tf.transpose(inp2cls)[None, :, :], [self.n_experts, 1, 1]
            #     )
            # )) + self.regression_biases

            with tf.variable_scope("discriminative_network"):
                network = DeepNetwork(
                    "layers",
                    [
                        ("cn", {
                            "n_kernels": 32, "prev_n_kernels": 1, "kernel": (5, 5)
                        }),
                        ("mp", {"k": 2}),
                        ("cn", {
                            "n_kernels": 32, "prev_n_kernels": 32, "kernel": (5, 5)
                        }),
                        ("mp", {"k": 2}),
                        ("fc", {"input_dim": 1568, "output_dim": 250})
                    ],
                    [
                        ("fc", {"input_dim": 250, "output_dim": self.output_dim})
                        for k in range(self.n_experts)
                    ],
                    activation=self.activation, initializer=self.initializer
                )

                expert_predictions = tf.stack(network(
                    tf.reshape(self.X, self.input_shape)
                ), axis=1)

            if self.classification:
                expert_class_probs = tf.nn.softmax(
                    # tf.transpose(expert_predictions, (0, 2, 1))
                    expert_predictions
                )

                unnorm_class_probs = tf.reduce_sum(
                    expert_class_probs * self.expert_probs[:, :, None], axis=1
                )
                self.reconstructed_Y_soft = unnorm_class_probs / tf.reduce_sum(
                    unnorm_class_probs, axis=-1, keep_dims=True
                )

                self.reconstructed_Y = tf.one_hot(
                    tf.reshape(
                        tf.nn.top_k(self.reconstructed_Y_soft).indices, (-1,)
                    ), self.output_dim
                )

                self.error = tf.reduce_sum(
                    tf.abs(self.Y - self.reconstructed_Y)
                ) // 2
            else:
                self.reconstructed_Y = tf.reduce_sum(
                    # expert_predictions * self.expert_probs[:, None, :], axis=-1
                    expert_predictions * self.expert_probs[:, :, None], axis=1
                )

                self.error = tf.reduce_mean(
                    tf.square(self.reconstructed_Y - self.Y)
                ) * self.output_dim

            return self

    def sample_generative_feed(self, n, **kwargs):
        if self.vae is None:
            raise NotImplementedError

        return self.vae.sample_generative_feed(n, **kwargs)

    def sample_reparametrization_variables(self, n):
        if self.vae is None:
            raise NotImplementedError

        return self.vae.sample_reparametrization_variables(n)

    def define_train_loss(self):
        if self.classification:
            self.loss = - tf.reduce_mean(tf.reduce_sum(
                self.Y * tf.log(self.reconstructed_Y_soft + 1e-20), axis=-1
            ))
        else:
            self.loss = 0.5 * tf.reduce_mean(
                tf.square(self.reconstructed_Y - self.Y)
            ) * self.output_dim

        self.loss *= 2
        # self.loss += tf.reduce_sum(tf.square(self.regression_weights)) * 2.0

        if self.vae is not None:
            self.vae.define_train_loss()
            self.loss += self.vae.loss

    def define_pretrain_step(self, init_lr, decay_steps, decay_rate=0.9):
        if self.vae is not None:
            self.vae.define_train_step(
                init_lr, decay_steps, decay_rate
            )
        else:
            raise NotImplementedError

    def define_train_step(self, init_lr, decay_steps, decay_rate=0.9, pretrain_init_lr=None,
                          pretrain_decay_steps=None, pretrain_decay_rate=None):
        self.define_train_loss()

        learning_rate = tf.train.exponential_decay(
            learning_rate=init_lr,
            global_step=0,
            decay_steps=decay_steps,
            decay_rate=decay_rate
        )

        self.train_step = tf.train.AdamOptimizer(
            learning_rate=learning_rate
        ).minimize(self.loss)

    def pretrain(self, session, data, n_epochs):
        if self.vae is None:
            raise NotImplementedError

        print("Pretraining Model")
        data = Dataset((data.data, data.classes),
                       data.batch_size, data.shuffle)

        with tqdm(range(n_epochs)) as bar:
            for _ in bar:
                self.vae.train_op(session, data)

    def train_op(self, session, data):
        assert(self.train_step is not None)

        loss = 0.0
        for X_batch, Y_batch, _ in data.get_batches():
            feed = {
                self.X: X_batch,
                self.Y: Y_batch
            }
            if self.vae is not None:
                feed.update(
                    self.vae.sample_reparametrization_variables(len(X_batch))
                )

            batch_loss, _ = session.run(
                [self.loss, self.train_step],
                feed_dict=feed
            )
            loss += batch_loss / data.epoch_len

        return loss

    def get_accuracy(self, session, data):
        error = 0.0
        logits = []
        for X_batch, Y_batch, _ in data.get_batches():
            feed = {
                self.X: X_batch,
                self.Y: Y_batch
            }
            feed.update(
                self.vae.sample_reparametrization_variables(len(X_batch))
            )

            batch_logits, batch_error = session.run(
                [self.vae.logits, self.error], feed_dict=feed)

            error += batch_error
            logits.append(batch_logits)

        logits = np.concatenate(logits, axis=0)

        if self.classification:
            error = 1 - error / data.len
        else:
            error /= -data.epoch_len

        return error

    def debug(self, session, data):
        import pdb

        for X_batch, Y_batch, _ in data.get_batches():
            feed = {
                self.X: X_batch,
                self.Y: Y_batch
            }
            feed.update(
                self.vae.sample_reparametrization_variables(len(X_batch))
            )

            pdb.set_trace()

            break


class DeepMoE(MoE):
    def __init__(self, name, input_type, input_dim, input_shape, output_dim, n_experts, classification,
                 activation=None, initializer=None, feature_learning=False, use_cnn=False):
        MoE.__init__(self, name, input_type, input_dim, input_shape, 1, output_dim, n_experts,
                     classification, activation=activation, initializer=initializer, lossVAE=0, feature_learning=feature_learning, use_cnn=use_cnn)

    def _define_logits_network(self):
        raise NotImplementedError


class DeepVariationalMoE(MoE):
    def _define_vae(self):
        with tf.variable_scope(self.name) as _:
            self.vae = DeepMixtureVAE(
                "deep_mixture_vae", self.input_type, self.input_dim, self.input_shape, self.latent_dim,
                self.n_experts, activation=self.activation, initializer=self.initializer
            ).build_graph(cnn=self.use_cnn)


class AdversarialDVMoE(MoE):
    def _define_vae(self):
        with tf.variable_scope(self.name) as _:
            self.vae = AdversarialDMVAE(
                "adversarial_dmvae", self.input_type, self.input_dim, self.input_shape, self.latent_dim,
                self.n_experts, lam=1.0, activation=self.activation, initializer=self.initializer
            ).build_graph(cnn=self.use_cnn)


class VaDEMoE(MoE):
    def _define_vae(self):
        with tf.variable_scope(self.name) as _:
            self.vae = VaDE(
                "vade", self.input_type, self.input_dim, self.input_shape, self.latent_dim,
                self.n_experts, activation=self.activation, initializer=self.initializer
            ).build_graph(cnn=self.use_cnn)
