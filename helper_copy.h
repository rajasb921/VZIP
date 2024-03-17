#ifndef HELPER_COPY_H
#define HELPER_COPY_H

#include <zlib.h>

#define BUFFER_SIZE 1048576 // 1MB

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

void *Zip (file_data **all_files) {
  int nfiles = all_files[0]->nfiles;
  for (int i=0 ; i<nfiles; i++) {
    printf("Zipping file %d\n", all_files[i]->file_num);
    /*
    z_stream strm;
		int ret = deflateInit(&strm, 9);
		assert(ret == Z_OK);
		strm.avail_in = all_files[i]->nbytes;
		strm.next_in = all_files[i]->buffer_in;
		strm.avail_out = BUFFER_SIZE;
		strm.next_out = all_files[i]->buffer_out;

		ret = deflate(&strm, Z_FINISH);
		assert(ret == Z_STREAM_END);

    *(all_files[i]->nbytes_zipped) = BUFFER_SIZE-strm.avail_out;
    */
  }
}

#endif