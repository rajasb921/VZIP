#ifndef HELPER_H
#define HELPER_H

#include <zlib.h>

#define BUFFER_SIZE 1048576 // 1MB
z_stream strm1;
z_stream strm2;
z_stream strm3;
z_stream strm4;
z_stream strm5;
z_stream strm6;
z_stream strm7;
z_stream strm8;
z_stream strm9;
z_stream strm10;

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

void *Zip1(void *arg) {
  file_data **all_files = (file_data **) arg;
  int step = all_files[0]->nfiles / 10;
  for (int i = 0; i < step; i++) {
    int ret = deflateInit(&strm1, 9);
    assert(ret == Z_OK);
    strm1.avail_in = all_files[i]->nbytes;
    strm1.next_in = all_files[i]->buffer_in;
    strm1.avail_out = BUFFER_SIZE;
    strm1.next_out = all_files[i]->buffer_out;

    ret = deflate(&strm1, Z_FINISH);
    assert(ret == Z_STREAM_END);

    *(all_files[i]->nbytes_zipped) = BUFFER_SIZE - strm1.avail_out;
  }
}

void *Zip2(void *arg) {
  file_data **all_files = (file_data **) arg;
  int step = all_files[0]->nfiles / 10;
  for (int i = 1 * step; i < 2 * step; i++) {
    int ret = deflateInit(&strm2, 9);
    assert(ret == Z_OK);
    strm2.avail_in = all_files[i]->nbytes;
    strm2.next_in = all_files[i]->buffer_in;
    strm2.avail_out = BUFFER_SIZE;
    strm2.next_out = all_files[i]->buffer_out;

    ret = deflate(&strm2, Z_FINISH);
    assert(ret == Z_STREAM_END);

    *(all_files[i]->nbytes_zipped) = BUFFER_SIZE - strm2.avail_out;
  }
}

void *Zip3(void *arg) {
  file_data **all_files = (file_data **) arg;
  int step = all_files[0]->nfiles / 10;
  for (int i = 2 * step; i < 3 * step; i++) {
    int ret = deflateInit(&strm3, 9);
    assert(ret == Z_OK);
    strm3.avail_in = all_files[i]->nbytes;
    strm3.next_in = all_files[i]->buffer_in;
    strm3.avail_out = BUFFER_SIZE;
    strm3.next_out = all_files[i]->buffer_out;

    ret = deflate(&strm3, Z_FINISH);
    assert(ret == Z_STREAM_END);

    *(all_files[i]->nbytes_zipped) = BUFFER_SIZE - strm3.avail_out;
  }
}

void *Zip4(void *arg) {
  file_data **all_files = (file_data **) arg;
  int step = all_files[0]->nfiles / 10;
  for (int i = 3 * step; i < 4 * step; i++) {
    int ret = deflateInit(&strm4, 9);
    assert(ret == Z_OK);
    strm4.avail_in = all_files[i]->nbytes;
    strm4.next_in = all_files[i]->buffer_in;
    strm4.avail_out = BUFFER_SIZE;
    strm4.next_out = all_files[i]->buffer_out;

    ret = deflate(&strm4, Z_FINISH);
    assert(ret == Z_STREAM_END);

    *(all_files[i]->nbytes_zipped) = BUFFER_SIZE - strm4.avail_out;
  }
}

void *Zip5(void *arg) {
  file_data **all_files = (file_data **) arg;
  int step = all_files[0]->nfiles / 10;
  for (int i = 4 * step; i < 5 * step; i++) {
    int ret = deflateInit(&strm5, 9);
    assert(ret == Z_OK);
    strm5.avail_in = all_files[i]->nbytes;
    strm5.next_in = all_files[i]->buffer_in;
    strm5.avail_out = BUFFER_SIZE;
    strm5.next_out = all_files[i]->buffer_out;

    ret = deflate(&strm5, Z_FINISH);
    assert(ret == Z_STREAM_END);

    *(all_files[i]->nbytes_zipped) = BUFFER_SIZE - strm5.avail_out;
  }
}

void *Zip6(void *arg) {
  file_data **all_files = (file_data **) arg;
  int step = all_files[0]->nfiles / 10;
  for (int i = 5 * step; i < 6 * step; i++) {
    int ret = deflateInit(&strm6, 9);
    assert(ret == Z_OK);
    strm6.avail_in = all_files[i]->nbytes;
    strm6.next_in = all_files[i]->buffer_in;
    strm6.avail_out = BUFFER_SIZE;
    strm6.next_out = all_files[i]->buffer_out;

    ret = deflate(&strm6, Z_FINISH);
    assert(ret == Z_STREAM_END);

    *(all_files[i]->nbytes_zipped) = BUFFER_SIZE - strm6.avail_out;
  }
}

void *Zip7(void *arg) {
  file_data **all_files = (file_data **) arg;
  int step = all_files[0]->nfiles / 10;
  for (int i = 6 * step; i < 7 * step; i++) {
    int ret = deflateInit(&strm7, 9);
    assert(ret == Z_OK);
    strm7.avail_in = all_files[i]->nbytes;
    strm7.next_in = all_files[i]->buffer_in;
    strm7.avail_out = BUFFER_SIZE;
    strm7.next_out = all_files[i]->buffer_out;

    ret = deflate(&strm7, Z_FINISH);
    assert(ret == Z_STREAM_END);

    *(all_files[i]->nbytes_zipped) = BUFFER_SIZE - strm7.avail_out;
  }
}

void *Zip8(void *arg) {
  file_data **all_files = (file_data **) arg;
  int step = all_files[0]->nfiles / 10;
  for (int i = 7 * step; i < 8 * step; i++) {
    int ret = deflateInit(&strm8, 9);
    assert(ret == Z_OK);
    strm8.avail_in = all_files[i]->nbytes;
    strm8.next_in = all_files[i]->buffer_in;
    strm8.avail_out = BUFFER_SIZE;
    strm8.next_out = all_files[i]->buffer_out;

    ret = deflate(&strm8, Z_FINISH);
    assert(ret == Z_STREAM_END);

    *(all_files[i]->nbytes_zipped) = BUFFER_SIZE - strm8.avail_out;
  }
}

void *Zip9(void *arg) {
  file_data **all_files = (file_data **) arg;
  int step = all_files[0]->nfiles / 10;
  for (int i = 8 * step; i < 9 * step; i++) {
    int ret = deflateInit(&strm9, 9);
    assert(ret == Z_OK);
    strm9.avail_in = all_files[i]->nbytes;
    strm9.next_in = all_files[i]->buffer_in;
    strm9.avail_out = BUFFER_SIZE;
    strm9.next_out = all_files[i]->buffer_out;

    ret = deflate(&strm9, Z_FINISH);
    assert(ret == Z_STREAM_END);

    *(all_files[i]->nbytes_zipped) = BUFFER_SIZE - strm9.avail_out;
  }
}

void *Zip10(void *arg) {
  file_data **all_files = (file_data **) arg;
  int step = all_files[0]->nfiles / 10;
  for (int i = 9 * step; i < all_files[0]->nfiles; i++) {
    int ret = deflateInit(&strm10, 9);
    assert(ret == Z_OK);
    strm10.avail_in = all_files[i]->nbytes;
    strm10.next_in = all_files[i]->buffer_in;
    strm10.avail_out = BUFFER_SIZE;
    strm10.next_out = all_files[i]->buffer_out;

    ret = deflate(&strm10, Z_FINISH);
    assert(ret == Z_STREAM_END);

    *(all_files[i]->nbytes_zipped) = BUFFER_SIZE - strm10.avail_out;
  }
}


#endif