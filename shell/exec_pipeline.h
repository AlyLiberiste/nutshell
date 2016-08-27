#ifndef EXEC_PIPELINE_H
#define EXEC_PIPELINE_H

#include <tparse.h>

void exec_pipeline_one_command(pipeline_t *);
void exec_pipeline_redir_input(pipeline_t *, int);
void exec_pipeline_redir_output(pipeline_t *, int);
void exec_pipeline_redir_input_output(pipeline_t *, int);
void exec_pipeline_many_commands(pipeline_t *, int);
int execute_pipeline(pipeline_t * , int[], int);
#endif /* EXEC_PIPELINE_H*/

