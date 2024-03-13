#ifndef HELPER_H
#define HELPER_H

#define BUFFER_SIZE 1048576 // 1MB

typedef struct thread_data {
  FILE *f_out;
  char *full_path;
  unsigned char *buffer_in;
  unsigned char *buffer_out;
  int *total_in;
  int *total_out;
  int *nbytes;
  z_stream *strm;
} thread_data;


thread_data *thread_data_init(FILE *f_out, char *fullpath, unsigned char *buffer_in, unsigned char *buffer_out, int *total_in, int *total_out, int *nbytes, z_stream *strm){
  thread_data *data = (thread_data *)malloc(sizeof(thread_data));
  data->f_out = f_out;
  data->full_path = fullpath;
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
  FILE *f_in = fopen(data->full_path, "r");
	assert(f_in != NULL);
	*(data->nbytes) = fread(data->buffer_in, sizeof(unsigned char), BUFFER_SIZE, f_in);
	fclose(f_in);
	*(data->total_in) += *(data->nbytes);
}

void *Zip(void *arg){
  thread_data *data = (thread_data *)arg;
  int ret = deflateInit(data->strm, 9);
  assert(ret == Z_OK);
  (*(data->strm)).avail_in = *(data->nbytes);       // Not sure why data->strm->avail_in = *(data->nbytes) doesn't work
  (*(data->strm)).next_in = data->buffer_in;
  (*(data->strm)).avail_out = BUFFER_SIZE;
  (*(data->strm)).next_out = data->buffer_out;

  ret = deflate(data->strm, Z_FINISH);
  assert(ret == Z_STREAM_END);
}

void *Dump(void *arg){
  thread_data *data = (thread_data *)arg;
  int nbytes_zipped = BUFFER_SIZE-(*(data->strm)).avail_out;
  fwrite(&nbytes_zipped, sizeof(int), 1, data->f_out);
  fwrite(data->buffer_out, sizeof(unsigned char), nbytes_zipped, data->f_out);
  *(data->total_out) += nbytes_zipped;
  free(data->full_path);
}

#endif
