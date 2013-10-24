#entropypic

entropypic generates an image from an entropy stream to help identify patterns in entropy.  Statistical analysis of entropy streams is done well by other tool sets (I like to see the results of running 'ent' against it) providing a simple way to visually inspect an entropy stream.

This has been built on Mac OSX using gcc and will probably build on other platforms.


##Build and Installing

Images are generated as BPM files using libbmp from version 0.1.3:

http://code.google.com/p/libbmp/

You will need this library built and installed in order to build entropypic.  The good news is that there are no further dependencies for either this library.

To build simply type 'make', if it fails to build drop me a note and I'll see if I can fix whatever is broken.  To install simply copy the binary that results from the make to your favorite 'bin' directory.

##Running

Here is the output from running with no arguments:

```
USAGE: entropypic [-B] -s <x,y> [-f <file>] -o <file>

-B         input file is bytes
-s <x,y>   set image size in pixels
-f <file>  input file, default is stdin
-o <file>  output file

Input file should be a stream of 1s and 0s, a 1 will show as black
on the output image, if the input file is s stream of bytes (-B) then
each byte is converted to bits.

The file is read until EOF or until the bitmap is filled.

Glen Wiley <glen.wiley@gmail.com>
```
