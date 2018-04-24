glove_size = 100

num_relations = 37

max_token_id = 1000

model_size = (150, 100)

steps = {
    'la': 0,
    'ra': 1,
    'shift': 2,
}

tags = dict([(el, i) for i, el in enumerate(
    ['ADJ', 'ADV', 'INTJ', 'NOUN', 'PROPN', 'VERB', 'ADP', 'AUX', 'CCONJ',
        'DET', 'NUM', 'PART', 'PRON', 'SCONJ', 'PUNCT', 'SYM', 'X']
)])

tags_len = len(tags.keys())

relations = dict([(el, i) for i, el in enumerate(
    ['nsubj', 'obj', 'iobj', 'obl', 'vocative', 'expl', 'dislocated', 'nmod', 'appos', 'nummod', 'conj', 'cc', 'fixed', 'flat', 'compound', 'csubj', 'ccomp', 'xcomp', 'advcl',
        'acl', 'root', 'advmod', 'discourse', 'amod', 'aux', 'cop', 'mark', 'det', 'clf', 'case', 'dep', 'list', 'parataxis', 'orphan', 'goeswith', 'reparandum', 'punct']
)])

relations_len = len(relations.keys())
