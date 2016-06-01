
void cd(char * argv[]);

char * parse_argument(int argc, char ** argv);

int parse_input(char * cmd, char * argv[]);

void execute_block(char * argv[]);

void execute(char * argv[]);

void processStatus(pid_t childPid, int status);


