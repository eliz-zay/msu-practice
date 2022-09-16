import pickle5 as pickle
from os import listdir
from os.path import isfile, join


def to_file(file, what):
    fd = open(file, 'wb+')
    pickle.dump(what, fd)
    fd.close()


def from_file(file):
    fd = open(file, 'rb')
    val = pickle.load(fd)
    fd.close()

    return val


def get_file_names(dir):
    return [f.split('.npz')[0] for f in listdir(dir) if isfile(join(dir, f)) and f[0] != '.']
