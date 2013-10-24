// entropypic.c
// Glen Wiley <glen.wiley@gmail.com>
//
// generate a BMP from an entropy stream to help catch non-random
// patterns in the entropy

#include "bmpfile.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define READBUFSIZE 128
#define MODE_BITS   'b'
#define MODE_BYTES  'B'

//---------------------------------------- usage
void
usage(void)
{
	printf("USAGE: entropypic [-B] -s <x,y> [-f <file>] -o <file>\n"
	 "\n"
	 "-B         input file is bytes\n"
	 "-s <x,y>   set image size in pixels\n"
	 "-f <file>  input file, default is stdin\n"
	 "-o <file>  output file\n"
	 "\n"
	 "Input file should be a stream of 1s and 0s, a 1 will show as black\n"
	 "on the output image, if the input file is s stream of bytes (-B) then\n"
	 "each byte is converted to bits.\n"
	 "\n"
	 "The file is read until EOF or until the bitmap is filled.\n"
	 "\n"
	 "Glen Wiley <glen.wiley@gmail.com>\n"
	);
} // usage

//---------------------------------------- createbmp
// generate the bmp
void
createbmp(FILE *fh_in, char *fn_out, int xsize, int ysize, char mode)
{
	int         bmpfull = 0;
	int         nbytes;
	int         bnum;
	int         i;
	int         x = 0;
	int         y = 0;
	int         bitval;
	char        buf[READBUFSIZE+1];
	bmpfile_t   *bmp;
	rgb_pixel_t pixel = {128, 64, 0, 0};

	bmp = bmp_create(xsize, ysize, 1);

	while((nbytes = fread(buf, 1, READBUFSIZE, fh_in)) > 0 && bmpfull == 0)
	{
		for(bnum=0; bnum < nbytes; bnum++)
		{
			// if we are in bit mode then ignore anything other than 0 and 1

			if(mode == MODE_BITS && (buf[bnum] != '0' && buf[bnum] != '1'))
				continue;

			for(i=0; i<(mode==MODE_BITS ? 1 : 8); i++)
			{
				if(mode == MODE_BITS)
				{
					if(buf[bnum] == '1')
						bitval == 1;
				}
				else
				{
					bitval = buf[bnum] & 1;
					buf[bnum] = buf[bnum] >> 1;
				}

				if(bitval == 1)
					bmp_set_pixel(bmp, x, y, pixel);

				x++;
				if(x == xsize)
				{
					x = 0;
					y++;
				}
				if(y == ysize)
				{
					bmpfull = 1;
					break;
				}
			} // for i
		} // for bnum
	} // while fread

	bmp_save(bmp, fn_out);
	bmp_destroy(bmp);

	return;
} // createbmp

//---------------------------------------- main
int
main(int argc, char *argv[])
{
	int opt;
	int retval   = 0;
	int x        = 0;
	int y        = 0;
	FILE *fh_in  = NULL;
	char *fn_in  = NULL;
	char *fn_out = NULL;
	char *tok;
	char mode    = MODE_BITS;

	if(argc == 1)
	{
		usage();
		exit(1);
	}

	while((opt=getopt(argc, argv, "Bf:o:s:")) != -1)
	{
		switch(opt)
		{
			case 'B':
				mode = MODE_BYTES;
				break;

			case 'f':
				fn_in = optarg;
				break;

			case 'o':
				fn_out = optarg;
				break;

			case 's':
				tok = strtok(optarg, ",xX");
				if(tok != NULL && *tok != '\0')
				{
					x = atoi(tok);
					if((tok = strtok(NULL, ",xX")) != NULL && *tok != '\0')
						y = atoi(tok);
				}
				break;

			default:
				usage();
				exit(1);
		}
	} // while getopt

	if(x == 0 || y == 0)
		printf("ERROR: no size specified\n");

	if(fn_in != NULL)
	{
		fh_in = fopen(fn_in, "r");
		if(fh_in == NULL)
		{
			printf("ERROR: failed to open input file %s\n", fn_in);
			exit(1);
		}
	}
	else
		fh_in = stdin;

	if(fn_out == NULL)
	{
		printf("ERROR: no output file specified, exiting.\n");
		exit(1);
	}

	createbmp(fh_in, fn_out, x, y, mode);
	fclose(fh_in);

	return retval;
} // main

// entropypic.c
