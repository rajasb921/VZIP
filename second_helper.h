#ifndef HELPER_H
#define HELPER_H

#include <pthread.h>

#define BUFFER_SIZE 1048576 // 1MB
#define ON 1
#define OFF 0

int flag1 = OFF;
int flag2 = ON;

pthread_mutex_t load_lock = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t dump_lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t load_done = PTHREAD_COND_INITIALIZER;
pthread_cond_t dump_done = PTHREAD_COND_INITIALIZER;
pthread_cond_t start_load = PTHREAD_COND_INITIALIZER;
pthread_cond_t start_dump = PTHREAD_COND_INITIALIZER;

typedef struct thread_data {
  int nfiles;
  char **argv;
  FILE *f_out;
  char **files;
  char *full_path;
  unsigned char *buffer_in;
  unsigned char *buffer_out;
  int *total_in;
  int *total_out;
  int *nbytes;
  z_stream *strm;
} thread_data;

thread_data *thread_data_init_files(int nfiles, char **argv, char **files, FILE *f_out, unsigned char *buffer_in, unsigned char *buffer_out, int *total_in, int *total_out, int *nbytes, z_stream *strm){
  thread_data *data = (thread_data *)malloc(sizeof(thread_data));
  data->nfiles = nfiles;
  data->argv = argv;
  data->files = files;
  data->f_out = f_out;
  data->buffer_in = buffer_in;
  data->buffer_out = buffer_out;
  data->total_in = total_in;
  data->total_out = total_out;
  data->nbytes = nbytes;
  data->strm = strm;
  return data;
}

void *Load(void *arg){
  thread_data *data = (thread_data *)arg;
  int i;
  for(i = 0; i < data->nfiles; i++){
    pthread_mutex_lock(&load_lock);
    while (flag1 == ON){
      pthread_cond_wait(&start_load, &load_lock);
    }
    
    int len = strlen(data->argv[1])+strlen(data->files[i])+2;
    char *full_path = malloc(len*sizeof(char));
    assert(full_path != NULL);
    strcpy(full_path, data->argv[1]);
    strcat(full_path, "/");
    strcat(full_path, data->files[i]);
    data->full_path = full_path;
    FILE *f_in = fopen(full_path, "r");
    assert(f_in != NULL);
    *(data->nbytes) = fread(data->buffer_in, sizeof(unsigned char), BUFFER_SIZE, f_in);
    fclose(f_in);
    *(data->total_in) += *(data->nbytes);
    flag1 = ON;
    pthread_cond_signal(&load_done);
    pthread_mutex_unlock(&load_lock);
  }
}

void *Zip(void *arg){
  thread_data *data = (thread_data *)arg;
  int i;
  for (i = 0; i < data->nfiles; i++){
    pthread_mutex_lock(&load_lock);
    while (flag1 == OFF){
      pthread_cond_wait(&load_done, &load_lock);
    }
    pthread_mutex_lock(&dump_lock);
    while (flag2 == OFF){
      pthread_cond_wait(&dump_done, &dump_lock);
    }
    
    int ret = deflateInit(data->strm, 9);
    assert(ret == Z_OK);
    (*(data->strm)).avail_in = *(data->nbytes);       // Not sure why data->strm->avail_in = *(data->nbytes) doesn't work
    (*(data->strm)).next_in = data->buffer_in;
    (*(data->strm)).avail_out = BUFFER_SIZE;
    (*(data->strm)).next_out = data->buffer_out;
    ret = deflate(data->strm, Z_FINISH);
    assert(ret == Z_STREAM_END);

    flag1 = OFF;
    flag2 = OFF;
    pthread_cond_signal(&start_load);
    pthread_cond_signal(&start_dump);
    pthread_mutex_unlock(&load_lock);
    pthread_mutex_unlock(&dump_lock);
  }
}

void *Dump(void *arg){
  thread_data *data = (thread_data *)arg;
  int i;
  for (i = 0; i < data->nfiles; i++){
    pthread_mutex_lock(&dump_lock);
    while (flag2 == ON){
      pthread_cond_wait(&start_dump, &dump_lock);
    }

    int nbytes_zipped = BUFFER_SIZE-(*(data->strm)).avail_out;
    fwrite(&nbytes_zipped, sizeof(int), 1, data->f_out);
    fwrite(data->buffer_out, sizeof(unsigned char), nbytes_zipped, data->f_out);
    *(data->total_out) += nbytes_zipped;
    //free(data->full_path);
    flag2 = ON;
    pthread_cond_signal(&dump_done);
    pthread_mutex_unlock(&dump_lock);
    }
}

#endif
