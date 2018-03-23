import os
import nltk
import gensim

from collections import namedtuple


def load_vocab(vocab_file="data/imdb.vocab"):
    vocab = dict()

    with open(vocab_file, "r") as f:
        words = [line.strip() for line in f.read().split("\n")]

    id = 0
    stop_words = set(nltk.corpus.stopwords.words("english"))
    for word in words:
        word = word.lower()
        if word not in stop_words:
            vocab[word] = id
            id += 1

    return vocab


def collect_data(dir):
    for label in ["neg", "pos"]:
        files_list = os.listdir(dir + label + "/")

        with open(dir + label + ".txt", "w") as out:
            for file_name in files_list:
                with open(dir + label + "/" + file_name, "r") as f:
                    out.write(" ".join(f.read().lower().split()) + "\n")

        print "Written data to file: " + dir + label + ".txt"


def download_stopwords():
    nltk.download("stopwords")


def extract_glove_vectors(vocab, data_file_name="data/glove.840B.300d.txt", glove_file_name="data/glove.txt"):
    print
    print "Extracting GloVe Vectors ..."

    glove_file = open(data_file_name, "r")
    glove_filtered_file = open(glove_file_name, "w")

    for line in glove_file:
        line = line.strip()
        if line.split(' ')[0] in vocab:
            glove_filtered_file.write(line + "\n")

    glove_file.close()
    glove_filtered_file.close()

    print "Done"


def extract_word2vec_vectors(vocab, data_file_name="data/GoogleNews-vectors-negative300.bin", word2vec_file_name="data/word2vec.txt"):
    print
    print "Extracting Word2Vec Vectors ..."

    wv_model = gensim.models.KeyedVectors.load_word2vec_format(
        data_file_name, binary=True
    )
    word2vec_filtered_file = open(word2vec_file_name, "w")

    for word in wv_model.wv.vocab:
        if word in vocab:
            word2vec_filtered_file.write(
                word + " " + " ".join(
                    [str(field) for field in wv_model.wv[word]]
                ) + "\n"
            )

    word2vec_filtered_file.close()

    print "Done"


def extract_doc2vec_vectors(vocab, dir="data/train", doc2vec_file_name="data/doc2vec.txt"):
    print
    print "Extracting Doc2Vec Vectors ..."

    docs = []

    analyzedDocument = namedtuple('AnalyzedDocument', 'words tags')

    with open(dir + "/pos.txt", "r") as f:
        for i, line in enumerate(f):
            docs.append(
                analyzedDocument(line.strip().split(), [i])
            )

    with open(dir + "/neg.txt", "r") as f:
        for i, line in enumerate(f):
            docs.append(
                analyzedDocument(line.strip().split(), [i])
            )

    doc2vec_model = gensim.models.doc2vec.Doc2Vec(
        docs, vector_size=300, window=300, min_count=1, workers=4
    )

    with open(doc2vec_file_name, "w") as f:
        for word in doc2vec_model.wv.vocab:
            if word in vocab:
                vec = [str(field) for field in doc2vec_model[word]]
                f.write(word + " " + " ".join(vec) + "\n")


def main():
    download_stopwords()

    vocab = load_vocab()

    collect_data("data/train/")
    collect_data("data/test/")

    extract_glove_vectors(vocab)
    extract_word2vec_vectors(vocab)

    extract_doc2vec_vectors(vocab, "data/train")


if __name__ == "__main__":
    main()
