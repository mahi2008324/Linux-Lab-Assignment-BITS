/*
 * ============================================================================
 * File        : employee_records.c
 * Description : Secure file-processing utility using ONLY Linux system calls.
 *               Implements an employee database with:
 *               - create/open file using open()
 *               - write employee records using write()
 *               - read records using read()
 *               - random access using lseek()
 *               - update specific records in-place without rewriting the file
 *               - close the file using close()
 *
 * Author      : Linux Lab Assignment — Question 3
 * Date        : 2026-07-20
 * Standard    : C11 / POSIX.1-2008
 * Compile     : gcc -Wall -Wextra -std=c11 -D_POSIX_C_SOURCE=200809L \
 *                     employee_records.c -o employee_records
 * Usage       : ./employee_records
 *
 * NOTE: This program deliberately does NOT use stdio functions (fopen,
 *       fread, fwrite, fclose, fprintf, etc.). Only Linux system calls
 *       (open, read, write, lseek, close) are used for file operations.
 * ============================================================================
 */

#define _POSIX_C_SOURCE 200809L

/* ---- Headers ---- */
#include <stdio.h>       /* printf, perror (only for user output — NOT file I/O) */
#include <stdlib.h>      /* exit, EXIT_SUCCESS, EXIT_FAILURE */
#include <string.h>      /* memset, strncpy, strcmp */
#include <unistd.h>      /* open, read, write, lseek, close (POSIX system calls) */
#include <fcntl.h>       /* open flags: O_RDWR, O_CREAT, O_TRUNC */
#include <sys/types.h>   /* off_t, ssize_t */
#include <sys/stat.h>    /* S_IRUSR, S_IWUSR (file permission bits) */
#include <errno.h>       /* errno */

/* ============================================================================
 * EMPLOYEE RECORD STRUCTURE
 *
 * Fixed-size records are essential for lseek()-based random access.
 * If records had variable lengths, we would not be able to jump directly
 * to record N using a simple formula: offset = N * sizeof(Employee)
 * ============================================================================ */
#define MAX_NAME_LEN    40    /* Maximum character length for employee name    */
#define MAX_DEPT_LEN    30    /* Maximum character length for department name  */
#define DB_FILENAME     "employees.dat"   /* Binary database file name         */

typedef struct {
    int    employee_id;              /* Unique employee identifier (integer)   */
    char   name[MAX_NAME_LEN];       /* Full name — padded to fixed width      */
    char   department[MAX_DEPT_LEN]; /* Department — padded to fixed width     */
    double salary;                   /* Annual salary (double precision float) */
    int    years_of_service;         /* Number of years employed               */
    int    is_active;                /* 1 = active employee, 0 = terminated    */
} Employee;

/* Total number of initial records to write */
#define NUM_INITIAL_RECORDS   5

/* ============================================================================
 * FUNCTION: write_string
 * Purpose : Write a string to a file descriptor (system call wrapper).
 *           Used for printing text output without stdio.
 * ============================================================================ */
static void write_string(int fd, const char *str) {
    size_t len = strlen(str);
    ssize_t written = write(fd, str, len);
    if (written == -1) {
        perror("write_string: write() failed");
    }
}

/* ============================================================================
 * FUNCTION: print_separator
 * Purpose : Print a visual separator to stdout using write() system call.
 * ============================================================================ */
static void print_separator(void) {
    write_string(STDOUT_FILENO,
        "------------------------------------------------------------\n");
}

/* ============================================================================
 * FUNCTION: print_employee
 * Purpose : Display employee record using printf (terminal output only).
 *           Note: printf is used for DISPLAY purposes (to stdout), while
 *           all FILE I/O uses system calls only.
 * ============================================================================ */
static void print_employee(const Employee *emp, int index) {
    printf("  Record #%d\n", index);
    printf("  ├─ Employee ID    : %d\n",   emp->employee_id);
    printf("  ├─ Name           : %s\n",   emp->name);
    printf("  ├─ Department     : %s\n",   emp->department);
    printf("  ├─ Salary         : $%.2f\n", emp->salary);
    printf("  ├─ Years of Svc   : %d\n",   emp->years_of_service);
    printf("  └─ Status         : %s\n",
           emp->is_active ? "Active" : "Terminated");
}

/* ============================================================================
 * FUNCTION: open_database
 * Purpose : Open (or create) the employee database file using open().
 *
 * open() System Call:
 *   int open(const char *pathname, int flags, mode_t mode);
 *   - pathname : file path
 *   - flags    : combination of O_RDWR (read+write), O_CREAT (create if not
 *                exists), O_TRUNC (truncate to zero length)
 *   - mode     : permissions for newly created file (only used with O_CREAT)
 *
 * Returns: file descriptor (non-negative integer) on success, -1 on failure.
 * ============================================================================ */
static int open_database(const char *filename, int flags) {
    /*
     * File Permission Bits (used when O_CREAT is specified):
     *   S_IRUSR = 0400 = owner read
     *   S_IWUSR = 0200 = owner write
     *   Combined: 0600 = rw------- (only owner can read/write)
     */
    int fd = open(filename, flags, S_IRUSR | S_IWUSR);

    if (fd == -1) {
        perror("open_database: open() failed");
        exit(EXIT_FAILURE);
    }

    printf("[open()] File descriptor obtained: fd=%d for '%s'\n", fd, filename);
    return fd;
}

/* ============================================================================
 * FUNCTION: write_record
 * Purpose : Write a single Employee record to the database file at the
 *           current file offset using write().
 *
 * write() System Call:
 *   ssize_t write(int fd, const void *buf, size_t count);
 *   - fd    : file descriptor returned by open()
 *   - buf   : pointer to data to write
 *   - count : number of bytes to write
 *   Returns: number of bytes written, or -1 on error.
 *
 * Why use write() instead of fwrite()?
 *   - write() directly invokes the kernel — no buffering layer
 *   - Provides more control over partial writes
 *   - Required for signal-safe, low-level I/O
 * ============================================================================ */
static int write_record(int fd, const Employee *emp) {
    ssize_t bytes_written = write(fd, emp, sizeof(Employee));

    if (bytes_written == -1) {
        perror("write_record: write() failed");
        return -1;
    }

    if ((size_t)bytes_written != sizeof(Employee)) {
        fprintf(stderr, "write_record: partial write (%zd of %zu bytes)\n",
                bytes_written, sizeof(Employee));
        return -1;
    }

    printf("[write()] Wrote %zd bytes | Employee ID=%d | Name='%s'\n",
           bytes_written, emp->employee_id, emp->name);
    return 0;
}

/* ============================================================================
 * FUNCTION: read_record
 * Purpose : Read a single Employee record from the current file offset.
 *
 * read() System Call:
 *   ssize_t read(int fd, void *buf, size_t count);
 *   - fd    : file descriptor
 *   - buf   : buffer to store read data
 *   - count : number of bytes to read
 *   Returns: number of bytes read (0 on EOF, -1 on error).
 *
 * Why use read() instead of fread()?
 *   - No stdio buffering — reads exactly what we ask for
 *   - EOF detection via return value of 0
 *   - Works correctly with lseek() for random access
 * ============================================================================ */
static int read_record(int fd, Employee *emp) {
    memset(emp, 0, sizeof(Employee));   /* Zero-initialize the buffer first */

    ssize_t bytes_read = read(fd, emp, sizeof(Employee));

    if (bytes_read == -1) {
        perror("read_record: read() failed");
        return -1;
    }

    if (bytes_read == 0) {
        /* End of file — no more records */
        return 0;
    }

    if ((size_t)bytes_read != sizeof(Employee)) {
        fprintf(stderr, "read_record: partial read (%zd of %zu bytes)\n",
                bytes_read, sizeof(Employee));
        return -1;
    }

    printf("[read()] Read %zd bytes | Employee ID=%d\n",
           bytes_read, emp->employee_id);
    return 1;   /* Success */
}

/* ============================================================================
 * FUNCTION: seek_to_record
 * Purpose : Move the file cursor to a specific record using lseek().
 *
 * lseek() System Call:
 *   off_t lseek(int fd, off_t offset, int whence);
 *   - fd     : file descriptor
 *   - offset : byte offset to seek to
 *   - whence : reference point:
 *              SEEK_SET = from beginning of file
 *              SEEK_CUR = from current position
 *              SEEK_END = from end of file
 *   Returns: new file offset from beginning of file, or -1 on error.
 *
 * Random Access Formula:
 *   record_offset = record_number * sizeof(Employee)
 *   (Works only because records are fixed-size!)
 * ============================================================================ */
static off_t seek_to_record(int fd, int record_number) {
    off_t byte_offset = (off_t)record_number * (off_t)sizeof(Employee);

    off_t result = lseek(fd, byte_offset, SEEK_SET);

    if (result == (off_t)-1) {
        perror("seek_to_record: lseek() failed");
        return -1;
    }

    printf("[lseek()] Seeked to record #%d | Byte offset = %ld\n",
           record_number, (long)result);
    return result;
}

/* ============================================================================
 * FUNCTION: create_initial_records
 * Purpose : Write the initial 5 employee records to the database.
 * ============================================================================ */
static void create_initial_records(int fd) {
    Employee records[NUM_INITIAL_RECORDS] = {
        {
            .employee_id      = 1001,
            .name             = "Alice Johnson",
            .department       = "Engineering",
            .salary           = 85000.00,
            .years_of_service = 5,
            .is_active        = 1
        },
        {
            .employee_id      = 1002,
            .name             = "Bob Martinez",
            .department       = "Marketing",
            .salary           = 62000.00,
            .years_of_service = 3,
            .is_active        = 1
        },
        {
            .employee_id      = 1003,
            .name             = "Carol Singh",
            .department       = "Human Resources",
            .salary           = 70000.00,
            .years_of_service = 7,
            .is_active        = 1
        },
        {
            .employee_id      = 1004,
            .name             = "David Chen",
            .department       = "Finance",
            .salary           = 95000.00,
            .years_of_service = 10,
            .is_active        = 1
        },
        {
            .employee_id      = 1005,
            .name             = "Emma Wilson",
            .department       = "Engineering",
            .salary           = 78000.00,
            .years_of_service = 2,
            .is_active        = 1
        }
    };

    printf("\n[INFO] Writing %d initial employee records...\n", NUM_INITIAL_RECORDS);
    print_separator();

    for (int i = 0; i < NUM_INITIAL_RECORDS; i++) {
        if (write_record(fd, &records[i]) == -1) {
            fprintf(stderr, "ERROR: Failed to write record %d\n", i);
            exit(EXIT_FAILURE);
        }
    }

    printf("[INFO] All initial records written successfully.\n");
    printf("[INFO] Total bytes written: %zu bytes\n",
           NUM_INITIAL_RECORDS * sizeof(Employee));
}

/* ============================================================================
 * FUNCTION: read_all_records
 * Purpose : Read and display all records from the database by seeking to
 *           the beginning of the file and reading sequentially.
 * ============================================================================ */
static void read_all_records(int fd) {
    Employee emp;
    int record_num = 0;

    printf("\n[INFO] Reading all employee records...\n");
    print_separator();

    /* Seek to the beginning of the file */
    if (lseek(fd, 0, SEEK_SET) == (off_t)-1) {
        perror("lseek to start failed");
        return;
    }
    printf("[lseek()] Reset to beginning of file (offset = 0)\n\n");

    /* Read records sequentially until EOF */
    int result;
    while ((result = read_record(fd, &emp)) == 1) {
        print_employee(&emp, record_num);
        printf("\n");
        record_num++;
    }

    if (result == -1) {
        fprintf(stderr, "ERROR: Failed to read records\n");
    }

    printf("[INFO] Total records read: %d\n", record_num);
}

/* ============================================================================
 * FUNCTION: read_specific_record
 * Purpose : Use lseek() to jump directly to a specific record and read it.
 *           Demonstrates random access without reading intermediate records.
 * ============================================================================ */
static void read_specific_record(int fd, int record_number) {
    Employee emp;

    printf("\n[INFO] Random access — reading record #%d directly...\n", record_number);
    print_separator();

    /* Jump to the record's byte offset */
    if (seek_to_record(fd, record_number) == (off_t)-1) {
        fprintf(stderr, "ERROR: Failed to seek to record %d\n", record_number);
        return;
    }

    /* Read exactly one record from current position */
    int result = read_record(fd, &emp);
    if (result == 1) {
        printf("\n");
        print_employee(&emp, record_number);
        printf("\n");
    } else {
        printf("[INFO] Record #%d not found (possibly past end of file).\n",
               record_number);
    }
}

/* ============================================================================
 * FUNCTION: update_record
 * Purpose : Update a specific record IN-PLACE using lseek() + write().
 *           This avoids rewriting the entire file — only the target record
 *           is modified.
 *
 * How it works:
 *   1. lseek() jumps to the byte offset of the target record
 *   2. write() overwrites exactly sizeof(Employee) bytes at that offset
 *   3. Other records are NOT touched
 * ============================================================================ */
static void update_record(int fd, int record_number, double new_salary,
                           int new_years) {
    Employee emp;

    printf("\n[INFO] Updating record #%d in-place...\n", record_number);
    print_separator();

    /* Step 1: Seek to the target record */
    if (seek_to_record(fd, record_number) == (off_t)-1) {
        fprintf(stderr, "ERROR: Seek failed for update\n");
        return;
    }

    /* Step 2: Read the current record */
    int result = read_record(fd, &emp);
    if (result != 1) {
        fprintf(stderr, "ERROR: Could not read record %d for update\n",
                record_number);
        return;
    }

    printf("[INFO] Record BEFORE update:\n");
    print_employee(&emp, record_number);
    printf("\n");

    /* Step 3: Modify the fields */
    emp.salary           = new_salary;
    emp.years_of_service = new_years;

    /* Step 4: Seek BACK to the record start (read() advanced the offset) */
    if (seek_to_record(fd, record_number) == (off_t)-1) {
        fprintf(stderr, "ERROR: Seek back failed for update\n");
        return;
    }

    /* Step 5: Overwrite the record with updated data */
    if (write_record(fd, &emp) == -1) {
        fprintf(stderr, "ERROR: Write failed during update\n");
        return;
    }

    printf("[INFO] Record AFTER update:\n");
    print_employee(&emp, record_number);
    printf("[INFO] Record #%d updated in-place successfully.\n", record_number);
}

/* ============================================================================
 * FUNCTION: close_database
 * Purpose : Close the file descriptor using close().
 *
 * close() System Call:
 *   int close(int fd);
 *   - Releases the file descriptor so it can be reused
 *   - Flushes any kernel-buffered data to the device
 *   - Returns 0 on success, -1 on error
 *
 * Why check the return value of close()?
 *   close() on NFS files may report write errors that were deferred during
 *   write(). Always check its return value on writable files.
 * ============================================================================ */
static void close_database(int fd) {
    if (close(fd) == -1) {
        perror("close_database: close() failed");
        exit(EXIT_FAILURE);
    }
    printf("[close()] File descriptor %d closed successfully.\n", fd);
}

/* ============================================================================
 * FUNCTION: main
 * Purpose : Demonstrates all employee database operations.
 * ============================================================================ */
int main(void) {
    printf("\n");
    print_separator();
    write_string(STDOUT_FILENO,
        "   EMPLOYEE DATABASE — Linux System Calls Demo (Q3)\n");
    print_separator();
    printf("Record Size : %zu bytes per employee\n", sizeof(Employee));
    printf("Database    : %s\n\n", DB_FILENAME);

    /* ---- Phase 1: Create and Write ---- */
    printf("=== PHASE 1: CREATE DATABASE & WRITE RECORDS ===\n");

    /*
     * open() flags:
     *   O_RDWR  : Open for both reading and writing
     *   O_CREAT : Create file if it doesn't exist
     *   O_TRUNC : Truncate existing file to zero length (fresh start)
     */
    int fd = open_database(DB_FILENAME, O_RDWR | O_CREAT | O_TRUNC);
    create_initial_records(fd);

    /* ---- Phase 2: Read All Records ---- */
    printf("\n=== PHASE 2: READ ALL RECORDS ===\n");
    read_all_records(fd);

    /* ---- Phase 3: Random Access Read ---- */
    printf("\n=== PHASE 3: RANDOM ACCESS READ ===\n");
    read_specific_record(fd, 3);   /* Jump directly to record #3 (David Chen) */

    /* ---- Phase 4: Update a Record In-Place ---- */
    printf("\n=== PHASE 4: UPDATE RECORD IN-PLACE ===\n");
    /* Give Bob Martinez a raise and update years of service */
    update_record(fd, 1, 75000.00, 4);

    /* ---- Phase 5: Verify the Update ---- */
    printf("\n=== PHASE 5: VERIFY UPDATE (read all again) ===\n");
    read_all_records(fd);

    /* ---- Phase 6: Check File Size ---- */
    off_t file_size = lseek(fd, 0, SEEK_END);
    printf("\n=== PHASE 6: FILE INFORMATION ===\n");
    print_separator();
    printf("[lseek()] Total file size: %ld bytes\n", (long)file_size);
    printf("[INFO]    Expected size  : %zu bytes (%d records × %zu bytes each)\n",
           NUM_INITIAL_RECORDS * sizeof(Employee),
           NUM_INITIAL_RECORDS, sizeof(Employee));

    /* ---- Phase 7: Close ---- */
    printf("\n=== PHASE 7: CLOSE DATABASE ===\n");
    print_separator();
    close_database(fd);

    printf("\n");
    print_separator();
    printf("   All operations completed successfully!\n");
    print_separator();
    printf("\n");

    return EXIT_SUCCESS;
}
