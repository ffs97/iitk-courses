from includes.utils import load_data

import tensorflow as tf

# sess = tf.Session()

dataset = load_data()

print((dataset.val_course_labels == 0).sum())
print((dataset.train_course_labels == 0).sum())
