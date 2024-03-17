vzip: serial_correct.c
	gcc serial_correct.c -lz -o vzip

test:
	rm -f video.vzip
	./vzip frames
	./check.sh

clean:
	rm -f vzip video.vzip

