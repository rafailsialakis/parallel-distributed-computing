/* | Threads | 1           | 2           | 4           | 8
 * | Time    | 2.79495 sec | 1.87883 sec | 1.32614 sec | 1.20855 sec
 * |   
 * | File: bible.txt 16 times. 
 * | CPU Intel(R) Core(TM) i5-1035G1 CPU @ 1.00GHz
 */
#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <pthread.h>

// Structure to represent a word and its count
typedef struct WordNode {
    char* word;
    int count;
    struct WordNode* left;
    struct WordNode* right;
} WordNode;

typedef struct TArgs {
    int from;
    int to;
    char* file_chunk;
    size_t chunk_size;
    WordNode* root;
}TArgs;

struct timespec start, finish;

// Function to create a new word node
WordNode* createNode(const char* word) {
    WordNode* newNode = (WordNode*)malloc(sizeof(WordNode));
    if (newNode == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }
    
    newNode->word = strdup(word);
    if (newNode->word == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        free(newNode);
        exit(1);
    }
    
    newNode->count = 1;
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}

// Function to insert or update a word in the BST
WordNode* insert(WordNode* root, const char* word) {
    if (root == NULL) {
        return createNode(word);
    }
    
    int cmp = strcasecmp(word, root->word);
    
    if (cmp < 0) {
        root->left = insert(root->left, word);
    } else if (cmp > 0) {
        root->right = insert(root->right, word);
    } else {
        // Word already exists, increment count
        root->count++;
    }
    
    return root;
}

// Function to read the entire file into a string
char* readFile(const char* filename, int* filesize) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Error opening file %s\n", filename);
        exit(1);
    }
    
    // Find the file size
    fseek(file, 0, SEEK_END);
    *filesize = ftell(file);
    fseek(file, 0, SEEK_SET);
    
    // Allocate memory for the entire file content plus null terminator
    char* buffer = (char*)malloc((*filesize) + 1);
    if (buffer == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        fclose(file);
        exit(1);
    }
    
    // Read the file into the buffer
    size_t bytesRead = fread(buffer, 1, *(filesize), file);
    buffer[bytesRead] = '\0';  // Null terminate the string
    
    fclose(file);
    return buffer;
}

// Function for in-order traversal and printing the results
void printInOrder(WordNode* root) {
    if (root != NULL) {
        printInOrder(root->left);
        printf("%s\t %d\n", root->word, root->count);
        printInOrder(root->right);
    }
}

// Function to free the BST
void freeTree(WordNode* root) {
    if (root != NULL) {
        freeTree(root->left);
        freeTree(root->right);
        free(root->word);
        free(root);
    }
}

// Function to check if a character is a delimiter
int isDelimiter(char c) {
    return (c == ' ' || c == '\n' || c == '\t' || c == '\r' ||
            c == '.' || c == ',' || c == ';' || c == ':' ||
            c == '!' || c == '?' || c == '(' || c == ')' ||
            c == '{' || c == '}');
}

void* Worker(void* args) {
    TArgs* arguments = (TArgs*)args;
    char *saveptr;
    char* token = strtok_r(arguments->file_chunk, " \n\t\r.,;:!?(){}", &saveptr);
    while (token != NULL) {
        for (char* p = token; *p; p++) {
            *p = tolower((unsigned char)*p);
        }
        if (strlen(token) > 0) {
            arguments->root = insert(arguments->root, token);
        }
        token = strtok_r(NULL, " \n\t\r.,;:!?(){}", &saveptr);
    }
    return NULL;
}

void mergeTrees(WordNode** dest, WordNode* src) {
    if (src == NULL) return;
    *dest = insert(*dest, src->word);
    for (int i = 1; i < src->count; i++)
        *dest = insert(*dest, src->word);  // insert multiple times

    mergeTrees(dest, src->left);
    mergeTrees(dest, src->right);
}


int main(int argc, char* argv[]) {
    if (argc != 3) {
        printf("Usage: %s <file name> <num of threads>\n", argv[0]);
        return 1;
    }
    double elapsed;
    char* endptr;
    int filesize;
    int threads = strtol(argv[2], &endptr, 10);
    if (*endptr != '\0') {
        fprintf(stderr, "Invalid number of threads: %s\n", argv[2]);
        return 1;
    }    
    char* fileparts = (char*) malloc(sizeof(char)*threads);
    char* fileContent = readFile(argv[1], &filesize);
    TArgs* threadArgs = (TArgs*) malloc(sizeof(TArgs) * threads);
    pthread_t* thread_id = (pthread_t*)malloc(sizeof(pthread_t) * threads);
    int previous_end = 0;
    size_t chunk_size = filesize / threads;
    for (int i = 0; i < threads; i++) {
        threadArgs[i].from = previous_end;
        if (i == threads - 1) {
            threadArgs[i].to = filesize;
        } else {
            threadArgs[i].to = threadArgs[i].from + chunk_size;
            if (threadArgs[i].to > filesize) {
                threadArgs[i].to = filesize;
            } else {
                while (threadArgs[i].to < filesize && !isDelimiter(fileContent[threadArgs[i].to])) {
                    threadArgs[i].to++;
                }
                if (threadArgs[i].to < filesize) {
                    threadArgs[i].to++;
                }
            }
            if (threadArgs[i].to > filesize) {
                threadArgs[i].to = filesize;
            }
        }
        threadArgs[i].chunk_size = threadArgs[i].to - threadArgs[i].from;
        threadArgs[i].file_chunk = (char*)malloc(threadArgs[i].chunk_size + 1);
        if (threadArgs[i].file_chunk == NULL) {
            fprintf(stderr, "Memory allocation failed\n");
            for (int j = 0; j < i; j++) {
                free(threadArgs[j].file_chunk);
            }
            free(fileContent);
            free(threadArgs);
            free(thread_id);
            return 1;
        }
        memcpy(threadArgs[i].file_chunk, fileContent + threadArgs[i].from, threadArgs[i].chunk_size);
        threadArgs[i].file_chunk[threadArgs[i].chunk_size] = '\0';
        threadArgs[i].root = NULL;
        previous_end = threadArgs[i].to;
    }
    clock_gettime(CLOCK_MONOTONIC, &start);
    for(int i = 0; i < threads; i++){
        pthread_create(&thread_id[i], NULL, Worker, (void*) &threadArgs[i]);
    }    
    for(int i = 0; i < threads; i++){
        pthread_join(thread_id[i], NULL);
    }

    WordNode* finalRoot = NULL;
    for (int i = 0; i < threads; ++i) {
        mergeTrees(&finalRoot, threadArgs[i].root);
        freeTree(threadArgs[i].root);
    }

    printInOrder(finalRoot);
    clock_gettime(CLOCK_MONOTONIC, &finish);
    elapsed = (finish.tv_sec - start.tv_sec);
    elapsed += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;
    printf("Time taken %.5lf seconds.\n", (float)elapsed);    
    freeTree(finalRoot);
    free(fileContent);
    return 0;
}
