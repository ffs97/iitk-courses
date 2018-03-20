import os
import nltk
import gensim


def extract_glove_vectors(vocab):
    print
    print "Extracting GloVe Vectors ..."

    glove_file = open("data/glove.840B.300d.txt", "r")
    glove_filtered_file = open("data/glove.txt", "w")

    for line in glove_file:
        line = line.strip()
        if line.split(' ')[0] in vocab:
            glove_filtered_file.write(line + "\n")

    glove_file.close()
    glove_filtered_file.close()

    print "Done"


def extract_word2vec_vectors(vocab):
    print
    print "Extracting Word2Vec Vectors ..."

    wv_model = gensim.models.KeyedVectors.load_word2vec_format(
        "data/GoogleNews-vectors-negative300.bin", binary=True
    )
    word2vec_filtered_file = open("data/word2vec.txt", "w")

    for word in wv_model.wv.vocab:
        if word in vocab:
            word2vec_filtered_file.write(
                word + " " + " ".join(
                    [str(field) for field in wv_model.vw[word]]
                ) + "\n"
            )

    wv_model.close()
    word2vec_filtered_file.close()

    print "Done"


def download_stopwords():
    nltk.download("stopwords")


def collect_data(dir):
    data = list()
    labels = list()

    for label in ["neg", "pos"]:
        files_list = os.listdir(dir + label + "/")

        with open(dir + label + ".txt", "w") as out:
            for file_name in files_list:
                with open(dir + label + "/" + file_name, "r") as f:
                    out.write(" ".join(f.read().lower().split()) + "\n")

        print "Written data to file: " + dir + label + ".txt"


def main():
    download_stopwords()

    extract_glove_vectors()
    extract_word2vec_vectors()

    collect_data("data/train/")
    collect_data("data/test/")


if __name__ == "__main__":
    main()
