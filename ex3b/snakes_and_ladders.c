#include <string.h> // For strlen(), strcmp(), strcpy()
#include "markov_chain.h"

#define MAX(X, Y) (((X) < (Y)) ? (Y) : (X))

#define EMPTY -1
#define BOARD_SIZE 100
#define MAX_GENERATION_LENGTH 60
#define BASE 10
#define DICE_MAX 6
#define NUM_OF_TRANSITIONS 20
#define VALID_ARGS 3
#define NUM_ARGS_ERROR "Usage: invalid number of arguments"
#define WALK "Random Walk %d: "

#define SNAKE_TO "-snake to-> "
#define LADDER_TO "-ladder to-> "
#define ARROW "-> "
#define WITHOUT ""
#define PRINT_FORMAT "[%d] %s"
/**
 * represents the transitions by ladders and snakes in the game
 * each tuple (x,y) represents a ladder from x to if x<y or a snake otherwise
 */
const int transitions[][2] = {{13, 4},
                              {85, 17},
                              {95, 67},
                              {97, 58},
                              {66, 89},
                              {87, 31},
                              {57, 83},
                              {91, 25},
                              {28, 50},
                              {35, 11},
                              {8,  30},
                              {41, 62},
                              {81, 43},
                              {69, 32},
                              {20, 39},
                              {33, 70},
                              {79, 99},
                              {23, 76},
                              {15, 47},
                              {61, 14}};

/**
 * struct represents a Cell in the game board
 */
typedef struct Cell {
    int number;
    int ladder_to;
    int snake_to;
} Cell;

/** Error handler **/
int handle_error_snakes(char *error_msg, MarkovChain **database)
{
    printf("%s", error_msg);
    if (database != NULL)
    {
        free_markov_chain(database);
    }
    return EXIT_FAILURE;
}


int create_board(Cell *cells[BOARD_SIZE])
{
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        cells[i] = malloc(sizeof(Cell));
        if (cells[i] == NULL)
        {
            for (int j = 0; j < i; j++) {
                free(cells[j]);
            }
            handle_error_snakes(ALLOCATION_ERROR_MASSAGE,NULL);
            return EXIT_FAILURE;
        }
        *(cells[i]) = (Cell) {i + 1, EMPTY, EMPTY};
    }

    for (int i = 0; i < NUM_OF_TRANSITIONS; i++)
    {
        int from = transitions[i][0];
        int to = transitions[i][1];
        if (from < to)
        {
            cells[from - 1]->ladder_to = to;
        }
        else
        {
            cells[from - 1]->snake_to = to;
        }
    }
    return EXIT_SUCCESS;
}

/**
 * fills database
 * @param markov_chain
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
int fill_database_snakes(MarkovChain *markov_chain){
    Cell* cells[BOARD_SIZE];
    if(create_board(cells) == EXIT_FAILURE){
        return EXIT_FAILURE;
    }
    MarkovNode *from_node = NULL, *to_node = NULL;
    size_t index_to;
    for (size_t i = 0; i < BOARD_SIZE; i++){
        add_to_database(markov_chain, cells[i]);
    }
    for (size_t i = 0; i < BOARD_SIZE; i++){
        from_node = get_node_from_database(markov_chain,cells[i])->data;
        if (cells[i]->snake_to != EMPTY || cells[i]->ladder_to != EMPTY)
        {
            index_to = MAX(cells[i]->snake_to,cells[i]->ladder_to) - 1;
            to_node = get_node_from_database(markov_chain,
                                             cells[index_to])->data;
            add_node_to_frequency_list(from_node,
                                       to_node, markov_chain);
        }
        else{
            for (int j = 1; j <= DICE_MAX; j++){
                index_to = ((Cell*) (from_node->data))->number + j - 1;
                if (index_to >= BOARD_SIZE){
                    break;
                }
                to_node = get_node_from_database(markov_chain,
                                                 cells[index_to])->data;
                int  res = add_node_to_frequency_list(from_node,
                                                      to_node, markov_chain);
                if(res==EXIT_FAILURE){
                    return EXIT_FAILURE;
                }
            }
        }
    }
    // free temp arr
    for (size_t i = 0; i < BOARD_SIZE; i++)
    {
        free(cells[i]);
    }
    return EXIT_SUCCESS;
}

bool is_last_cell(const void *cell) {
    const Cell *cll = cell;
    return cll->number == BOARD_SIZE;
}

int compare_cell(const void *cell1, const void *cell2){
    Cell *c1 = (Cell*)cell1, *c2 = (Cell*)cell2;
    return c1->number - c2->number;
}

void free_cell(void* ptr){
    free((char*)ptr);
}

void * copy_cell(const void * src){
    Cell *cpy = malloc(sizeof(*cpy));
    if(!cpy){
        return NULL;
    }
    Cell *my_src = (Cell*)src;
    cpy->number = my_src->number;
    cpy->ladder_to = my_src->ladder_to;
    cpy->snake_to = my_src->snake_to;
    return cpy;
}

void print_cell(const void * ptr){
    const Cell *cll = ptr;

    fprintf(stdout, PRINT_FORMAT, cll->number, cll->snake_to != EMPTY ?
    SNAKE_TO: cll->ladder_to != EMPTY ?
    LADDER_TO: cll->number == BOARD_SIZE?
    WITHOUT: ARROW);
}


/**
 * @param argc num of arguments
 * @param argv 1) Seed
 *             2) Number of sentences to generate
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */

/* this function creates new random walks from given file */
int create_walks(int seed, int walks) {
    /* update seed */
    srand(seed);

    MarkovChain *mrk = create_chain(is_last_cell,
                                    compare_cell, free_cell,
                                    copy_cell, print_cell);
    if (mrk) {
        if (fill_database_snakes(mrk)) {
            fprintf(stdout, ALLOCATION_ERROR_MASSAGE);
            free_markov_chain(&mrk);
            return EXIT_FAILURE;
        }

        /* successful insert data to markov-chain, creates the walks */
        for (int k = 1; k <= walks; k++) {
            fprintf(stdout, WALK, k);
            generate_random_sequence(mrk, mrk->database->first->data,
                                     MAX_GENERATION_LENGTH);
        }
    } else {
        fprintf(stdout, ALLOCATION_ERROR_MASSAGE);
        return EXIT_FAILURE;
    }
    free_markov_chain(&mrk);
    return EXIT_SUCCESS;
}

int main(int argc, char *argv[]){
/* checks validation of args */
    if (argc == VALID_ARGS) {
        int seed = strtol(argv[1], NULL, BASE);
        int walks = strtol(argv[2], NULL, BASE);
        return create_walks(seed, walks);
    }
    fprintf(stdout, NUM_ARGS_ERROR);
    return EXIT_FAILURE;
}

