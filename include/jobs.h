#ifndef JOBS_H
#define JOBS_H

#include <sys/types.h>

typedef enum {
    JOB_RUNNING, 
    JOB_STOPPED,
    JOB_DONE
} JobState;

typedef struct {
    int job_id;
    pid_t pid;
    char command[256];
    
    JobState state;
} Job;

void jobs_init(void);

int add_job(pid_t pid, const char *command);
void jobs_print(void);

Job *find_job(pid_t pid);
void mark_job_stopped(pid_t pid);
void mark_job_done(pid_t pid);
void remove_job(pid_t pid);

void jobs_check_finished(void);

#endif

