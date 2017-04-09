from __future__ import absolute_import
from __future__ import division
from __future__ import print_function
import argparse
import sys
import tensorflow as tf
from tensorflow.examples.tutorials.mnist import input_data

# The MNIST dataset has 10 classes, representing the digits 0 through 9.
NUM_CLASSES = 10

# The MNIST images are always 28x28 pixels.
IMAGE_SIZE = 28
IMAGE_PIXELS = IMAGE_SIZE * IMAGE_SIZE
LEARNING_RATE = 0.001


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

# Input placeholders
with tf.name_scope('input'):
	x = tf.placeholder(tf.float32, [None, 784], name='x-input')
	y_ = tf.placeholder(tf.float32, [None, 10], name='y-input')

with tf.name_scope('input_reshape'):
	image_shaped_input = tf.reshape(x, [-1, 28, 28, 1])
	tf.summary.image('input', image_shaped_input, 10)

def weight_variable(shape):
	initial = tf.truncated_normal(shape, stddev=0.1)
	return tf.Variable(initial)

def bias_variable(shape):
	initial = tf.truncated_normal(shape,stddev=0.1)
	return tf.Variable(initial)

def nn_layer(input, input_dimension, output_dimention, layer_name, act_function=tf.nn.sigmoid):
	with tf.name_scope(layer_name):
		with tf.name_scope('weight'):
			weight = weight_variable([input_dimension, output_dimention])
			variable_summaries(weight)
		with tf.name_scope('bias'):
			bias = bias_variable([output_dimention])
			variable_summaries(bias)
		with tf.name_scope('linear_model_wx_b'):
			pre_activate = tf.matmul(input, weight) + bias
		activations = 10 * act_function(pre_activate)
		tf.summary.histogram('activations', activations)
		return activations

def train():
	session = tf.InteractiveSession()
	hidden_layer1 = nn_layer(x, IMAGE_PIXELS, 500, 'hidden_layer1')
	with tf.name_scope('dropout'):
		keep_prob = tf.placeholder(tf.float32)
		tf.summary.scalar('dropout_keep_probability', keep_prob)
		dropped = tf.nn.dropout(hidden_layer1, keep_prob)
	y_output = nn_layer(dropped, 500, NUM_CLASSES, 'output_layer', act_function=tf.nn.sigmoid)

	with tf.name_scope('cross_entropy'):
		diff = tf.nn.softmax_cross_entropy_with_logits(labels=y_, logits=y_output)
		with tf.name_scope('total_entropy'):
			cross_entropy = tf.reduce_mean(diff)
	tf.summary.scalar('cross_entropy',cross_entropy)
	with tf.name_scope('train'):
		train_step = tf.train.AdamOptimizer(LEARNING_RATE).minimize(cross_entropy)
	with tf.name_scope('accuracy'):
		with tf.name_scope('correct_prediction'):
			correct_prediction = tf.equal(tf.argmax(y_output, 1), tf.argmax(y_, 1))
		with tf.name_scope('accuracy'):
			accuracy = tf.reduce_mean(tf.cast(correct_prediction, tf.float32))
		tf.summary.scalar('accuracy', accuracy)
	merged = tf.summary.merge_all()
	train_writer = tf.summary.FileWriter('/tmp/mnist_sensor/train', session.graph)
	test_writer = tf.summary.FileWriter('/tmp/mnist_sensor/test', session.graph)

	tf.global_variables_initializer().run()

	def feed_dict(train):
		"""Make a TensorFlow feed_dict: maps data onto Tensor placeholders."""
		if train:
			xs, ys = minst_data.train.next_batch(100)
			k = 0.5
		else:
			xs, ys = minst_data.test.images, minst_data.test.labels
			k = 1.0
		return {x: xs, y_: ys, keep_prob: k}

	for i in range(4000):
		if i % 10 == 0:
			summary, acc = session.run([merged, accuracy], feed_dict=feed_dict(False))
			test_writer.add_summary(summary, i)
			print('Accuracy at step %s: %s' % (i, acc))
		else:
			if i % 100 == 99:
				run_options = tf.RunOptions(trace_level=tf.RunOptions.FULL_TRACE)
				run_metadata = tf.RunMetadata()
				summary, _ = session.run([merged, train_step],
				                      feed_dict=feed_dict(True),
				                      options=run_options,
				                      run_metadata=run_metadata)
				train_writer.add_run_metadata(run_metadata, 'step%03d' % i)
				train_writer.add_summary(summary, i)
				print('Adding run metadata for', i)
			else:
				summary, _ = session.run([merged, train_step], feed_dict=feed_dict(True))
				train_writer.add_summary(summary, i)
	train_writer.close()
	test_writer.close()

train()
