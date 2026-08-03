#ifndef JOBS_H
#define JOBS_H

#include <sys/types.h>

typedef struct {
    int job_id;
    pid_t pid;
    char command[256];
    int finished;
} Job;

void jobs_init(void);
void add_job(pid_t pid, const char *command);
void jobs_print(void);
void remove_job(pid_t pid);
Job *find_job(pid_t pid);

void mark_job_finished(pid_t pid);
void jobs_check_finished(void);

#endif

