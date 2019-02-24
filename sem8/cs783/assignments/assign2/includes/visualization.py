import os
import numpy as np
import tensorflow as tf
import matplotlib as mpl

from sklearn.manifold import TSNE

from matplotlib import pyplot as plt
from matplotlib import gridspec as grid


mpl.rc_file_defaults()


colors = ["dimgray", "gainsboro", "firebrick", "maroon", "cadetblue",
          "orange", "burlywood", "mediumslateblue", "palevioletred", "navy"]


def mnist_regeneration_plot(model, data, sess):
    if not os.path.exists("plots/%s/mnist" % model.name):
        os.makedirs("plots/%s/mnist" % model.name)

    gs = grid.GridSpec(1, 2)

    ax1 = plt.subplot(gs[0])
    ax2 = plt.subplot(gs[1])

    def reshape(images):
        images = images.reshape((10, 10, 28, 28))
        images = np.concatenate(np.split(images, 10, axis=0), axis=3)
        images = np.concatenate(np.split(images, 10, axis=1), axis=2)
        images = np.squeeze(images)

        return images

    orig_X = data.data[:100]

    feed = model.sample_reparametrization_variables(len(orig_X))
    for var in feed:
        feed[var] = np.zeros(feed[var].shape)
    feed.update({
        model.X: orig_X,
    })

    recn_X = sess.run(model.reconstructed_X, feed_dict=feed)

    ax1.imshow(reshape(orig_X), cmap="Greys_r")
    ax2.imshow(reshape(recn_X), cmap="Greys_r")

    ax2.spines['left'].set_visible(False)
    ax2.spines['bottom'].set_visible(False)
    ax1.spines['left'].set_visible(False)
    ax1.spines['bottom'].set_visible(False)

    ax1.get_xaxis().set_visible(False)
    ax1.get_yaxis().set_visible(False)
    ax2.get_xaxis().set_visible(False)
    ax2.get_yaxis().set_visible(False)

    plt.tight_layout()
    plt.savefig("plots/%s/mnist/regenerated.png" % model.name)
    plt.close()


def mnist_sample_plot(model, sess, tsne=False):
    global colors

    if not os.path.exists("plots/%s/mnist" % model.name):
        os.makedirs("plots/%s/mnist" % model.name)

    figure = np.zeros((28 * model.n_classes, 280))

    sample_Z = []
    for i in range(0, model.n_classes):
        kwargs = {
            "Z": {
                "session": sess,
                "c": i
            }
        }

        sample_Z_ = model.sample_generative_feed(1000, **kwargs)["Z"]
        sample_Z.append(sample_Z_)

        eps = sample_Z_[:100]
        out = sess.run(model.reconstructed_X, feed_dict={model.Z: eps})

        for j in range(10):
            figure[i * 28: (i + 1) * 28, j * 28: (j + 1) *
                   28] = out[10 * i + j].reshape((28, 28)) * 255

    if tsne:
        gs = grid.GridSpec(1, 2)

        ax1 = plt.subplot(gs[0])
        ax2 = plt.subplot(gs[1])

        sample_Z = np.concatenate(sample_Z, axis=0)
        if sample_Z.shape[1] > 2:
            sample_Z = TSNE(n_components=2).fit_transform(sample_Z)

        for k in range(0, 10):
            ax2.scatter(
                sample_Z[1000*k:1000*(k+1), 0],
                sample_Z[1000*k:1000*(k+1), 1],
                s=0.5, color=colors[k % 10]
            )

        ax2.spines['left'].set_visible(False)
        ax2.spines['bottom'].set_visible(False)

        ax2.get_xaxis().set_visible(False)
        ax2.get_yaxis().set_visible(False)

    else:
        ax1 = plt.axes()

    ax1.imshow(figure, cmap="Greys_r")

    ax1.spines['left'].set_visible(False)
    ax1.spines['bottom'].set_visible(False)

    ax1.get_xaxis().set_visible(False)
    ax1.get_yaxis().set_visible(False)

    plt.tight_layout()
    plt.savefig("plots/%s/mnist/sampled.png" % model.name)
    plt.close()


def cifar10_regeneration_plot(model, data, sess):
    if not os.path.exists("plots/%s/cifar10" % model.name):
        os.makedirs("plots/%s/cifar10" % model.name)

    gs = grid.GridSpec(1, 2)

    ax1 = plt.subplot(gs[0])
    ax2 = plt.subplot(gs[1])

    def reshape(images):
        images = images.reshape((10, 10, 32, 32, 3))
        images = np.concatenate(np.split(images, 10, axis=0), axis=3)
        images = np.concatenate(np.split(images, 10, axis=1), axis=2)
        images = np.squeeze(images)

        return images

    orig_X = data.data[:100]

    feed = model.sample_reparametrization_variables(len(orig_X))
    for var in feed:
        feed[var] = np.zeros(feed[var].shape)
    feed.update({
        model.X: orig_X,
    })

    recn_X = sess.run(model.reconstructed_X, feed_dict=feed)

    ax1.imshow(reshape(orig_X))
    ax2.imshow(reshape(recn_X))

    ax2.spines['left'].set_visible(False)
    ax2.spines['bottom'].set_visible(False)
    ax1.spines['left'].set_visible(False)
    ax1.spines['bottom'].set_visible(False)

    ax1.get_xaxis().set_visible(False)
    ax1.get_yaxis().set_visible(False)
    ax2.get_xaxis().set_visible(False)
    ax2.get_yaxis().set_visible(False)

    plt.tight_layout()
    plt.savefig("plots/%s/cifar10/regenerated.png" % model.name)
    plt.close()


def spiral_regeneration_plot(model, data, sess):
    if not os.path.exists("plots/%s/spiral" % model.name):
        os.makedirs("plots/%s/spiral" % model.name)

    gs = grid.GridSpec(1, 2)

    ax1 = plt.subplot(gs[0])
    ax2 = plt.subplot(gs[1])

    orig_X = data.data

    feed = model.sample_reparametrization_variables(len(orig_X))
    feed.update({
        model.X: orig_X
    })

    recn_X = sess.run(model.reconstructed_X, feed_dict=feed)

    ax1.scatter(orig_X[:, 0], orig_X[:, 1], s=0.5)
    ax2.scatter(recn_X[:, 0], recn_X[:, 1], s=0.5)

    ax2.spines['left'].set_visible(False)
    ax2.spines['bottom'].set_visible(False)
    ax1.spines['left'].set_visible(False)
    ax1.spines['bottom'].set_visible(False)

    ax1.get_xaxis().set_visible(False)
    ax1.get_yaxis().set_visible(False)
    ax2.get_xaxis().set_visible(False)

    plt.tight_layout()
    plt.savefig("plots/%s/spiral/regenerated.png" % model.name)
    plt.close()


def spiral_sample_plot(model, sess):
    global colors

    if not os.path.exists("plots/%s/spiral" % model.name):
        os.makedirs("plots/%s/spiral" % model.name)

    gs = grid.GridSpec(1, 2)

    ax1 = plt.subplot(gs[0])
    ax2 = plt.subplot(gs[1])

    sample_Z = []
    for k in range(0, 5):
        kwargs = {
            "Z": {
                "session": sess,
                "c": k
            }
        }

        sample_Z_ = model.sample_generative_feed(1000, **kwargs)["Z"]
        sample_Z.append(sample_Z_)

        recn_X = sess.run(
            model.reconstructed_X, feed_dict={
                model.Z: sample_Z_
            }
        )

        ax1.scatter(recn_X[:, 0], recn_X[:, 1], s=0.5, color=colors[k])

    sample_Z = np.concatenate(sample_Z, axis=0)
    if sample_Z.shape[1] > 2:
        sample_Z = TSNE(n_components=2).fit_transform(sample_Z)

    for k in range(0, 5):
        ax2.scatter(
            sample_Z[1000*k:1000*(k+1), 0],
            sample_Z[1000*k:1000*(k+1), 1],
            s=0.5, color=colors[k % 10]
        )

    ax2.spines['left'].set_visible(False)
    ax2.spines['bottom'].set_visible(False)
    ax1.spines['left'].set_visible(False)
    ax1.spines['bottom'].set_visible(False)

    ax1.get_xaxis().set_visible(False)
    ax1.get_yaxis().set_visible(False)
    ax2.get_xaxis().set_visible(False)

    plt.tight_layout()
    plt.savefig("plots/%s/spiral/sampled.png" % model.name)
    plt.close()
