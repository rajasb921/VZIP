#include <dirent.h> 
#include <stdio.h> 
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <zlib.h>
#include <time.h>
#include "helper.h"
#include <pthread.h>

#define BUFFER_SIZE 1048576 // 1MB


pthread_t thread1;
pthread_t thread2;
pthread_t thread3;
pthread_t thread4;
pthread_t thread5;
pthread_t thread6;
pthread_t thread7;
pthread_t thread8;
pthread_t thread9;
pthread_t thread10;

int cmp(const void *a, const void *b) {
	return strcmp(*(char **) a, *(char **) b);
}

int main(int argc, char **argv) {
	// time computation header
	struct timespec start, end;
	clock_gettime(CLOCK_MONOTONIC, &start);
	// end of time computation header

	// do not modify the main function before this point!

	assert(argc == 2);

	DIR *d;
	struct dirent *dir;
	char **files = NULL;
	int nfiles = 0;

	d = opendir(argv[1]);
	if(d == NULL) {
		printf("An error has occurred\n");
		return 0;
	}

	// create sorted list of PPM files
	while ((dir = readdir(d)) != NULL) {
		files = realloc(files, (nfiles+1)*sizeof(char *));
		assert(files != NULL);

		int len = strlen(dir->d_name);
		if(dir->d_name[len-4] == '.' && dir->d_name[len-3] == 'p' && dir->d_name[len-2] == 'p' && dir->d_name[len-1] == 'm') {
			files[nfiles] = strdup(dir->d_name);
			assert(files[nfiles] != NULL);

			nfiles++;
		}
	}
	closedir(d);
	qsort(files, nfiles, sizeof(char *), cmp);


	// create a single zipped package with all PPM files in lexicographical order
	int total_in = 0, total_out = 0;
	FILE *f_out = fopen("video.vzip", "w");
	assert(f_out != NULL);

  // Create an array of file pointers
  file_data *all_files[nfiles];
  
  // Load
  for (int i=0; i<nfiles; i++) {
    // Init file
    all_files[i] = file_data_init(nfiles, i, 0);

    int len = strlen(argv[1])+strlen(files[i])+2;
		char *full_path = malloc(len*sizeof(char));
		assert(full_path != NULL);
		strcpy(full_path, argv[1]);
		strcat(full_path, "/");
		strcat(full_path, files[i]);

    // load file
		FILE *f_in = fopen(full_path, "r");
		assert(f_in != NULL);
		all_files[i]->nbytes = fread(all_files[i]->buffer_in, sizeof(unsigned char), BUFFER_SIZE, f_in);
		fclose(f_in);
		total_in += all_files[i]->nbytes;

    free(full_path);
  }

  // Zip                                                                      /* This ZIP takes the most time. Use threads here */
  pthread_create(&thread1, NULL, Zip1, (void *) all_files);
  pthread_create(&thread2, NULL, Zip2, (void *) all_files);
  pthread_create(&thread3, NULL, Zip3, (void *) all_files);
  pthread_create(&thread4, NULL, Zip4, (void *) all_files);
  pthread_create(&thread5, NULL, Zip5, (void *) all_files);
  pthread_create(&thread6, NULL, Zip6, (void *) all_files);
  pthread_create(&thread7, NULL, Zip7, (void *) all_files);
  pthread_create(&thread8, NULL, Zip8, (void *) all_files);
  pthread_create(&thread9, NULL, Zip9, (void *) all_files);
  pthread_create(&thread10, NULL, Zip10, (void *) all_files);

  pthread_join(thread1, NULL);
  pthread_join(thread2, NULL);
  pthread_join(thread3, NULL);
  pthread_join(thread4, NULL);
  pthread_join(thread5, NULL);
  pthread_join(thread6, NULL);
  pthread_join(thread7, NULL);
  pthread_join(thread8, NULL);
  pthread_join(thread9, NULL);
  pthread_join(thread10, NULL);

  // Dump
  for (int i=0; i<nfiles; i++) {
		fwrite(all_files[i]->nbytes_zipped, sizeof(int), 1, f_out);
		fwrite(all_files[i]->buffer_out, sizeof(unsigned char), *(all_files[i]->nbytes_zipped), f_out);
		total_out += *(all_files[i]->nbytes_zipped);
	}

	fclose(f_out);

	printf("Compression rate: %.2lf%%\n", 100.0*(total_in-total_out)/total_in);

	// release list of files
	for(int i=0; i < nfiles; i++)
		free(files[i]);
	free(files);

	// do not modify the main function after this point!

	// time computation footer
	clock_gettime(CLOCK_MONOTONIC, &end);
	printf("Time: %.2f seconds\n", ((double)end.tv_sec+1.0e-9*end.tv_nsec)-((double)start.tv_sec+1.0e-9*start.tv_nsec));
	// end of time computation footer

	return 0;
}
