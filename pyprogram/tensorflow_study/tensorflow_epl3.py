import tensorflow as tf

weight = tf.Variable(tf.random_normal([10, 20], stddev=2.0), name="weitht")

biases = tf.Variable(tf.zeros([20]), name="biases")

saver = tf.train.Saver()

init_op = tf.global_variables_initializer()

with tf.Session() as sess:
    print(sess.run(init_op))
    print(sess.run(weight))
    print(sess.run(biases))

    save_path = saver.save(sess, "model.ckpt")
    print("Model saved in file:%s" % save_path)