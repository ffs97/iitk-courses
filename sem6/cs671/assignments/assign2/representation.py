import numpy as np
from sklearn.feature_extraction.text import CountVectorizer, TfidfVectorizer


vocab = None
glove_vectors = None
word2vec_vectors = None

vocab_size = 0


def bow(data, binary=True):
    bow_vectorizer = CountVectorizer(vocabulary=vocab, binary=binary)
    return bow_vectorizer.fit_transform(data).toarray()


def normalizedtf(data):
    vectorizer = TfidfVectorizer(vocabulary=vocab, norm='l1', use_idf=False)
    return vectorizer.fit_transform(data).toarray()


def tfidf(data):
    vectorizer = TfidfVectorizer(vocabulary=vocab, norm='l1')
    return vectorizer.fit_transform(data).toarray()


def glove(data):
    glove_data = []
    for doc in data:
        doc = doc.split()
        doc_vector = np.zeros(300)

        word_count = 1  # To avoid zero word-counts

        for word in doc:
            if word in glove_vectors:
                doc_vector += glove_vectors[word]
                word_count += 1

        glove_data.append(doc_vector / word_count)

    return np.array(glove_data)


def word2vec(data):
    word2vec_data = []
    for doc in data:
        doc = doc.split()
        doc_vector = np.zeros(300)

        word_count = 1  # To avoid zero word-counts

        for word in doc:
            if word in word2vec_vectors:
                doc_vector += word2vec_vectors[word]
                word_count += 1

        word2vec_data.append(doc_vector / word_count)

    return np.array(word2vec_data)


def data_vectors(data, format, max_len=50):
    data_format_vectors = []

    vectors = None
    if format == "glove":
        vectors = glove_vectors
    elif format == "word2vec":
        vectors = word2vec_vectors

    for doc in data:
        doc = doc.split()
        doc_vectors = []

        for word in doc:
            if word in vectors:
                doc_vectors.append(vectors[word])

                if len(doc_vectors) == max_len:
                    break

        while len(doc_vectors) < max_len:
            doc_vectors.append(np.zeros(300))

        data_format_vectors.append(doc_vectors)

    return np.array(data_format_vectors)


def data_index(data, max_len=50):
    indices = []
    for doc in data:
        doc = doc.split()
        doc_indices = []

        for word in doc:
            if word in vocab:
                doc_indices.append(vocab[word])

        indices.append(doc_indices)

    return indices


def get_embedding_matrix(format):
    embedding_matrix = np.zeros([vocab_size, 300])

    vectors = None
    if format == "glove":
        vectors = glove_vectors
    elif format == "word2vec":
        vectors = word2vec_vectors

    for word in vocab:
        if word in vectors:
            embedding_matrix[vocab[word]] = vectors[word]

    return [embedding_matrix]
