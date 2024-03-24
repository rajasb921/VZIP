#include <dirent.h> 
#include <stdio.h> 
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <zlib.h>
#include <time.h>
#include "helper.h"

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
	
  pthread_t load_thread, zip_thread, dump_thread;
  // create a single zipped package with all PPM files in lexicographical order
	int total_in = 0, total_out = 0, nbytes = 0;
  unsigned char buffer_in[BUFFER_SIZE];
	unsigned char buffer_out[BUFFER_SIZE];
  z_stream strm;
	FILE *f_out = fopen("video.vzip", "w");
	assert(f_out != NULL);
  thread_data *data = thread_data_init_files(nfiles, argv, files, f_out, buffer_in, buffer_out, &total_in, &total_out, &nbytes, &strm);

    /* Start of Load function */
    int rc1 = pthread_create(&load_thread, NULL, Load, (void *)data);
    assert(rc1 == 0);
		//Load((void *)data); 
    /* End of Load function */

    /* Start of Zip function */
    int rc2 = pthread_create(&zip_thread, NULL, Zip, (void *)data);
    assert(rc2 == 0);
		//Zip((void *)data);
    /* End of Zip function */

		
    /* Start of Dump function */
    // dump zipped file
    int rc3 = pthread_create(&dump_thread, NULL, Dump, (void *)data);
    assert(rc3 == 0);
		//Dump((void *)data);
    /* End of Dump function */


  pthread_join(load_thread, NULL);
  pthread_join(zip_thread, NULL);
  pthread_join(dump_thread, NULL);
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
