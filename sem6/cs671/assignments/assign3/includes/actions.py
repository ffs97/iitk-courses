import numpy as np

from includes import config


glove_vectors = None


def step(stack, buffer, dt, sentence):

    if len(stack) > 0:
        if stack[-1] == 0:
            stack_top = {
                'id': 0,
                'head': None
            }
        else:
            stack_top = sentence[stack[-1] - 1]

        buffer_top = sentence[buffer[-1] - 1]

        if stack_top['head'] == buffer_top['id']:
            if 'deprel' not in stack_top:
                stack_top['deprel'] = None

            dt[stack_top['id']] = {
                'head': buffer_top['id'],
                'relation': stack_top['deprel'],
                'side': 'left'
            }

            stack.pop()

            relation = stack_top['deprel']
            if relation is not None:
                relation = relation.split(":")[0]

            return stack, buffer, dt, 'la', relation

        elif buffer_top['head'] == stack_top['id']:

            flag = False

            for word in sentence:
                if word['head'] == buffer_top['id']:
                    if word['id'] not in dt:
                        flag = True
                        break

            if not flag:

                if 'deprel' not in stack_top:
                    buffer_top['deprel'] = None

                dt[buffer_top['id']] = {
                    'head': stack_top['id'],
                    'relation': buffer_top['deprel'],
                    'side': 'right'
                }

                buffer.pop()
                buffer.append(
                    stack.pop()
                )

                relation = buffer_top['deprel']
                if relation is not None:
                    relation = relation.split(":")[0]

                return stack, buffer, dt, 'ra', relation

    stack.append(
        buffer.pop()
    )

    return stack, buffer, dt, 'shift', None


def _features(id, dt, sentence):
    global glove_vectors

    if id <= 0 or id >= config.max_token_id:
        return np.concatenate([
            np.zeros(config.tags_len),
            np.zeros(config.relations_len),
            np.zeros(config.glove_size)
        ])

    word_dict = sentence[id - 1]

    tags = np.zeros(config.tags_len)
    relations = np.zeros(config.relations_len)

    tags[config.tags[word_dict['upostag']]] = 1

    if id in dt and dt[id]['relation'] is not None:
        relations[config.relations[dt[id]['relation'].split(":")[0]]] = 1
    else:
        relations[config.relations_len - 1] = 1

    glove = glove_vectors or dict()

    if 'lemma' in word_dict and word_dict['lemma'] in glove:
        vector = glove[word_dict['lemma']]
    else:
        vector = np.zeros(config.glove_size)

    return np.concatenate([tags, relations, vector])


def features(stack, buffer, dt, sentence):
    stack_pos = _features(
        stack[-1] if len(stack) > 0 else 0,
        dt,
        sentence
    )

    buffer_pos = np.concatenate([
        _features(
            buffer[-1] if len(buffer) > 0 else 0,
            dt,
            sentence
        ),
        _features(
            buffer[-2] if len(buffer) > 1 else 0,
            dt,
            sentence
        )
    ])

    stack_occ = [
        nd for nd in dt if len(stack) > 0 and dt[nd]['head'] == stack[-1]
    ]
    min_stack_occ = min(stack_occ + [config.max_token_id])
    max_stack_occ = max(stack_occ + [-1])

    stack_occ_left = _features(
        min_stack_occ, dt, sentence
    )
    stack_occ_right = _features(
        max_stack_occ, dt, sentence
    )

    buffer_occ = [
        nd for nd in dt if len(buffer) > 0 and dt[nd]['head'] == buffer[-1]
    ]
    min_buffer_occ = min(buffer_occ + [config.max_token_id])
    max_buffer_occ = max(buffer_occ + [-1])

    buffer_occ_left = _features(
        min_buffer_occ, dt, sentence
    )
    buffer_occ_right = _features(
        max_buffer_occ, dt, sentence
    )

    return np.concatenate([stack_pos, buffer_pos, stack_occ_left, stack_occ_right, buffer_occ_left, buffer_occ_right])


def output(step, relation):
    action = np.zeros(2 * config.num_relations + 1)

    if config.steps[step] == 0:
        if relation:
            action[config.relations[relation]] = 1
    elif config.steps[step] == 1:
        if relation:
            action[config.relations[relation] + config.num_relations] = 1
    elif config.steps[step] == 2:
        action[-1] = 1

    return action
