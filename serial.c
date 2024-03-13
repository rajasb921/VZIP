#include <dirent.h> 
#include <stdio.h> 
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <zlib.h>
#include <time.h>
#include "helper.h"
#include <pthread.h>

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

/* Start of Sort function */

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

/* End of Sort function */
	
  // create a single zipped package with all PPM files in lexicographical order
	int total_in = 0, total_out = 0, flagLZ = 0, flagZD = 1, nbytes = 0;
  unsigned char buffer_in[BUFFER_SIZE];
	unsigned char buffer_out[BUFFER_SIZE];
  z_stream strm;
	FILE *f_out = fopen("video.vzip", "w");
	assert(f_out != NULL);
	for(int i=0; i < nfiles; i++) {

    /* Start of Load function */
		int len = strlen(argv[1])+strlen(files[i])+2;
		char *full_path = malloc(len*sizeof(char));
		assert(full_path != NULL);
		strcpy(full_path, argv[1]);
		strcat(full_path, "/");
		strcat(full_path, files[i]);
    thread_data *data = thread_data_init(f_out, full_path, buffer_in, buffer_out, &total_in, &total_out, &nbytes, &strm);
		Load((void *)data);
    /* End of Load function */

    /* Start of Zip function */
		Zip((void *)data);
    /* End of Zip function */

		
    /* Start of Dump function */
    // dump zipped file
		Dump((void *)data);
    /* End of Dump function */

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
