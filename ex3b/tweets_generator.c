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
#define POINT '.'

/* check if the given word is end of sentence, last char is "." */
bool is_last_word(const void *word) {
    const char *word_c = word;
    return word_c[strlen(word_c) - 1] == POINT;
}

int compare_word(const void *word1, const void *word2){
    return strcmp((char*)word1, (char*)word2);
}

void free_word(void* ptr){
    free((char*)ptr);
}

void * copy_word(const void * src){
    char *cpy = malloc(strlen(src) + 1);
    if(!cpy){
        return NULL;
    }
    strcpy(cpy, src);
    return cpy;
}

void my_print(const void * ptr){
    const char *word_c = ptr;
    fprintf(stdout, is_last_word(ptr) ? "%s" : "%s ", word_c);
}

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
                free_markov_chain(&markov_chain);
                return EXIT_FAILURE;
            }
            if (last) {
                /* not start of sentence, add to frequency of previous word */
                if (add_node_to_frequency_list(
                        last->data, new_word->data, markov_chain)) {
                    /* FREE */
                    free_markov_chain(&markov_chain);
                    return EXIT_FAILURE;
                }
            }
            last = is_last_word(pch) ? NULL : new_word;
            pch = strtok(NULL, DELIMITERS);
        }
    }
    return EXIT_SUCCESS;
}

/* this function creates new random tweets from given file */
int create_tweets(int seed, int tweets, FILE *file, int word_to_read) {
    /* update seed */
    srand(seed);
    MarkovChain *mrk = create_chain(is_last_word, compare_word,
                                    free_word, copy_word, my_print);
    if (mrk) {
        if (fill_database(file, word_to_read, mrk)) {
            fprintf(stdout, ALLOCATION_ERROR_MASSAGE);
            free_markov_chain(&mrk);
            return EXIT_FAILURE;
        }

        /* successful insert data to markov-chain, creates the tweets */
        for (int k = 1; k <= tweets; k++) {
            fprintf(stdout, TWEET, k);
            generate_random_sequence(mrk,get_first_random_node(mrk),
                                     MAX_LENGTH);
        }
    } else {
        fprintf(stdout, ALLOCATION_ERROR_MASSAGE);
        return EXIT_FAILURE;
    }
    free_markov_chain(&mrk);
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