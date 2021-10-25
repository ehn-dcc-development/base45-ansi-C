#include <ctype.h> 	// for size_t
#include <string.h> 	// for strlen()

/*                  Table 1: The Base 45 Alphabet

   Value Encoding  Value Encoding  Value Encoding  Value Encoding
      00 0            12 C            24 O            36 Space
      01 1            13 D            25 P            37 $
      02 2            14 E            26 Q            38 %
      03 3            15 F            27 R            39 *
      04 4            16 G            28 S            40 +
      05 5            17 H            29 T            41 -
      06 6            18 I            30 U            42 .
      07 7            19 J            31 V            43 /
      08 8            20 K            32 W            44 :
      09 9            21 L            33 X
      10 A            22 M            34 Y
      11 B            23 N            35 Z

*/

static const char BASE45_CHARSET[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ $%*+-./:";

static unsigned char _C2I[256] = {
	255,255,255,255, 255,255,255,255, 255,255,255,255, 255,255,255,255,
	255,255,255,255, 255,255,255,255, 255,255,255,255, 255,255,255,255,
	36, 255,255,255,  37, 38,255,255, 255,255, 39, 40, 255, 41, 42, 43,
         0,   1,  2,  3,   4,  5,  6,  7,   8,  9, 44,255, 255,255,255,255, 

        255, 10, 11, 12,  13, 14, 15, 16,  17, 18, 19, 20,  21, 22, 23, 24, /* uppercase */
         25, 26, 27, 28,  29, 30, 31, 32,  33, 34, 35, 35, 255,255,255,255,
        255, 10, 11, 12,  13, 14, 15, 16,  17, 18, 19, 20,  21, 22, 23, 24, /* lowercase */
         25, 26, 27, 28,  29, 30, 31, 32,  33, 34, 35, 35, 255,255,255,255,

	255,255,255,255, 255,255,255,255, 255,255,255,255, 255,255,255,255, 
	255,255,255,255, 255,255,255,255, 255,255,255,255, 255,255,255,255,
	255,255,255,255, 255,255,255,255, 255,255,255,255, 255,255,255,255,
	255,255,255,255, 255,255,255,255, 255,255,255,255, 255,255,255,255,

	255,255,255,255, 255,255,255,255, 255,255,255,255, 255,255,255,255,
	255,255,255,255, 255,255,255,255, 255,255,255,255, 255,255,255,255,
	255,255,255,255, 255,255,255,255, 255,255,255,255, 255,255,255,255,
	255,255,255,255, 255,255,255,255, 255,255,255,255, 255,255,255,255,
};

int base45_encode(char * dst, size_t *_max_dst_len, const unsigned char * src, size_t src_len) {
  size_t out_len = 0, max_dst_len;
  max_dst_len = _max_dst_len ? *_max_dst_len : src_len * 4;

  for(unsigned i = 0; i < src_len; i+=2) {
     if (src_len - i > 1) {
        int x = ((src[i])<<8) + src[i+1];

        unsigned char e = x / (45 * 45);
        x %= 45 * 45;
        unsigned char d = x / 45;
        unsigned char c = x % 45;

	if (out_len < max_dst_len && dst)
		dst[ out_len ] = BASE45_CHARSET[c];
	out_len++;
	if (out_len < max_dst_len && dst)
		dst[ out_len ] = BASE45_CHARSET[d];
	out_len++;
	if (out_len < max_dst_len && dst)
		dst[ out_len ] = BASE45_CHARSET[e];
	out_len++;
     } else {
        int x = src[i];
        unsigned char d = x / 45;
        unsigned char c = x % 45;

	if (out_len < max_dst_len && dst)
		dst[ out_len ] = BASE45_CHARSET[c];
	out_len++;

	if (out_len < max_dst_len && dst)
		dst[ out_len ] = BASE45_CHARSET[d];
	out_len++;
     }
  }
  /* Same non guarantee as strncpy et.al. */
  if (out_len < max_dst_len && dst)
	dst[ out_len ] = 0;

  if (_max_dst_len)
	*_max_dst_len = out_len;
  return 0;
}

int base45_decode(unsigned char * dst, size_t * _max_dst_len, const char *src, size_t src_len) {
  size_t out_len = 0, max_dst_len;
  max_dst_len = _max_dst_len  ? *_max_dst_len : src_len;

  if (dst == NULL && _max_dst_len == NULL)
	return -2;

  if (src == NULL)
	return -2;

  if (src_len == 0)
	src_len = strlen(src);

  for(unsigned i = 0; i < src_len; i+=3) {
     int x,a,b;

     if (src_len - i < 2) 
	    return -1;

     if ((255 == (a = _C2I[(unsigned char)src[i]])) || (255 == (b = _C2I[(unsigned char)src[i+1]])))
	return -1;

     x = a + 45 * b;

     if (src_len - i >= 3) {
        if (255 == (a = _C2I[(unsigned char)src[i+2]]))
	    return -1;

        x += a * 45 * 45;

        if (out_len < max_dst_len && dst)
        	dst[out_len] = x / 256;
        out_len++;
	x %= 256;
    };

    if (out_len < max_dst_len && dst)
        dst[out_len] = x;

    out_len++;
  };
  if (_max_dst_len)
	*_max_dst_len = out_len;

  return 0;
}

#ifdef BASE45_UTIL
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char ** argv) {
	FILE * in = stdin;
	FILE * out = stdout;
	int decode = 0;
	int at  = 1;

	if (argc > 1 && argv[at][0] == '-') {
		if (argv[at][1] == 'd')
			decode = 1;
		else {
			fprintf(stderr,"Syntax: %s [-d] [infile [outfile]]\n", argv[0]);
			exit(1);
		};
		at++; argc--;
	};
	if (argc > 1) {
		if (NULL == (in = fopen(argv[at],"r"))) {
			perror("Cannot open input file for reading:");
			exit(1);
		};
		at++; argc--;
	};
	if (argc > 1) {
		if (NULL == (out = fopen(argv[at],"w"))) {
			perror("Cannot open out file for writing:");
			exit(1);
		};
		at++; argc--;
	};

#ifdef VALIDATE
	for(int i = 0; i < 45; i++) assert(i ==  _C2I[BASE45_CHARSET[i]]); 
#endif

        while(!feof(in)) {
        	unsigned char buff[       3 * 1024 ]; // multiple chosen to allow continuation.
        	unsigned char outbuf[ 3 * 3 * 1024 ];
		size_t olen = sizeof(outbuf);
		size_t len = fread(buff, 1, 3 * 1024, in);

		buff[len] = 0;

		if (len) {
			int e;
			if (decode) 
				e = base45_decode(outbuf, &olen, (char *) buff, len);
			else
				e = base45_encode((char *)outbuf, &olen, buff, len);

			if (e) {
				fprintf(stderr,"base45 %s failed\n", decode ? "decode" : "encode");
				exit(1);
			};

			if (olen)
				fwrite(outbuf, 1, olen, out);
		};
	}
	fclose(out);

	return(0);
};
#endif

		
