#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <pthread.h>
#include <string.h>

// TO-DO update the arguments the program receives
#define MSG "Usage: ./fileio <num_threads>"

typedef struct {
    int *index;
    int *file_no_threads;
    char *file_access_pattern;
    int *file_size_in_bytes_var;
    int *record_size_in_bytes_var;
} file_access;

int byte_conversion(char *size) 
{
    int new_size = 0;

    if( size[strlen(size)-1] == 'k')
    {
        new_size = atoi(size) * 1000;
    }
    else if(size[strlen(size)-1] == 'm')
    {
        new_size =  atoi(size) * 1000000;
    }
    else if(size[strlen(size)-1] == 'g')
    {
        new_size = atoi(size) * 1000000000;
    }
    return new_size;
}

char *create_file(int file_size, int thread_id)
{   
    // Create files from D1 to D8
    FILE *fp;
    char *arr = (char *)malloc(file_size*sizeof(char));
    for(int i = 0;i<file_size;i++)
        arr[i] = '0';
    char *filename = malloc (sizeof (char) * 20);
    sprintf(filename, "files_%d.txt", thread_id);
    fp = fopen(filename, "w+");
    fputs(arr, fp);
    return filename;
}

//sequential write funciton
void write_seq(char *file_name, int file_size_in_bytes, int record_size_in_bytes)
{
    FILE *fd;
    int result;
    char *buffer, *data;
    long int temp_size = 0;
    fd = fopen(file_name, "w");
    if(fd == NULL)
    {
        printf("Error Opening the file!\n");
        exit(-1);
    }

    buffer = (char *)malloc(record_size_in_bytes * sizeof(char));
    data = buffer;
    //Generate string of record size 
    for ( long int i=0; i < record_size_in_bytes; i++)
    {
        *data = 'z';
        data++;
    }
    while( temp_size <= file_size_in_bytes )
    {
        result = fwrite(buffer, record_size_in_bytes * sizeof(char), 1, fd);
        if ( result == -1)
        {
            printf("Error : Cannot write data to file");
        }
        temp_size =  temp_size +  record_size_in_bytes;
    }
    fclose(fd);
    fflush(fd);
}

//sequential read funciton
void read_seq(char *file_name, int file_size_in_bytes, int record_size_in_bytes)
{
    FILE *fd;
    int result;
    char *buffer, *data;
    long int temp_size = 0;
    fd = fopen(file_name, "r");
    if(fd == NULL)
    {
        printf("Error Opening the file!\n");
        exit(-1);
    }
    buffer = (char *)malloc(record_size_in_bytes * sizeof(char));
    data = buffer;
    //Generate string of record size 
    for ( long int i=0; i < record_size_in_bytes; i++)
    {
        *data = 'z';
        data++;
    }
    
    while( temp_size <= file_size_in_bytes )
    {
        result = fread(buffer, record_size_in_bytes * sizeof(char), 1, fd);
        if ( result == -1)
        {
            printf("Error : Cannot read data from file");
        }
        temp_size =  temp_size +  record_size_in_bytes;
    }
    fclose(fd);
    fflush(fd);
}

//random write funciton
void write_ran(char *file_name, int file_size_in_bytes, int record_size_in_bytes)
{
    FILE *fd;
    int result;
    char *buffer, *data;
    long int temp_size = 0;
    fd = fopen(file_name, "w");
    if(fd == NULL)
    {
        printf("Error Opening the file!\n");
        exit(-1);
    }

    buffer = (char *)malloc(record_size_in_bytes * sizeof(char));
    data = buffer;
    //Generate string of record size 
    for ( long int i=0; i < record_size_in_bytes; i++)
    {
        *data = 'a';
        data++;
    }
    while( temp_size <= file_size_in_bytes )
    {
        int count = rand() % file_size_in_bytes ;
        fseek(fd , count , 0);
        result = fwrite(buffer, record_size_in_bytes * sizeof(char), 1, fd);
        if ( result == -1)
        {
            printf("Error : Cannot write data to file");
        }
        temp_size =  temp_size +  record_size_in_bytes;
    }
    fclose(fd);
    fflush(fd);
}

void read_ran(char *file_name, int file_size_in_bytes, int record_size_in_bytes)
{
    FILE *fd;
    int result;
    char *buffer, *data;
    long int temp_size = 0;
    fd = fopen(file_name, "r");
    if(fd == NULL)
    {
        printf("Error Opening the file!\n");
        exit(-1);
    }
    buffer = (char *)malloc(record_size_in_bytes * sizeof(char));
    data = buffer;
    //Generate string of record size 
    for ( long int i=0; i < record_size_in_bytes; i++)
    {
        *data = 'z';
        data++;
    }
    
    while( temp_size <= file_size_in_bytes )
    {
        int count = rand() % file_size_in_bytes ;
        fseek(fd , count , 0);
        result = fread(buffer, record_size_in_bytes * sizeof(char), 1, fd);
        if ( result == -1)
        {
            printf("Error : Cannot read data from file");
        }
        temp_size =  temp_size +  record_size_in_bytes;
    }
    fclose(fd);
    fflush(fd);
}


//funciton to start a thread
void *start_thread(void *args)
{
    file_access *actual_args = args;

    char *file_name = create_file(*(actual_args->file_size_in_bytes_var), *(actual_args->index));

    if(strcmp(actual_args->file_access_pattern, "WS") == 0)
    {
        write_seq(file_name, *(actual_args->file_size_in_bytes_var), *(actual_args->record_size_in_bytes_var));
    }
    else if (strcmp(actual_args->file_access_pattern, "RS") == 0)
    {
        read_seq(file_name, *(actual_args->file_size_in_bytes_var), *(actual_args->record_size_in_bytes_var));
    }
    else if (strcmp(actual_args->file_access_pattern, "RR") == 0)
    {
        read_ran(file_name, *(actual_args->file_size_in_bytes_var), *(actual_args->record_size_in_bytes_var));
    }
    else 
    {
        write_ran(file_name, *(actual_args->file_size_in_bytes_var), *(actual_args->record_size_in_bytes_var));
    }
    pthread_exit(NULL);
    free(actual_args);
}


void benchmark(int num_threads, char *access_pattern, int file_size_in_bytes, int record_size_in_bytes)
{
    // TO-DO implement the file/storage benchmark
    int i;
    int result;
    pthread_t threads;

    for(i=0;i < num_threads ; i++)
    {
        file_access *args = malloc(sizeof *args);
        args->index = &i;
        args->file_access_pattern = access_pattern;
        args->file_no_threads = &num_threads;
        args->file_size_in_bytes_var = &file_size_in_bytes;
        args->record_size_in_bytes_var = &record_size_in_bytes;
        result = pthread_create(&threads, NULL, start_thread, args); 
        if(result != 0)
        {
            printf("Unable to create a thread %d\n", result);
            exit(-1);
        }
        pthread_join(threads,NULL);
    }

}

int main(int argc, char **argv)
{
    struct timeval start, end;
    double exec_time;
    double throughput;
    long double latency;

    // TO-DO update the arguments the program receives
    if (argc != 6) {
        printf("%s\n", MSG);
        return -1;
    }

    int num_threads = atoi(argv[1]);
    char *file_size =  argv[2];
    char *access_pattern = argv[3];
    char *record_size =  argv[4];
    char *measure = argv[5];

    int file_size_in_bytes =  byte_conversion(file_size) ; //function to convert file size in bytes
    int record_size_in_bytes = byte_conversion(record_size); //funciton to convert record size in bytes

    printf("Running fileio:\n");
    printf(" - num_threads: %d\n", num_threads);
    printf(" - file_size_in_bytes: %s\n", file_size);
    printf(" - record_size_in_bytes: %s\n", record_size);
    printf(" - access_pattern: %s\n", access_pattern);
    printf(" - measure: %s\n", measure);

    gettimeofday(&start, NULL);
    benchmark(num_threads, access_pattern, file_size_in_bytes, record_size_in_bytes); //funciton to calculate benchmarks
    gettimeofday(&end, NULL);
    exec_time = (double) (end.tv_sec - start.tv_sec) 
                + (double) (end.tv_usec - start.tv_usec) / 1000000;
    
    // TO-DO transform execution time in throughput
    printf("Execution time: %lf seconds\n", exec_time);

    throughput = (double)((file_size_in_bytes/ exec_time) / 10000000);
    if(strcmp(measure,"throughput") == 0)
        printf("Throughput : %lf MB/sec\n",throughput );
        
    latency = (long double)((throughput * 10000000)/record_size_in_bytes);
    if(strcmp(measure, "latency") == 0)
        printf("Latency : %Lf ops/sec\n", latency);
    return 0;
}
