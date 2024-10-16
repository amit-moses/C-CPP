#include "markov_chain.h"
#include <string.h>

/**
 * Get random number between 0 and max_number [0, max_number).
 * @param max_number
 * @return Random number
 */
int get_random_number(int max_number) {
    return rand() % max_number;
}

/* check if the given word is end of sentence, last char is "." */
int end_sentence(const char *word) {
    return word[strlen(word) - 1] == '.';
}

/* get node from the chain, return NULL if not exist*/
Node *get_node_from_database(MarkovChain *markov_chain, char *data_ptr) {
    Node *pos = markov_chain->database->first;
    /* loop will be stopped if is end of list or found */
    while (pos && strcmp(data_ptr, pos->data->data) != 0) {
        pos = pos->next;
    }
    return pos;
}

/* this function free ONE markov-node and all its fields */
void free_markov_node(MarkovNode **mrc) {
    MarkovNode *mrk = *mrc;
    if (mrk) {
        free(mrk->data);
        free(mrk->frequency_list);
        free(mrk);
    }
}

/* this function free all markov chain include all its node and fields */
void free_database(MarkovChain **ptr_chain) {
    MarkovChain *edit = *ptr_chain;
    if (edit) {
        Node *pos = edit->database->first;
        while (pos) {
            Node *temp = pos->next;
            free_markov_node(&pos->data);
            free(pos);
            pos = temp;
        }
        free(edit->database);
        free(edit);
        *ptr_chain = NULL;
    }

}

/* this function creates node with the given word */
MarkovNode *create_markov_node(char *word) {
    /* allocate new markov-node and str for its name */
    char *copy = malloc(sizeof(char) * (strlen(word) + 1));
    MarkovNode *mrc = malloc(sizeof(MarkovNode));
    if (copy && mrc) {
        mrc->data = copy;
        strcpy(mrc->data, word);
        mrc->load = 0;
        mrc->frequency_list = NULL;
        return mrc;
    }
    free(mrc);
    free(copy);
    return NULL;
}

/* this function get pointer to word and push to markov-chain */
Node *add_to_database(MarkovChain *markov_chain, char *data_ptr) {
    if(!markov_chain || !markov_chain->database){
        return NULL;
    }
    Node *cur = get_node_from_database(markov_chain, data_ptr);
    if (cur) {
        /* the word already in database - return it */
        return cur;
    }

    /* create new markov-node and add to linked-list */
    MarkovNode *new_markov_node = create_markov_node(data_ptr);
    if (new_markov_node) {
        if (!add(markov_chain->database, new_markov_node)) {
            return markov_chain->database->last;
        }
    }
    free_markov_node(&new_markov_node);
    return NULL;
}

/* this function updates the frequency for word if exist */
int add_one_to_frequency_list(MarkovNode *first_node,
                              MarkovNode *second_node) {
    /* search the word in frequency list*/
    for (int k = 0; k < first_node->load; k++) {
        MarkovNodeFrequency *mrk = first_node->frequency_list + k;
        if (!strcmp(mrk->markov_node->data, second_node->data)) {
            /* add one if exist */
            mrk->frequency++;
            return 1;
        }
    }
    /* word not exist in frequency list */
    return 0;
}

/* this function allocates new space for new markov-node-frequency */
int add_space(MarkovNode *mrc_node) {
    /* add 1 to space of frequency list */
    size_t new_size = sizeof(MarkovNodeFrequency) * (mrc_node->load + 1);
    MarkovNodeFrequency *temp = realloc(mrc_node->frequency_list, new_size);
    if (temp) {
        mrc_node->frequency_list = temp;
        return EXIT_SUCCESS;
    }
    return EXIT_FAILURE;
}

/* this function add node to frequency list if not exist, else update it */
int add_node_to_frequency_list(MarkovNode *first_node,
                               MarkovNode *second_node) {
    if(!first_node || !second_node){
        return EXIT_FAILURE;
    }
    /* add 1 to frequency if word already exist in the list */
    if (add_one_to_frequency_list(first_node, second_node)) {
        return EXIT_SUCCESS;
    }

    /* add space for new one word to list */
    if (add_space(first_node)) {
        return EXIT_FAILURE;
    }

    /* add the word */
    first_node->frequency_list[first_node->load].markov_node = second_node;
    first_node->frequency_list[first_node->load].frequency = 1;
    first_node->load++;
    return EXIT_SUCCESS;
}

/* this function returns random words from the chain */
MarkovNode *get_random_node_from_chain(MarkovChain *markov_chain) {
    Node *pos = markov_chain->database->first;
    /* get random number represents index of node in the chain */
    int rnd_num = get_random_number(markov_chain->database->size);

    /* search the random word */
    for (int k = 0; k < rnd_num && pos; k++) {
        pos = pos->next;
    }
    return pos ? pos->data : NULL;
}

/* this function returns random node from chain - not end of sentence */
MarkovNode *get_first_random_node(MarkovChain *markov_chain) {
    MarkovNode *rnd_node = get_random_node_from_chain(markov_chain);
    /* try again until the node will not be end of sentence or NULL*/
    while (!rnd_node || end_sentence(rnd_node->data)) {
        rnd_node = get_random_node_from_chain(markov_chain);
    }
    return rnd_node;
}

/* this function return the sum of repeats frequency in frequency list */
int get_sum_of_frequency(MarkovNode *mrn) {
    int sum = 0;
    for (int k = 0; k < mrn->load; k++) {
        sum += mrn->frequency_list[k].frequency;
    }
    return sum;
}

/* this function returns next word in the new sentence */
MarkovNode *get_next_random_node(MarkovNode *cur_markov_node) {
    /* get random number low than the number of repeats words */
    int rnd_num = get_random_number(get_sum_of_frequency(cur_markov_node));
    int pos = 0;
    while(0 <= rnd_num){
        /* reduce number frequency of specific node until it hit the random */
        rnd_num -= cur_markov_node->frequency_list[pos].frequency;
        pos++;
    }
    return cur_markov_node->frequency_list[pos-1].markov_node;
}

/* this function prints new random tweets */
void generate_tweet(MarkovNode *first_node, int max_length) {
    int is_last = 0;
    for (int k = 0; k < max_length && !is_last && first_node; k++) {
        is_last = end_sentence(first_node->data) || k == max_length - 1;
        if (is_last) {
            fprintf(stdout, "%s\n", first_node->data);
        } else {
            fprintf(stdout, "%s ", first_node->data);
            first_node = get_next_random_node(first_node);
        }
    }
}