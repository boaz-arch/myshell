#include "jobs.h"

#include <stdio.h>
#include <string.h>

#define MAX_JOBS 1000

static Job jobs[MAX_JOBS];

static int job_count = 0;
static int next_job_id = 1;

void jobs_init(void) {
    job_count = 0;
    next_job_id = 1;
}

int add_job(pid_t pid, const char *command) {
    if (job_count >= MAX_JOBS)
        return -1;

    jobs[job_count].job_id = next_job_id++;
    jobs[job_count].pid = pid;
    jobs[job_count].state = JOB_RUNNING;

    strncpy(jobs[job_count].command,
            command,
            sizeof(jobs[job_count].command) - 1);

    jobs[job_count].command[sizeof(jobs[job_count].command) - 1] = '\0';

    job_count++;
    
    return jobs[job_count - 1].job_id;
}

void jobs_print(void) {
    for (int i = 0; i < job_count; i++) {
    
        const char *state;
        
        if (jobs[i].state == JOB_RUNNING) state = "Running";
        else if (jobs[i].state == JOB_STOPPED) state = "Stopped";
        else state = "Done";
        
        printf("[%d] %s %s\n",
                jobs[i].job_id,
                state,
                jobs[i].command);
    }
}


Job *find_job(pid_t pid) {
    for (int i = 0; i < job_count; i++) {
        if (jobs[i].pid == pid)
            return &jobs[i];
    }

    return NULL;
}


void mark_job_stopped(pid_t pid){
    Job * job = find_job(pid);
    
    if (job != NULL)
        job->state = JOB_STOPPED;
}


void mark_job_done(pid_t pid){
    Job * job = find_job(pid);
    
    if (job != NULL)
        job->state = JOB_DONE;
}


void remove_job(pid_t pid) {
    for (int i = 0; i < job_count; i++) {

        if (jobs[i].pid == pid) {

            for (int j = i; j < job_count - 1; j++)
                jobs[j] = jobs[j + 1];

            job_count--;
            return;
        }
    }
}

void jobs_check_finished(void) {
    int i = 0;

    while (i < job_count) {

        if (jobs[i].state == JOB_DONE) {

            printf("[%d] Done                    %s\n",
                   jobs[i].job_id,
                   jobs[i].command);

            remove_job(jobs[i].pid);

        } else {
            i++;
        }
    }
}

