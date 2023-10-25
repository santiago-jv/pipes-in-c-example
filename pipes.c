#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>

// *** Pipes ***
// [0] -> read
// [1] -> write
// close(pipe_files_descriptor[0]); -> close read
// close(pipe_files_descriptor[1]); -> close write
// pipe(pipe_files_descriptor); -> create pipe
// write(pipe_files_descriptor[1], buffer, sizeof(buffer)); -> write
// read(pipe_files_descriptor[0], buffer, sizeof(buffer)); -> read
//  buffer[bytes_read] = '\0'; -> end of string after read from pipe

void create_pipe(int pipe_files_descriptor[2]) {
    if (pipe(pipe_files_descriptor) == -1) {
        perror("Error al crear la tubería");
        exit(EXIT_FAILURE);
    }
}

pid_t create_process() {
    pid_t pid = fork();

    if(pid == -1) {
        perror("Error al crear el proceso hijo");
        exit(EXIT_FAILURE);
    }

    return pid;
}

int main() {
    int pipe_files_descriptor[2];
    create_pipe(pipe_files_descriptor);

    pid_t pid = create_process();

    //Padre
    if(pid == 0) {
        close(pipe_files_descriptor[0]);
        char buffer[100] = "Este es un mensaje desde el padre";
        write(pipe_files_descriptor[1], buffer, sizeof(buffer));
    }
    //Hijo
    else {
        close(pipe_files_descriptor[1]);
        char buffer_out[100];
        int byte_size = read(pipe_files_descriptor[0], buffer_out, sizeof(buffer_out));
        buffer_out[byte_size] = '\0';

        printf("Mensaje recibido: %s\n", buffer_out);
    }
    

    return EXIT_SUCCESS;
}