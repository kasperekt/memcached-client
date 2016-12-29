#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>

const char MC_ACCEPT = 0xFF;
const char MC_FAILURE = 0x00;

static const uint32_t CHUNK_SIZE = 256;

void draw_progress(unsigned long total, unsigned long downloaded) {
    float progress = ((float)downloaded / (float)total) * 100.0;
    // printf("%c[2K", 27);
    printf("\rDownloading: %d%%", (int)progress);
    fflush(stdout);
}

int main(int argc, char** argv) {
    if (argc < 4) {
        fprintf(stderr, "You should pass 3 arguments: mc_client [ip] [port] [requested-file]\n");
        exit(1);
    }

    char* ip_str = argv[1];
    char* port_str = argv[2];
    char* requested_file = argv[3];

    struct sockaddr_in sck_addr;
    int sck;

    memset(&sck_addr, 0, sizeof sck_addr);
    sck_addr.sin_family = AF_INET;
    inet_aton(ip_str, &sck_addr.sin_addr);
    sck_addr.sin_port = htons(atoi(port_str));

    if ((sck = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
        fprintf(stderr, "Cannot create socket\n");
        exit(1);
    }

    if (connect(sck, (struct sockaddr*) &sck_addr, sizeof sck_addr) < 0) {
        fprintf(stderr, "No connection\n");
        exit(1);
    }

    int w_status = write(sck, requested_file, strlen(requested_file) + 1);
    if (w_status < 0) {
        fprintf(stderr, "Error writing to socket. Status: %d", w_status);
        exit(1);
    }

    char status;
    read(sck, &status, 1);

    unsigned long fsize;
    read(sck, &fsize, sizeof(fsize));

    switch (status) {
    case MC_ACCEPT: {
        printf("File exists with size of: %lu\n", fsize);
        
        FILE* fp = fopen(requested_file, "wb");
        unsigned long bytes_read = 0;
        char buffer[CHUNK_SIZE];
        
        while (bytes_read < fsize) {
           bytes_read += read(sck, &buffer, CHUNK_SIZE);
           fwrite(&buffer, sizeof buffer, 1, fp);
           draw_progress(fsize, bytes_read);
        }
        
        printf("\nFinished\n");
        fclose(fp);
        break;
    }
    case MC_FAILURE:
        printf("File <<%s>> doesnt exist on the server\n", requested_file);
        close(sck);
        exit(2);
        break;
    default:
        printf("Status: <<%d>>\n", status);
    }


    close(sck);
    return 0;
}