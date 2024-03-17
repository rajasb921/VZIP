#ifndef HELPER_COPY_H
#define HELPER_COPY_H

#include <zlib.h>

#define BUFFER_SIZE 1048576 // 1MB
z_stream strm1;
z_stream strm2;

// Data for each file
typedef struct file_data {
  int nfiles;
  int file_num;
  int nbytes;
  int *nbytes_zipped;
  unsigned char *buffer_in;
  unsigned char *buffer_out;
} file_data;


file_data *file_data_init(int nfiles, int file_num, int nbytes) {
  file_data *fd = (file_data *)malloc(sizeof(file_data));
  fd->nfiles = nfiles;
  fd->file_num = file_num;
  fd->nbytes = nbytes;
  fd->nbytes_zipped = malloc(sizeof(int));
  fd->buffer_in = malloc(BUFFER_SIZE * sizeof(unsigned char));
  fd->buffer_out = malloc(BUFFER_SIZE * sizeof(unsigned char));
  return fd;
}

void *PrintFiles(file_data **all_files) {
  for (int i=0; i<all_files[0]->nfiles; i++) {
    printf("--------------------------------\n");
    printf("File %d\n", all_files[i]->file_num);
    printf("nbytes: %d\n", all_files[i]->nbytes);
    printf("nbytes_zipped: %d\n", all_files[i]->nbytes_zipped[0]);
    printf("buffer_in: %p\n", all_files[i]->buffer_in);
    printf("buffer_out: %p\n", all_files[i]->buffer_out);
    printf("--------------------------------\n\n");
  }
}

void *ZipEven(void *arg) {
  file_data **all_files = (file_data **) arg;
  for (int i=0; i<all_files[0]->nfiles; i++) {
		int ret = deflateInit(&strm1, 9);
		assert(ret == Z_OK);
		strm1.avail_in = all_files[i]->nbytes;
		strm1.next_in = all_files[i]->buffer_in;
		strm1.avail_out = BUFFER_SIZE;
		strm1.next_out = all_files[i]->buffer_out;

		ret = deflate(&strm1, Z_FINISH);
		assert(ret == Z_STREAM_END);

    *(all_files[i]->nbytes_zipped) = BUFFER_SIZE-strm1.avail_out;
  }
  printf("First (Even) half done\n");
}

void *ZipOdd(void *arg) {
    file_data **all_files = (file_data **) arg;
    for (int i=1; i<all_files[0]->nfiles; i += 2) {
		int ret = deflateInit(&strm2, 9);
		assert(ret == Z_OK);
		strm2.avail_in = all_files[i]->nbytes;
		strm2.next_in = all_files[i]->buffer_in;
		strm2.avail_out = BUFFER_SIZE;
		strm2.next_out = all_files[i]->buffer_out;

		ret = deflate(&strm2, Z_FINISH);
		assert(ret == Z_STREAM_END);

    *(all_files[i]->nbytes_zipped) = BUFFER_SIZE-strm2.avail_out;
  }
  printf("Second (Odd) half done\n");
}

#endif