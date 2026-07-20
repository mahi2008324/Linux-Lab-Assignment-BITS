#!/usr/bin/env bash
# =============================================================================
# Script Name  : duplicate_backup.sh
# Description  : Identifies duplicate submissions, backs up unique files,
#                generates a summary report, and logs errors separately.
# Author       : Linux Lab Assignment — Question 1
# Date         : 2026-07-20
# Shell        : Bash 3.x+ (POSIX-compatible, works on macOS and Linux)
# Usage        : ./duplicate_backup.sh
# =============================================================================

# ---------------------------------------------------------------------------
# STRICT MODE
# -e  : Exit immediately on any error
# -u  : Treat unset variables as errors
# -o pipefail : Fail the pipeline if any command fails
# ---------------------------------------------------------------------------
set -euo pipefail

# ---------------------------------------------------------------------------
# GLOBAL CONFIGURATION VARIABLES
# ---------------------------------------------------------------------------
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"   # Absolute path to script location
SOURCE_DIR="${SCRIPT_DIR}/sample_submissions"                  # Where student submissions are stored
BACKUP_DIR="${SCRIPT_DIR}/backup"                              # Destination for unique file backups
REPORT_DIR="${SCRIPT_DIR}/reports"                             # Directory to store generated reports
ERROR_LOG="${SCRIPT_DIR}/errors.log"                           # File to capture all error messages
TIMESTAMP="$(date '+%Y%m%d_%H%M%S')"                          # Unique timestamp for report filename
REPORT_FILE="${REPORT_DIR}/backup_report_${TIMESTAMP}.txt"    # Full path of the generated report
FILE_EXTENSION="*.txt"                                         # Target file extension to process

# Global temp file for hash-to-filename lookup (set in process_submissions)
HASH_STORE=""

# ---------------------------------------------------------------------------
# COUNTERS (used for the final report)
# ---------------------------------------------------------------------------
total_files=0       # Number of files found in the source directory
duplicate_count=0   # Number of duplicate files detected
unique_count=0      # Number of unique files successfully backed up

# ---------------------------------------------------------------------------
# ARRAYS for collecting duplicate info
# ---------------------------------------------------------------------------
duplicate_list=()   # Stores human-readable duplicate entries
unique_list=()      # Stores names of unique backed-up files

# =============================================================================
# FUNCTION: cleanup
# Purpose : Remove temporary files on script exit (normal or error).
#           Registered via 'trap' at the global level so it works correctly
#           regardless of which function the exit originates from.
# =============================================================================
cleanup() {
    if [[ -n "${HASH_STORE}" && -f "${HASH_STORE}" ]]; then
        rm -f "${HASH_STORE}"
    fi
}
trap cleanup EXIT

# =============================================================================
# FUNCTION: log_error
# Purpose : Append error messages to the errors.log file with a timestamp.
#           Errors are redirected to the log file using >> (append redirection).
# =============================================================================
log_error() {
    local message="$1"
    # Append to error log using >> (append redirection operator)
    echo "[$(date '+%Y-%m-%d %H:%M:%S')] ERROR: ${message}" >> "${ERROR_LOG}"
    # Also print to stderr using >&2 so the error is visible in the terminal
    echo "ERROR: ${message}" >&2
}

# =============================================================================
# FUNCTION: log_info
# Purpose : Print informational messages to stdout with a prefix.
# =============================================================================
log_info() {
    local message="$1"
    echo "[INFO] ${message}"
}

# =============================================================================
# FUNCTION: setup_directories
# Purpose : Create required directories if they do not already exist.
#           Uses mkdir -p which creates parent directories as needed and does
#           NOT fail if the directory already exists.
# =============================================================================
setup_directories() {
    log_info "Setting up required directories..."

    # Create backup directory if it does not exist
    if ! mkdir -p "${BACKUP_DIR}" 2>>"${ERROR_LOG}"; then
        log_error "Failed to create backup directory: ${BACKUP_DIR}"
        exit 1
    fi

    # Create reports directory if it does not exist
    if ! mkdir -p "${REPORT_DIR}" 2>>"${ERROR_LOG}"; then
        log_error "Failed to create reports directory: ${REPORT_DIR}"
        exit 1
    fi

    log_info "Directories ready: backup/ and reports/"
}

# =============================================================================
# FUNCTION: validate_source
# Purpose : Confirm that the source directory exists and is readable.
#           If the source directory is missing, we log the error and exit.
# =============================================================================
validate_source() {
    log_info "Validating source directory: ${SOURCE_DIR}"

    if [[ ! -d "${SOURCE_DIR}" ]]; then
        log_error "Source directory does not exist: ${SOURCE_DIR}"
        exit 1
    fi

    if [[ ! -r "${SOURCE_DIR}" ]]; then
        log_error "Source directory is not readable: ${SOURCE_DIR}"
        exit 1
    fi

    log_info "Source directory is valid and readable."
}

# =============================================================================
# FUNCTION: compute_md5
# Purpose : Compute the MD5 checksum of a given file.
#           md5sum produces a 128-bit hash. Two files with identical content
#           will produce the same hash, enabling duplicate detection.
#           Handles both Linux (md5sum) and macOS (md5) transparently.
# Parameters:
#   $1 — Full path to the file
# Returns  : MD5 hash string via stdout
# =============================================================================
compute_md5() {
    local filepath="$1"
    if command -v md5sum >/dev/null 2>&1; then
        # Linux: md5sum outputs "<hash>  <filename>"
        md5sum "${filepath}" 2>>"${ERROR_LOG}" | awk '{print $1}'
    elif command -v md5 >/dev/null 2>&1; then
        # macOS: md5 outputs "MD5 (filename) = <hash>"
        md5 "${filepath}" 2>>"${ERROR_LOG}" | awk '{print $NF}'
    else
        log_error "No MD5 tool found (md5sum or md5 required)"
        exit 1
    fi
}

# =============================================================================
# FUNCTION: generate_report
# Purpose : Write a professional summary report to the reports/ directory.
#           All output is redirected using > (overwrite) into the report file.
# =============================================================================
generate_report() {
    log_info "Generating backup report: ${REPORT_FILE}"

    {
        echo "============================================================"
        echo "         UNIVERSITY SUBMISSION BACKUP REPORT"
        echo "============================================================"
        echo "Date and Time    : $(date)"
        echo "Source Directory : ${SOURCE_DIR}"
        echo "Backup Directory : ${BACKUP_DIR}"
        echo "------------------------------------------------------------"
        echo "Total files processed : ${total_files}"
        echo "Duplicate files found : ${duplicate_count}"
        echo "Unique files backed up: ${unique_count}"
        echo "------------------------------------------------------------"

        echo ""
        echo "DUPLICATE FILES DETECTED:"
        if [[ ${duplicate_count} -eq 0 ]]; then
            echo "  (none — all submissions are unique)"
        else
            local entry
            for entry in "${duplicate_list[@]}"; do
                echo "  ${entry}"
            done
        fi

        echo ""
        echo "------------------------------------------------------------"

        echo ""
        echo "UNIQUE FILES BACKED UP:"
        if [[ ${unique_count} -eq 0 ]]; then
            echo "  (none — no unique files found)"
        else
            local fname
            for fname in "${unique_list[@]}"; do
                echo "  [OK] ${fname}"
            done
        fi

        echo ""
        echo "============================================================"
        echo "                  END OF REPORT"
        echo "============================================================"

    } > "${REPORT_FILE}"

    log_info "Report saved to: ${REPORT_FILE}"
}

# =============================================================================
# FUNCTION: process_submissions
# Purpose : Core function — iterates over all files in the source directory,
#           computes MD5 for each, detects duplicates using a portable
#           temp-file hash-lookup table (works on bash 3.x macOS & bash 4.x Linux),
#           and backs up unique files.
# =============================================================================
process_submissions() {
    log_info "Starting submission processing..."

    # Create a temporary file used as a hash-to-filename lookup table.
    # Each line format: "<md5hash>  <filename>"
    # We use grep to check if a hash already exists (portable, no assoc arrays needed).
    HASH_STORE="$(mktemp /tmp/dup_backup_hashes.XXXXXX)"
    log_info "Hash store created at: ${HASH_STORE}"

    # Build file list using find with null-delimited output (safe for filenames with spaces).
    local file_list=()
    local fp
    while IFS= read -r -d '' fp; do
        file_list+=("${fp}")
    done < <(find "${SOURCE_DIR}" -maxdepth 1 -type f -name "${FILE_EXTENSION}" -print0 2>>"${ERROR_LOG}" | sort -z)

    # Check if any files were found
    if [[ ${#file_list[@]} -eq 0 ]]; then
        log_error "No ${FILE_EXTENSION} files found in ${SOURCE_DIR}"
        echo "No files to process. Exiting."
        exit 0
    fi

    total_files=${#file_list[@]}
    log_info "Found ${total_files} file(s) to process."

    # ---------- Iterate over each file ----------
    local filepath filename current_md5 original_file
    for filepath in "${file_list[@]}"; do
        filename="$(basename "${filepath}")"

        # Validate: file must be readable
        if [[ ! -r "${filepath}" ]]; then
            log_error "Cannot read file (skipping): ${filepath}"
            continue
        fi

        # Compute MD5 hash of the current file
        current_md5="$(compute_md5 "${filepath}")"

        # Validate: MD5 computation must succeed
        if [[ -z "${current_md5}" ]]; then
            log_error "MD5 computation failed for: ${filepath}"
            continue
        fi

        # ---------- Duplicate Check ----------
        # grep -q searches the hash_store for this MD5.
        # Returns exit code 0 (found) or 1 (not found) — no output printed.
        # Use set +e temporarily so grep's exit-1 doesn't trigger set -e.
        set +e
        grep -q "^${current_md5}  " "${HASH_STORE}" 2>/dev/null
        local grep_result=$?
        set -e

        if [[ ${grep_result} -eq 0 ]]; then
            # Hash already exists → DUPLICATE
            original_file="$(grep "^${current_md5}  " "${HASH_STORE}" | cut -d' ' -f3)"
            duplicate_list+=("[DUPLICATE] ${filename}  (identical to: ${original_file})")
            (( duplicate_count++ )) || true
            log_info "Duplicate detected: ${filename} == ${original_file}"
        else
            # Hash is NEW → store it and back up the file
            printf '%s  %s\n' "${current_md5}" "${filename}" >> "${HASH_STORE}"

            # Copy unique file to backup directory with preserved attributes
            if cp -p "${filepath}" "${BACKUP_DIR}/${filename}" 2>>"${ERROR_LOG}"; then
                unique_list+=("${filename}")
                (( unique_count++ )) || true
                log_info "Backed up: ${filename}"
            else
                log_error "Failed to back up file: ${filepath}"
            fi
        fi
    done

    log_info "Processing complete."
}

# =============================================================================
# FUNCTION: print_summary
# Purpose : Print a concise summary to stdout after execution.
# =============================================================================
print_summary() {
    echo ""
    echo "========================================"
    echo "         EXECUTION SUMMARY"
    echo "========================================"
    echo "  Total Processed   : ${total_files}"
    echo "  Duplicates Found  : ${duplicate_count}"
    echo "  Unique (Backed Up): ${unique_count}"
    echo "  Report : ${REPORT_FILE}"
    echo "  Errors : ${ERROR_LOG}"
    echo "========================================"
    echo ""
}

# =============================================================================
# MAIN ENTRY POINT
# =============================================================================
main() {
    echo ""
    echo "============================================================"
    echo "   University Submission Duplicate Detector & Backup Tool"
    echo "============================================================"
    echo ""

    # Clear error log at start of run (> truncates file)
    > "${ERROR_LOG}" 2>/dev/null || true

    validate_source
    setup_directories
    process_submissions
    generate_report
    print_summary
}

main "$@"
