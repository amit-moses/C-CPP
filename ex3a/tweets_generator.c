#define FILE_PATH_ERROR "Error: incorrect file path"
#define NUM_ARGS_ERROR "Usage: invalid number of arguments"

#include <stdio.h>
#include <string.h>
#include "markov_chain.h"

#define MAX_LINE 1000
#define DELIMITERS " \n\t\r"
#define TWEET "Tweet %d: "
#define MAX_LENGTH 20
#define BASE 10
#define READ_ALL_FILE 4
#define READ_NOT_ALL 5

/* this function insert data to markov-chain from the given file*/
int fill_database(FILE *fp, int words_to_read, MarkovChain *markov_chain) {
    char line[MAX_LINE] = {0};
    int word_sum = 0;
    /* read one line in the file*/
    while (fgets(line, MAX_LINE, fp)) {
        char *pch;
        Node *last = NULL;
        pch = strtok(line, DELIMITERS);

        /* read all words in the line */
        while (pch != NULL) {
            if (words_to_read <= word_sum && words_to_read != -1) {
                return EXIT_SUCCESS;
            }

            Node *new_word = add_to_database(markov_chain, pch);
            word_sum++;

            if (!new_word) {
                /* FREE */
                free_database(&markov_chain);
                return EXIT_FAILURE;
            }
            if (last) {
                /* not start of sentence, add to frequency of previous word */
                if (add_node_to_frequency_list(last->data, new_word->data)) {
                    /* FREE */
                    free_database(&markov_chain);
                    return EXIT_FAILURE;
                }
            }
            last = end_sentence(pch) ? NULL : new_word;
            pch = strtok(NULL, DELIMITERS);
        }
    }
    return EXIT_SUCCESS;
}

/* this function creates new random tweets from given file */
int create_tweets(int seed, int tweets, FILE *file, int word_to_read) {
    /* update seed */
    srand(seed);

    /* allocate new space for the markov chain */
    MarkovChain *mrk = malloc(sizeof(*mrk));
    LinkedList *lkl = malloc(sizeof(*lkl));
    if (mrk && lkl) {
        lkl->first = NULL;
        lkl->last = NULL;
        lkl->size = 0;
        mrk->database = lkl;
        if (fill_database(file, word_to_read, mrk)) {
            fprintf(stdout, ALLOCATION_ERROR_MASSAGE);
            free_database(&mrk);
            return EXIT_FAILURE;
        }

        /* successful insert data to markov-chain, creates the tweets */
        for (int k = 1; k <= tweets; k++) {
            fprintf(stdout, TWEET, k);
            generate_tweet(get_first_random_node(mrk), MAX_LENGTH);
        }
    } else {
        free(mrk);
        free(lkl);
        fprintf(stdout, ALLOCATION_ERROR_MASSAGE);
        return EXIT_FAILURE;
    }
    free_database(&mrk);
    return EXIT_SUCCESS;
}

int main(int argc, char *argv[]) {
    /* checks validation of args */
    if (argc == READ_ALL_FILE || argc == READ_NOT_ALL) {
        int seed = strtol(argv[1], NULL, BASE);
        int tweets = strtol(argv[2], NULL, BASE);
        int words = argc == READ_ALL_FILE ? -1 : strtol(argv[4], NULL, BASE);
        FILE *file = fopen(argv[3], "r");
        if (file) {
            int ret = create_tweets(seed, tweets, file, words);
            fclose(file);
            return ret;
        }
        fprintf(stdout, FILE_PATH_ERROR);
        return EXIT_FAILURE;
    }
    fprintf(stdout, NUM_ARGS_ERROR);
    return EXIT_FAILURE;
}