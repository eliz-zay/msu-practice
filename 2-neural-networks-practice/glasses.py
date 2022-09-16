import numpy as np
import tensorflow as tf
import pandas as pd
import os
from PIL import Image
from keras.preprocessing import image
from keras.preprocessing.image import ImageDataGenerator
from keras.models import Sequential
from keras.layers import Conv2D, MaxPooling2D
from keras.layers import Activation, Dropout, Flatten, Dense
from keras import regularizers
from keras import backend as K


img_width = 32
img_height = 30
train_data_dir = 'train'
train_tags_dir = 'train_tags.csv'
test_data_dir = 'test'
test_result_dir = 'result.csv'
save_weights_dir = 'parameters.h5'
nb_train_samples = 378
epochs = 9
batch_size = 9


def create_model():

    if K.image_data_format() == 'channels_first':
        input_shape = (1, img_width, img_height)
    else:
        input_shape = (img_width, img_height, 1)

    my_regularizer = regularizers.l2(0.0001)

    model = Sequential()
    model.add(Conv2D(32, (3, 3), padding='same', input_shape=input_shape, kernel_regularizer = my_regularizer)) #padding same - add 0 at borders
    model.add(Activation('relu'))
    model.add(MaxPooling2D(pool_size=(2, 2)))

    model.add(Conv2D(32, (3, 3), padding='same'))
    model.add(Activation('relu'))
    model.add(MaxPooling2D(pool_size=(2, 2)))

    model.add(Conv2D(64, (3, 3), padding='same'))
    model.add(Activation('relu'))
    model.add(MaxPooling2D(pool_size=(2, 2)))

    model.add(Flatten())
    model.add(Dense(64))
    model.add(Activation('relu'))
    model.add(Dropout(0.5))
    model.add(Dense(1))
    model.add(Activation('sigmoid'))

    return model


def exec_model():
    def append_ext(fn):
        return fn+'.jpg'

    tag_file = pd.read_csv(train_tags_dir, dtype=str, delimiter=',')

    tag_file['id'] = tag_file['id'].apply(append_ext)

    data_gen = ImageDataGenerator(
        rescale=1. / 255.,
        validation_split=0.1,
        horizontal_flip = True)

    train_generator = data_gen.flow_from_dataframe(
            dataframe = tag_file,
            directory = train_data_dir,
            x_col = 'id',
            y_col = 'tag',
            color_mode = 'grayscale',
            subset = 'training',
            batch_size = batch_size,
            shuffle = False,
            class_mode = 'binary',
            target_size = (img_width, img_height))

    valid_generator = data_gen.flow_from_dataframe(
            dataframe = tag_file,
            directory = train_data_dir,
            x_col = 'id',
            y_col = 'tag',
            color_mode = 'grayscale',
            subset = 'validation',
            batch_size = batch_size,
            shuffle = False,
            class_mode = 'binary',
            target_size = (img_width, img_height))

    model.compile(
            loss = 'binary_crossentropy',
            optimizer = 'adam',
            metrics = ['accuracy'])

    model.fit_generator(
        generator = train_generator,
        steps_per_epoch = train_generator.n // train_generator.batch_size,
        validation_data = valid_generator,
        validation_steps = valid_generator.n // valid_generator.batch_size,
        epochs = epochs)

    model.save_weights(save_weights_dir)


def predict_model():
    images = [] # save images for prediction
    image_index = [] # save images names

    image_list = os.listdir(test_data_dir)

    # sort file (image) names in ascending order
    for img in image_list:
        image_index.append(img[:-4])

    image_index = np.asarray(image_index)
    image_index = image_index.astype(int)
    image_index = np.sort(image_index)
    image_index = image_index.astype(str)

    for i in range(0, len(image_index)):
        image_index[i] = image_index[i] + '.jpg'

    image_list = image_index.tolist()

    for img in image_list:
        img = os.path.join(test_data_dir, img) # make path to file img
        img = image.load_img(
            img,
            target_size=(img_width, img_height),
            color_mode = 'grayscale') # get a PIL image
        img = image.img_to_array(img)
        img = img / 255.
        img = np.expand_dims(img, axis=0)
        images.append(img)

    images = np.vstack(images)

    model.load_weights(save_weights_dir)
    predictions = model.predict(images)

    file_res = open(test_result_dir, 'w')

    file_res.write('id,sunglasses_probability\n')
    for img,pred in zip(image_index, predictions.astype(str)):
        file_res.write(img[:-4])
        file_res.write(',')
        file_res.write(pred[0])
        file_res.write('\n')

    file_res.close()

    print('Look for results in %s' % test_result_dir)

model = create_model()
exec_model() # comment this call for pre-trained model
predict_model()
