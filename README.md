gcc -Wall -Wextra -I./include \
./src/shell/shell.c \
./src/parser/parser.c \
./src/executor/executer.c \
./src/builtin/builtin.c \
./src/history/history.c \
./src/redirection/redirection.c \
-o myshell
