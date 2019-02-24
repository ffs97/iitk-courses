import os
import cv2
import math
import numpy as np
import pickle as pkl
import tensorflow as tf
import scipy.sparse as sp

from glob import glob

from sklearn.utils.linear_assignment_ import linear_assignment
from includes.config import data_dir, req_w, req_h, train_val_split


def get_clustering_accuracy(weights, classes):
    clusters = np.argmax(weights, axis=-1)

    n_clusters = weights.shape[1]
    n_classes = np.max(classes) + 1

    size = len(clusters)
    d = np.zeros((n_clusters, n_classes), dtype=np.int32)

    for i in range(size):
        d[clusters[i], classes[i]] += 1

    ind = linear_assignment(d.max() - d)
    return sum([d[i, j] for i, j in ind]) / (size*1.0)


def load_data(data_dir=data_dir):
    import os.path as path

    class ImageDataset:
        pass

    dataset = ImageDataset()

    course_classes = os.listdir(data_dir)

    fine_classes = {
        course_class: os.listdir(path.join(data_dir, course_class))
        for course_class in course_classes
    }

    train_data, val_data = list(), list()
    train_fine_labels, val_fine_labels = list(), list()
    train_course_labels, val_course_labels = list(), list()

    for c_, c in enumerate(course_classes):
        for f_, f in enumerate(fine_classes[c]):
            data = [
                np.reshape(cv2.imread(p, cv2.IMREAD_GRAYSCALE), -1) for
                p in glob(path.join(data_dir, c, f, "*.jpg"))
            ]
            fine_labels = [f_] * len(data)
            course_labels = [c_] * len(data)

            indices = np.random.permutation(len(data))

            data = np.array(data)[indices]
            fine_labels = np.array(fine_labels)[indices]
            course_labels = np.array(course_labels)[indices]

            split = int(math.ceil(train_val_split * len(data)))

            val_data.extend(data[split:])
            train_data.extend(data[:split])

            val_fine_labels.extend(fine_labels[split:])
            train_fine_labels.extend(fine_labels[:split])

            val_course_labels.extend(course_labels[split:])
            train_course_labels.extend(course_labels[:split])

    dataset.val_data = np.array(val_data)
    dataset.val_fine_labels = np.array(val_fine_labels)
    dataset.val_course_labels = np.array(val_course_labels)

    dataset.train_data = np.array(train_data)
    dataset.train_fine_labels = np.array(train_fine_labels)
    dataset.train_course_labels = np.array(train_course_labels)

    dataset.n_course_classes = 5

    dataset.n_fine_classes_split = [len(f) for _, f in fine_classes.items()]
    dataset.n_fine_classes = sum(dataset.n_fine_classes_split)

    dataset.input_dim = req_w * req_h
    dataset.input_shape = (-1, req_h, req_w)

    dataset.imsat_regularization_weight = 20.0

    return dataset


class Dataset:
    def __init__(self, data, batch_size=100, shuffle=True):
        data, classes = data

        self.data = np.copy(data)
        self.classes = np.copy(classes)

        self.batch_size = batch_size

        self.shuffle = shuffle

        self.data_dim = self.data.shape[1]

        self.len = len(self.data)

        self.epoch_len = int(math.ceil(len(self.data) / batch_size))

        if shuffle:
            indices = np.random.permutation(len(self.data))

            self.data = self.data[indices]
            self.classes = self.classes[indices]

    def get_batches(self):
        if self.shuffle:
            indices = np.random.permutation(len(self.data))

            self.data = self.data[indices]
            self.classes = self.classes[indices]

        batch = []
        for row in self.data:
            batch.append(row)
            if len(batch) == self.batch_size:
                yield np.array(batch)
                batch = []
        if len(batch) > 0:
            yield np.array(batch)

    def __len__(self):
        return self.epoch_len
