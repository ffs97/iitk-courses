import tensorflow as tf

from includes.layers import *


_layers_id_mapping = {
    "ll": Layer,
    "fc": FullyConnected,
    "cn": Convolution,
    "mp": MaxPooling,
    "bn": BatchNormalization
}


class DeepNetwork:
    def __init__(self, name, layers, outputs, activation=tf.nn.relu, initializer=tf.contrib.layers.xavier_initializer):
        self.name = name

        self.layers = []
        with tf.variable_scope(self.name):
            for index, (layer_id, args) in enumerate(layers):
                name = "layer_%d" % (index + 1)

                if layer_id not in _layers_id_mapping:
                    raise NotImplementedError

                self.layers.append(
                    _layers_id_mapping[layer_id](
                        name, activation=activation, initializer=initializer, **args
                    )
                )

            self.output_layers = []
            for index, (layer_id, args) in enumerate(outputs):
                name = "output_%d" % (index + 1)

                if layer_id not in _layers_id_mapping:
                    raise NotImplementedError

                self.output_layers.append(
                    _layers_id_mapping[layer_id](
                        name, activation=None, initializer=initializer, **args
                    )
                )

    def __call__(self, inputs):
        with tf.name_scope(self.name):
            outputs = inputs

            for layer in self.layers:
                outputs = layer(outputs)

            outputs = [layer(outputs) for layer in self.output_layers]

            if len(outputs) == 1:
                outputs = outputs[0]

            return outputs


def get_encoder_networks(model, dataset="reuters"):
    if dataset == "mnist":
        encoder_shared_ = DeepNetwork(
            "layers",
            [
                ("cn", {
                    "n_kernels": 32, "prev_n_kernels": 1, "kernel": (3, 3)
                }),
                ("mp", {"k": 2}),
                ("cn", {
                    "n_kernels": 32, "prev_n_kernels": 32, "kernel": (3, 3)
                }),
                ("mp", {"k": 2})
            ],
            [
                ("fc", {"input_dim": 1568, "output_dim": 250})
            ],
            activation=model.activation, initializer=model.initializer
        )

        def encoder_shared(x):
            return encoder_shared_(
                tf.reshape(x, model.input_shape)
            )

        # encoder_shared = DeepNetwork(
        #     "layers",
        #     [
        #         ("fc", {"input_dim": model.input_dim, "output_dim": 512}),
        #         ("fc", {"input_dim": 512, "output_dim": 256})
        #     ],
        #     [
        #         ("fc", {"input_dim": 256, "output_dim": 100})
        #     ],
        #     activation=model.activation, initializer=model.initializer
        # )

        with tf.variable_scope("z"):
            network_z = DeepNetwork(
                "layers",
                [
                    # ("fc", {"input_dim": 500, "output_dim": 250})
                ],
                [
                    ("fc", {"input_dim": 250, "output_dim": model.latent_dim}),
                    ("fc", {"input_dim": 250, "output_dim": model.latent_dim})
                ],
                activation=model.activation, initializer=model.initializer
            )

        with tf.variable_scope("c"):
            network_c = DeepNetwork(
                "layers",
                [
                    # ("fc", {"input_dim": 500, "output_dim": 250}),
                    # ("bn", {"input_dim": 100, "is_training": model.is_training}),
                    ("fc", {"input_dim": 250, "output_dim": 100}),
                    ("fc", {"input_dim": 100, "output_dim": 50}),
                    # ("bn", {"input_dim": 50, "is_training": model.is_training})
                    # ("fc", {"input_dim": 500, "output_dim": 250}),
                ],
                [
                    ("fc", {"input_dim": 50, "output_dim": model.n_classes})
                ],
                activation=model.activation, initializer=model.initializer
            )

        def encoder_z(inputs):
            hidden = encoder_shared(inputs)
            hidden = tf.nn.relu(hidden)

            return network_z(hidden)

        def encoder_c(inputs):
            hidden = encoder_shared(inputs)
            hidden = tf.nn.relu(hidden)

            return network_c(hidden)

        return encoder_z, encoder_c

    elif dataset == "reuters":
        with tf.variable_scope("z"):
            network_z = DeepNetwork(
                "layers",
                [
                    ("fc", {"input_dim": model.input_dim, "output_dim": 500}),
                    ("fc", {"input_dim": 500, "output_dim": 500}),
                    ("fc", {"input_dim": 500, "output_dim": 2000}),
                ],
                [
                    ("fc", {"input_dim": 2000, "output_dim": model.latent_dim}),
                    ("fc", {"input_dim": 2000, "output_dim": model.latent_dim})
                ],
                activation=model.activation, initializer=model.initializer
            )

        with tf.variable_scope("c"):
            network_c = DeepNetwork(
                "layers",
                [
                    ("fc", {"input_dim": model.input_dim, "output_dim": 1000}),
                    ("bn", {"input_dim": 1000, "is_training": model.is_training}),
                    ("fc", {"input_dim": 1000, "output_dim": 500}),
                    # ("bn", {"input_dim": 500, "is_training": model.is_training})
                ],
                [
                    ("fc", {"input_dim": 500, "output_dim": model.n_classes})
                ],
                activation=model.activation, initializer=model.initializer
            )

        return network_z, network_c

    else:
        raise NotImplementedError
