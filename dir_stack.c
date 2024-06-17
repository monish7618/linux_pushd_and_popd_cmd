#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

typedef struct DirNode {
    char *dir;
    struct DirNode *next;
} DirNode;

DirNode *push(DirNode *stack, const char *dir) {
    DirNode *node = (DirNode *)malloc(sizeof(DirNode));
    if (!node) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    node->dir = strdup(dir);
    if (!node->dir) {
        perror("strdup");
        exit(EXIT_FAILURE);
    }
    node->next = stack;
    return node;
}


DirNode *pop(DirNode *stack, char **dir) {
    if (!stack) return NULL;
    DirNode *next = stack->next;
    *dir = stack->dir;
    free(stack);
    return next;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <command> [dir]\n", argv[0]);
        return EXIT_FAILURE;
    }

    static DirNode *stack = NULL;
    char cwd[1024];

    if (strcmp(argv[1], "pushd") == 0) {
        if (argc < 3) {
            fprintf(stderr, "Usage: %s pushd <dir>\n", argv[0]);
            return EXIT_FAILURE;
        }

        if (!getcwd(cwd, sizeof(cwd))) {
            perror("getcwd");
            return EXIT_FAILURE;
        }

        if (chdir(argv[2]) != 0) {
            perror("chdir");
            return EXIT_FAILURE;
        }

        stack = push(stack, cwd);
        printf("Directory pushed: %s\n", cwd);

    } else if (strcmp(argv[1], "popd") == 0) {
        if (!stack) {
            fprintf(stderr, "Directory stack is empty\n");
            return EXIT_FAILURE;
        }

        char *dir;
        stack = pop(stack, &dir);

        if (chdir(dir) != 0) {
            perror("chdir");
            return EXIT_FAILURE;
        }

        printf("Directory popped and changed to: %s\n", dir);
        free(dir);

    } else {
        fprintf(stderr, "Unknown command: %s\n", argv[1]);
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

