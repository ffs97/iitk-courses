import tensorflow as tf


class Layer:
    def __init__(self, name, activation=tf.nn.relu, initializer=tf.contrib.layers.xavier_initializer):
        self.name = name

        self.activation = activation
        self.initializer = initializer

    def _call(self, inputs):
        return inputs

    def __call__(self, inputs):
        with tf.name_scope(self.name):
            return self._call(inputs)


class FullyConnected(Layer):
    def __init__(self, name, input_dim, output_dim, activation=tf.nn.relu, initializer=tf.contrib.layers.xavier_initializer):
        Layer.__init__(self, name, activation=activation,
                       initializer=initializer)

        with tf.variable_scope(self.name):
            self.W = tf.get_variable("weight", shape=(input_dim, output_dim),
                                     initializer=initializer())
            self.b = tf.get_variable("bias", shape=(1, output_dim),
                                     initializer=initializer())

    def _call(self, inputs):
        outputs = tf.layers.flatten(inputs)
        outputs = tf.matmul(outputs, self.W) + self.b

        if self.activation is not None:
            outputs = self.activation(outputs)

        return outputs


class Convolution(Layer):
    def __init__(self, name, n_kernels, prev_n_kernels, kernel, strides=1, activation=tf.nn.relu, initializer=tf.contrib.layers.xavier_initializer):
        Layer.__init__(self, name, activation=activation,
                       initializer=initializer)

        self.strides = [1, strides, strides, 1]

        weights_shape = list(kernel) + [prev_n_kernels] + [n_kernels]

        with tf.variable_scope(self.name):
            self.W = tf.get_variable("weight", shape=weights_shape,
                                     initializer=initializer())
            self.b = tf.get_variable("bias", shape=(n_kernels,),
                                     initializer=initializer())

    def _call(self, inputs):
        outputs = tf.nn.conv2d(
            inputs, self.W, strides=self.strides, padding='SAME'
        )
        outputs = tf.nn.bias_add(outputs, self.b)

        if self.activation is not None:
            outputs = self.activation(outputs)

        return outputs


class MaxPooling(Layer):
    def __init__(self, name, k, activation=None, initializer=None):
        Layer.__init__(self, name, activation=activation,
                       initializer=initializer)

        self.ksize = [1, k, k, 1]
        self.strides = [1, k, k, 1]

    def _call(self, inputs):
        outputs = tf.nn.max_pool(
            inputs, ksize=self.ksize, strides=self.strides, padding='SAME'
        )

        return outputs


class BatchNormalization(Layer):
    def __init__(self, name, input_dim, is_training, activation=None, initializer=tf.contrib.layers.xavier_initializer):
        Layer.__init__(self, name, activation=activation,
                       initializer=initializer)

        self.input_dim = input_dim
        self.is_training = is_training

        with tf.variable_scope(self.name):
            self.W = tf.get_variable("weight", (input_dim, input_dim),
                                     initializer=initializer())

            self.beta = tf.get_variable("beta", (input_dim,),
                                        initializer=tf.initializers.zeros)
            self.scale = tf.get_variable("scale", (input_dim,),
                                         initializer=tf.initializers.ones)

    def _call(self, inputs):
        batch_mean, batch_var = tf.nn.moments(inputs, [0])

        outputs = tf.nn.batch_normalization(
            inputs, batch_mean, batch_var, self.beta, self.scale, 1e-20
        )

        return outputs
