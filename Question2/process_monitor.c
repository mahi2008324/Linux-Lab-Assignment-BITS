/*
 * ============================================================================
 * File        : process_monitor.c
 * Description : Web server child process monitor — demonstrates fork(),
 *               wait(), waitpid(), kill(), SIGTERM, SIGKILL, SIGCHLD,
 *               alarm(), and sigaction() to prevent zombie processes and
 *               terminate unresponsive child processes.
 *
 * Author      : Linux Lab Assignment — Question 2
 * Date        : 2026-07-20
 * Standard    : C11 (POSIX.1-2008)
 * Compile     : gcc -Wall -Wextra -std=c11 process_monitor.c -o process_monitor
 * Usage       : ./process_monitor
 * ============================================================================
 */

/* ---- Feature-test macros (must come before any #include) ---- */
#define _POSIX_C_SOURCE 200809L   /* Enables POSIX.1-2008 API */

/* ---- Standard Headers ---- */
#include <stdio.h>      /* printf, fprintf, perror */
#include <stdlib.h>     /* exit, rand, srand, EXIT_SUCCESS, EXIT_FAILURE */
#include <unistd.h>     /* fork, sleep, alarm, getpid, getppid */
#include <sys/types.h>  /* pid_t */
#include <sys/wait.h>   /* wait, waitpid, WIFEXITED, WEXITSTATUS, WIFSIGNALED */
#include <signal.h>     /* kill, sigaction, SIGTERM, SIGKILL, SIGCHLD, SIGALRM */
#include <time.h>       /* time (for srand seeding) */
#include <string.h>     /* strerror */
#include <errno.h>      /* errno */

/* ============================================================================
 * CONSTANTS
 * ============================================================================ */
#define NUM_CHILDREN        5       /* Total child processes to create       */
#define CHILD_TIMEOUT_SEC   8       /* Max seconds a child may run           */
#define UNRESPONSIVE_INDEX  2       /* Index of the "hung" child (0-based)   */
#define UNRESPONSIVE_SLEEP  30      /* How long the hung child sleeps        */
#define SIGTERM_WAIT_SEC    2       /* Wait after SIGTERM before SIGKILL     */

/* ============================================================================
 * GLOBAL VARIABLES
 * Note: volatile sig_atomic_t is required for variables shared between
 * signal handlers and main program (signal-safe types).
 * ============================================================================ */
static volatile sig_atomic_t children_reaped = 0;  /* Count of reaped children */

/* Array to store child PIDs — populated in parent after each fork() */
static pid_t child_pids[NUM_CHILDREN];

/* ============================================================================
 * FUNCTION PROTOTYPES
 * ============================================================================ */
static void sigchld_handler(int signo);
static void sigalrm_handler(int signo);
static void setup_signal_handlers(void);
static void child_worker(int child_index);
static void monitor_children(void);
static void terminate_child(pid_t pid, int child_index);
static void print_separator(void);
static void print_exit_status(pid_t pid, int status, int child_index);

/* ============================================================================
 * FUNCTION: sigchld_handler
 * Purpose : Handles SIGCHLD signal — sent to parent whenever a child changes
 *           state (exits, stops, or resumes). We use waitpid() with WNOHANG
 *           to reap all available zombie children without blocking.
 *
 * Why WNOHANG?
 *   Without WNOHANG, waitpid() would block if no child has exited yet.
 *   With WNOHANG, it returns immediately — suitable inside a signal handler.
 *
 * Why a loop?
 *   Multiple children may exit between two SIGCHLD deliveries. A single
 *   waitpid() call might miss some — a loop ensures all are reaped.
 * ============================================================================ */
static void sigchld_handler(int signo) {
    (void)signo;    /* Suppress unused-parameter warning */
    int saved_errno = errno;   /* Save errno — signal handlers must restore it */

    pid_t pid;
    int status;

    /*
     * Loop: reap ALL available zombie children.
     * waitpid(-1, ...) waits for ANY child.
     * WNOHANG returns 0 immediately if no child has exited yet.
     */
    while ((pid = waitpid(-1, &status, WNOHANG)) > 0) {
        children_reaped++;
        /* Signal-safe output: write() is async-signal-safe; printf is NOT */
        /* We use write() to avoid undefined behaviour inside signal handlers */
        const char *msg = "[SIGCHLD] Child process reaped.\n";
        write(STDOUT_FILENO, msg, 33);
    }

    errno = saved_errno;   /* Restore errno */
}

/* ============================================================================
 * FUNCTION: sigalrm_handler
 * Purpose : Handles SIGALRM — triggered when alarm() fires in child processes.
 *           Causes the child to exit with a specific status code when its
 *           allotted time expires.
 * ============================================================================ */
static void sigalrm_handler(int signo) {
    (void)signo;
    /* POSIX: _exit() is async-signal-safe; exit() is NOT (it flushes buffers) */
    _exit(42);   /* Exit code 42 = timeout (child-side self-termination) */
}

/* ============================================================================
 * FUNCTION: setup_signal_handlers
 * Purpose : Installs all required signal handlers using sigaction().
 *
 * Why sigaction() instead of signal()?
 *   signal() has undefined behaviour on some systems (e.g., whether it
 *   auto-resets the handler after one delivery). sigaction() is:
 *   - More portable (POSIX standard)
 *   - More controllable (flags like SA_RESTART, SA_NOCLDSTOP)
 *   - Safer for production code
 * ============================================================================ */
static void setup_signal_handlers(void) {
    struct sigaction sa_chld, sa_alrm;

    /* ------ SIGCHLD Handler ------ */
    sa_chld.sa_handler = sigchld_handler;
    sigemptyset(&sa_chld.sa_mask);   /* No additional signals blocked during handler */
    /*
     * SA_RESTART: Automatically restart interrupted system calls (e.g., sleep).
     * SA_NOCLDSTOP: Only deliver SIGCHLD when a child EXITS, not when it stops.
     */
    sa_chld.sa_flags = SA_RESTART | SA_NOCLDSTOP;

    if (sigaction(SIGCHLD, &sa_chld, NULL) == -1) {
        perror("sigaction(SIGCHLD) failed");
        exit(EXIT_FAILURE);
    }

    /* ------ SIGALRM Handler ------ */
    sa_alrm.sa_handler = sigalrm_handler;
    sigemptyset(&sa_alrm.sa_mask);
    sa_alrm.sa_flags = 0;

    if (sigaction(SIGALRM, &sa_alrm, NULL) == -1) {
        perror("sigaction(SIGALRM) failed");
        exit(EXIT_FAILURE);
    }

    printf("[PARENT] Signal handlers installed (SIGCHLD, SIGALRM).\n");
}

/* ============================================================================
 * FUNCTION: child_worker
 * Purpose : Executed by each child process after fork().
 *           Simulates a web server worker handling requests.
 *
 * Parameters:
 *   child_index — the 0-based index of this child (used to identify it)
 * ============================================================================ */
static void child_worker(int child_index) {
    pid_t my_pid = getpid();
    pid_t parent_pid = getppid();

    printf("[CHILD-%d] PID=%d | Parent PID=%d | Starting work...\n",
           child_index, my_pid, parent_pid);

    /*
     * Set alarm: if work takes longer than CHILD_TIMEOUT_SEC, SIGALRM fires.
     * This protects against a runaway child that hangs indefinitely.
     * In child_index == UNRESPONSIVE_INDEX, we simulate a 30-second hang.
     */
    alarm(CHILD_TIMEOUT_SEC);

    /* Simulate work: random sleep duration */
    unsigned int work_duration;
    if (child_index == UNRESPONSIVE_INDEX) {
        /* This child is "stuck" — simulates an unresponsive web worker */
        work_duration = UNRESPONSIVE_SLEEP;
        printf("[CHILD-%d] PID=%d | *** SIMULATING UNRESPONSIVE PROCESS *** "
               "(sleeping %u seconds)\n",
               child_index, my_pid, work_duration);
    } else {
        /* Normal child: sleep 1-5 seconds */
        srand((unsigned int)(time(NULL) ^ (my_pid << 4)));
        work_duration = (unsigned int)(rand() % 5) + 1;
        printf("[CHILD-%d] PID=%d | Working for %u second(s)...\n",
               child_index, my_pid, work_duration);
    }

    sleep(work_duration);   /* Simulate actual work */

    /* Cancel the alarm if work finished before timeout */
    alarm(0);

    printf("[CHILD-%d] PID=%d | Work complete. Exiting normally.\n",
           child_index, my_pid);

    exit(EXIT_SUCCESS);   /* Child exits cleanly — parent will receive SIGCHLD */
}

/* ============================================================================
 * FUNCTION: terminate_child
 * Purpose : Attempts to gracefully terminate a child process:
 *           1. Send SIGTERM (polite request to terminate)
 *           2. Wait SIGTERM_WAIT_SEC seconds
 *           3. If still alive, send SIGKILL (forced kill — cannot be ignored)
 *
 * Why SIGTERM first?
 *   SIGTERM allows the child to clean up resources (close files, flush buffers).
 *   SIGKILL is a last resort because it cannot be caught or ignored — the kernel
 *   forcefully destroys the process without any cleanup.
 * ============================================================================ */
static void terminate_child(pid_t pid, int child_index) {
    printf("[PARENT] Child-%d (PID=%d) is unresponsive. Sending SIGTERM...\n",
           child_index, pid);

    /* Step 1: Send SIGTERM */
    if (kill(pid, SIGTERM) == -1) {
        if (errno == ESRCH) {
            printf("[PARENT] Child-%d (PID=%d) already exited.\n", child_index, pid);
            return;   /* Child already gone — no action needed */
        }
        perror("kill(SIGTERM) failed");
        return;
    }

    /* Step 2: Wait for the child to respond to SIGTERM */
    printf("[PARENT] Waiting %d second(s) for Child-%d to respond to SIGTERM...\n",
           SIGTERM_WAIT_SEC, child_index);
    sleep(SIGTERM_WAIT_SEC);

    /* Step 3: Check if child is still alive using kill(pid, 0) */
    /*
     * kill(pid, 0) sends signal number 0 — it does NOT kill the process.
     * It simply checks if the process exists and we have permission to signal it.
     * Returns 0 if process exists, -1 with errno=ESRCH if it doesn't.
     */
    if (kill(pid, 0) == 0) {
        /* Child is still alive — escalate to SIGKILL */
        printf("[PARENT] Child-%d (PID=%d) did not respond to SIGTERM. "
               "Sending SIGKILL...\n", child_index, pid);

        if (kill(pid, SIGKILL) == -1) {
            perror("kill(SIGKILL) failed");
        } else {
            printf("[PARENT] Child-%d (PID=%d) forcefully killed with SIGKILL.\n",
                   child_index, pid);
        }
    } else {
        /* errno == ESRCH means process no longer exists */
        printf("[PARENT] Child-%d (PID=%d) responded to SIGTERM and exited.\n",
               child_index, pid);
    }
}

/* ============================================================================
 * FUNCTION: print_exit_status
 * Purpose : Decode and print the exit status of a reaped child.
 *           Uses POSIX macros to determine HOW the child exited.
 * ============================================================================ */
static void print_exit_status(pid_t pid, int status, int child_index) {
    if (WIFEXITED(status)) {
        /*
         * WIFEXITED(status): True if child called exit() or returned from main().
         * WEXITSTATUS(status): Returns the low 8 bits of the exit code.
         */
        int exit_code = WEXITSTATUS(status);
        if (exit_code == 0) {
            printf("[PARENT] Child-%d (PID=%d) exited NORMALLY (code=0).\n",
                   child_index, pid);
        } else if (exit_code == 42) {
            printf("[PARENT] Child-%d (PID=%d) exited due to ALARM TIMEOUT "
                   "(code=42).\n", child_index, pid);
        } else {
            printf("[PARENT] Child-%d (PID=%d) exited with code=%d.\n",
                   child_index, pid, exit_code);
        }
    } else if (WIFSIGNALED(status)) {
        /*
         * WIFSIGNALED(status): True if child was killed by a signal.
         * WTERMSIG(status): Returns the signal number that killed the child.
         */
        int sig = WTERMSIG(status);
        printf("[PARENT] Child-%d (PID=%d) was killed by signal %d (%s).\n",
               child_index, pid, sig,
               (sig == SIGTERM) ? "SIGTERM" :
               (sig == SIGKILL) ? "SIGKILL" :
               (sig == SIGALRM) ? "SIGALRM" : "OTHER");
    } else {
        printf("[PARENT] Child-%d (PID=%d) terminated abnormally.\n",
               child_index, pid);
    }
}

/* ============================================================================
 * FUNCTION: monitor_children
 * Purpose : After spawning all children, the parent monitors each one.
 *           If a child runs longer than CHILD_TIMEOUT_SEC, terminate it.
 * ============================================================================ */
static void monitor_children(void) {
    printf("\n[PARENT] Monitoring %d child process(es)...\n", NUM_CHILDREN);
    print_separator();

    /*
     * Monitoring approach:
     * We use waitpid() in a polling-with-timeout manner.
     * For each child, we wait up to CHILD_TIMEOUT_SEC seconds.
     * If waitpid() returns 0 (not exited yet), we terminate the child.
     */
    for (int i = 0; i < NUM_CHILDREN; i++) {
        pid_t pid = child_pids[i];
        if (pid <= 0) continue;   /* Invalid PID — skip */

        int status;
        printf("[PARENT] Monitoring Child-%d (PID=%d)...\n", i, pid);

        /*
         * waitpid() with WNOHANG: returns immediately if child hasn't exited.
         * We poll for up to CHILD_TIMEOUT_SEC seconds with 1-second intervals.
         */
        int waited = 0;
        pid_t result;

        while (waited < CHILD_TIMEOUT_SEC) {
            result = waitpid(pid, &status, WNOHANG);

            if (result == pid) {
                /* Child has exited */
                print_exit_status(pid, status, i);
                break;
            } else if (result == -1) {
                /* Error (e.g., ECHILD if child was already reaped by SIGCHLD handler) */
                if (errno == ECHILD) {
                    printf("[PARENT] Child-%d (PID=%d) was already reaped.\n", i, pid);
                } else {
                    perror("waitpid failed");
                }
                break;
            } else {
                /* result == 0: child hasn't exited yet, keep waiting */
                sleep(1);
                waited++;
            }
        }

        /* If loop exhausted (timeout) and child still running — terminate it */
        if (waited >= CHILD_TIMEOUT_SEC) {
            terminate_child(pid, i);

            /* After termination, do a blocking wait to reap the zombie */
            result = waitpid(pid, &status, 0);
            if (result == pid) {
                print_exit_status(pid, status, i);
            }
        }
    }
}

/* ============================================================================
 * FUNCTION: print_separator
 * Purpose : Print a visual separator line for readability.
 * ============================================================================ */
static void print_separator(void) {
    printf("------------------------------------------------------------\n");
}

/* ============================================================================
 * FUNCTION: main
 * Purpose : Entry point — sets up signal handlers, forks child processes,
 *           and monitors them.
 * ============================================================================ */
int main(void) {
    printf("\n");
    print_separator();
    printf("      WEB SERVER PROCESS MONITOR — Linux Lab Assignment Q2\n");
    print_separator();
    printf("[PARENT] Process Monitor started. PID = %d\n", getpid());
    printf("[PARENT] Will create %d worker processes.\n", NUM_CHILDREN);
    printf("[PARENT] Child timeout set to %d seconds.\n", CHILD_TIMEOUT_SEC);
    printf("[PARENT] Child index %d will simulate an unresponsive process.\n",
           UNRESPONSIVE_INDEX);
    print_separator();

    /* Step 1: Install signal handlers BEFORE forking */
    setup_signal_handlers();

    /* Step 2: Seed the random number generator */
    srand((unsigned int)time(NULL));

    /* Step 3: Fork NUM_CHILDREN child processes */
    printf("\n[PARENT] Forking %d child process(es)...\n\n", NUM_CHILDREN);

    for (int i = 0; i < NUM_CHILDREN; i++) {
        pid_t pid = fork();

        if (pid < 0) {
            /* fork() failed — log error, continue trying to spawn remaining */
            fprintf(stderr, "[PARENT] fork() failed for child-%d: %s\n",
                    i, strerror(errno));
            child_pids[i] = -1;   /* Mark as invalid */
            continue;

        } else if (pid == 0) {
            /*
             * ---- CHILD PROCESS ----
             * After fork(), child gets a copy of the parent's address space.
             * child_pids[] in the child is irrelevant — we only use it in parent.
             * The child calls child_worker() and never returns to main().
             */
            child_worker(i);
            /* child_worker() calls exit() internally — we never reach here */
            exit(EXIT_FAILURE);   /* Safety net — should never execute */

        } else {
            /*
             * ---- PARENT PROCESS ----
             * fork() returns the child's PID to the parent.
             * We store it for monitoring.
             */
            child_pids[i] = pid;
            printf("[PARENT] Spawned Child-%d with PID=%d\n", i, pid);
        }
    }

    printf("\n[PARENT] All children spawned. Beginning monitoring phase.\n");

    /* Step 4: Monitor all children */
    monitor_children();

    /* Step 5: Final wait — ensure ALL children are reaped (no zombies) */
    printf("\n[PARENT] Performing final cleanup (waiting for remaining children)...\n");
    int status;
    pid_t reaped;
    while ((reaped = waitpid(-1, &status, WNOHANG)) > 0) {
        printf("[PARENT] Reaped remaining child PID=%d\n", reaped);
    }

    /* Step 6: Print final report */
    print_separator();
    printf("[PARENT] All child processes have been handled.\n");
    printf("[PARENT] Total children reaped by SIGCHLD handler: %d\n",
           children_reaped);
    printf("[PARENT] Process monitor exiting cleanly.\n");
    print_separator();
    printf("\n");

    return EXIT_SUCCESS;
}
