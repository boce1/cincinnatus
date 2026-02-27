/* NNUE wrapping functions */

// include headers
#include "./nnue/nnue.h"
#include "nnue_eval.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
    #include <windows.h>
#else
    #include <unistd.h>
    #include <limits.h>
#endif

#define NNUE_RELATIVE_PATH "nn-04cf2b4ed1da.nnue"

// init NNUE
void init_nnue(char *filename)
{
    // call NNUE probe lib function
    nnue_init(filename);
}

// get NNUE score directly
int evaluate_nnue(int player, int *pieces, int *squares)
{
    // call NNUE probe lib function
    return nnue_evaluate(player, pieces, squares);
}

// det NNUE score from FEN input
int evaluate_fen_nnue(char *fen)
{
    // call NNUE probe lib function
    return nnue_evaluate_fen(fen);
}


char* find_nnue_path() {
    char exe_path[1024];

#ifdef _WIN32
    DWORD len = GetModuleFileNameA(NULL, exe_path, sizeof(exe_path));
    if (len == 0 || len == sizeof(exe_path))
        return NULL;
#else
    ssize_t len = readlink("/proc/self/exe", exe_path, sizeof(exe_path) - 1);
    if (len == -1)
        return NULL;
    exe_path[len] = '\0';
#endif

    /* remove executable name */
    char* last_sep =
#ifdef _WIN32
        strrchr(exe_path, '\\');
#else
        strrchr(exe_path, '/');
#endif
    if (!last_sep)
        return NULL;

    *last_sep = '\0';  // now at /bin

    /* go up one level: project root */
    last_sep =
#ifdef _WIN32
        strrchr(exe_path, '\\');
#else
        strrchr(exe_path, '/');
#endif
    if (!last_sep)
        return NULL;

    *last_sep = '\0';  // now at project root

    /* build full NNUE path */
    char* nnue_path = malloc(1024);
    if (!nnue_path)
        return NULL;

#ifdef _WIN32
    snprintf(nnue_path, 1024, "%s\\%s", exe_path, NNUE_RELATIVE_PATH);
#else
    snprintf(nnue_path, 1024, "%s/%s", exe_path, NNUE_RELATIVE_PATH);
#endif

    return nnue_path;
}