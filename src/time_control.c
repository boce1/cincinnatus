#include "time_control.h"

time_controls* create_time_controls() {
    time_controls* time_info = (time_controls*)malloc(sizeof(time_controls));
    // if(!time_info) {
    //     fprintf(stderr, "Memory allocation failed for time contols.\n");
    //     exit(EXIT_FAILURE);
    // }

    return time_info;
}

void init_time_controls(time_controls* info) {
    info->movestogo = 30;
    info->movetime = -1; 
    info->time = -1;
    info->inc = 0;
    info->starttime = 0; 
    info->stoptime = 0; 
    info->timeset = 0; 
    info->stopped = 0;
}

// https://github.com/bluefeversoft/vice/blob/main/Vice11/src/misc.c
int get_time_ms() {
    #ifdef _WIN32
        return GetTickCount();
    #else
        struct timeval time_value;
        gettimeofday(&time_value, NULL);
        return time_value.tv_sec * 1000 + time_value.tv_usec / 1000;
    #endif
}

int input_waiting() {
    #ifndef _WIN32
        fd_set readfds;
        struct timeval tv;
        FD_ZERO (&readfds);
        FD_SET (fileno(stdin), &readfds);
        tv.tv_sec=0; tv.tv_usec=0;
        select(16, &readfds, 0, 0, &tv);

        return (FD_ISSET(fileno(stdin), &readfds));
    #else
        int init = 0, pipe;
        HANDLE inh;
        DWORD dw;
    
        if (!init) {
            init = 1;
            inh = GetStdHandle(STD_INPUT_HANDLE);
            pipe = !GetConsoleMode(inh, &dw);
            if (!pipe) {
                SetConsoleMode(inh, dw & ~(ENABLE_MOUSE_INPUT|ENABLE_WINDOW_INPUT));
                FlushConsoleInputBuffer(inh);
            }
        }
        if (pipe) {
            if (!PeekNamedPipe(inh, NULL, 0, NULL, &dw, NULL)) return 1;
            return dw;
        } else {
            GetNumberOfConsoleInputEvents(inh, &dw);
            return dw <= 1 ? 0 : dw;
        }
    #endif
}

void read_input(time_controls* info) {
   char input[1024];

    // Only enter if there is actually data to read
    if (input_waiting()) {
        // Use fgets to read the full line
        if (!fgets(input, sizeof(input), stdin)) return;

        // 1. Handle Pings (MUST NOT STOP SEARCH)
        if (strncmp(input, "isready", 7) == 0) {
            printf("readyok\n");
            fflush(stdout); // Tell the GUI we are alive
            return; 
        }

        // 2. Handle Stop/Quit (MUST STOP SEARCH)
        if (strncmp(input, "stop", 4) == 0 || strncmp(input, "quit", 4) == 0) {
            info->stopped = 1;
        }
    }
}

// bridge function to interact between the search and GUI
void communicate(time_controls* info) {
    if(info->timeset && get_time_ms() >= info->stoptime) { // set the stop flag if time is up
        info->stopped = 1;
    }

    read_input(info); // set the stop flag if input is waiting
                        // input in this case is either "stop" or "quit", in any case stop the search

    /*
        the engine communicates with the GUI every 2048 nodes
        if theres input waiting in the stdin buffer it sets the stop flag
        the search function checks this flag and if its set it returns the best move found so far
    */
}