
from __future__ import absolute_import
from __future__ import division
from __future__ import print_function

import argparse
import sys

import tensorflow as tf

from tensorflow.examples.tutorials.mnist import input_data
#read data from the minst respository
train_dir = '/tmp/tensorflow/mnist/input_data'
minst_data = input_data.read_data_sets(train_dir, one_hot=True)

#this is the sensor model.
x = tf.placeholder(tf.float32, [None, 784])
W = tf.Variable(tf.zeros([784, 10]))
b = tf.Variable(tf.zeros([10]))
v = tf.matmul(x, W) + b
y = 10 * tf.sigmoid(v)
#target
y_target = tf.placeholder(tf.float32, [None, 10])
cross_entropy = tf.reduce_mean(
  tf.nn.softmax_cross_entropy_with_logits(labels=y_target, logits=y))
train_step = tf.train.GradientDescentOptimizer(0.5).minimize(cross_entropy)

session = tf.InteractiveSession()
tf.global_variables_initializer().run()

merged_summary_op = tf.summary.merge_all()
summary_writer = tf.summary.FileWriter('/tmp/mnist_sensor_model.graph', session.graph)
for i in range(4000):
    batch_xs, batch_ys = minst_data.train.next_batch(100)
    summary = session.run(train_step, feed_dict={x: batch_xs, y_target: batch_ys})
    summary_writer.add_summary(summary, i)


correct_prediction = tf.equal(tf.argmax(y, 1), tf.argmax(y_target, 1))
accuracy = tf.reduce_mean(tf.cast(correct_prediction, tf.float32))
print(session.run(accuracy, feed_dict={x: minst_data.test.images,
                                  y_target: minst_data.test.labels}))










