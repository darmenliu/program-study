from __future__ import absolute_import
from __future__ import division
from __future__ import print_function
import argparse
import sys
import tensorflow as tf
from tensorflow.examples.tutorials.mnist import input_data

def variable_summaries(var):
    """Attach a lot of summaries to a Tensor (for TensorBoard visualization)."""
    with tf.name_scope('summaries'):
      mean = tf.reduce_mean(var)
      tf.summary.scalar('mean', mean)
      with tf.name_scope('stddev'):
        stddev = tf.sqrt(tf.reduce_mean(tf.square(var - mean)))
      tf.summary.scalar('stddev', stddev)
      tf.summary.scalar('max', tf.reduce_max(var))
      tf.summary.scalar('min', tf.reduce_min(var))
      tf.summary.histogram('histogram', var)

#read data from the minst respository
train_dir = '/tmp/tensorflow/mnist/input_data'
with tf.name_scope("read_data"):
    minst_data = input_data.read_data_sets(train_dir, one_hot=True)

#this is the sensor model.
with tf.name_scope("sensor_input_x"):
    x = tf.placeholder(tf.float32, [None, 784])
    y_target = tf.placeholder(tf.float32, [None, 10])

with tf.name_scope("sensor_weight"):
    W = tf.Variable(tf.zeros([784, 10]))
    variable_summaries(W)

with tf.name_scope("sensor_biase"):
    b = tf.Variable(tf.zeros([10]))
    variable_summaries(b)

with tf.name_scope("sensor_linear_model"):
    v = tf.matmul(x, W) + b
    variable_summaries(v)

with tf.name_scope("sensor_sigmoid"):
    y = 10 * tf.sigmoid(v)
    variable_summaries(y)
    tf.summary.histogram('pre_activations', y)
    activations = tf.nn.relu(y, name='activation')
    tf.summary.histogram('activations', activations)
#target
with tf.name_scope("entropy"):
    cross_entropy = tf.reduce_mean(
        tf.nn.softmax_cross_entropy_with_logits(labels=y_target, logits=y))
    tf.summary.scalar('cross_entropy', cross_entropy)

with tf.name_scope("train"):
    train_step = tf.train.GradientDescentOptimizer(0.5).minimize(cross_entropy)

with tf.name_scope("correct_prediction"):
    correct_prediction = tf.equal(tf.argmax(y, 1), tf.argmax(y_target, 1))

with tf.name_scope("accuracy"):
    accuracy = tf.reduce_mean(tf.cast(correct_prediction, tf.float32))
    tf.summary.scalar('accuracy', accuracy)

session = tf.InteractiveSession()
tf.global_variables_initializer().run()

merged_summary_op = tf.summary.merge_all()
train_writer = tf.summary.FileWriter('/tmp/mnist_sensor/train', session.graph)
test_writer = tf.summary.FileWriter('/tmp/mnist_sensor/test', session.graph)


for i in range(4000):
    batch_xs, batch_ys = minst_data.train.next_batch(100)
    summary, _train = session.run([merged_summary_op, train_step], feed_dict={x: batch_xs, y_target: batch_ys})
    train_writer.add_summary(summary, i)
    if i % 10 == 0:  # Record summaries and test-set accuracy
        summary, acc = session.run([merged_summary_op, accuracy],
                                feed_dict={x: minst_data.test.images,
                                y_target: minst_data.test.labels})
        print('Accuracy at step %s: %s' % (i, acc))
        test_writer.add_summary(summary, i)

train_writer.close()
test_writer.close()










