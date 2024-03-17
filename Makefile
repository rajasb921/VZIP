vzip: serial_copy.c
	gcc serial_copy.c -lz -o vzip

test:
	rm -f video.vzip
	./vzip frames
	./check.sh

clean:
	rm -f vzip video.vzip

