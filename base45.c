#include <ctype.h> 
#include <strings.h>
#include <stdio.h>


static const char BASE45_CHARSET[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ $%*+-./:";

int base45_encode(char * dst, size_t *_max_dst_len, const unsigned char * src, size_t src_len) {
  size_t out_len = 0, max_dst_len;
  max_dst_len = _max_dst_len ? *_max_dst_len : src_len * 4;

  for(int i = 0; i < src_len; i+=2) {
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

static inline int c2i(char c) {
	char * p = index(BASE45_CHARSET,toupper(c));
	return p ? p - BASE45_CHARSET : 255;
}
	
int base45_decode(unsigned char * dst, size_t * _max_dst_len, const char * src, size_t src_len) {
  size_t out_len = 0, max_dst_len;
  max_dst_len = _max_dst_len  ? *_max_dst_len : src_len;

  if (src_len == 0)
	src_len = strlen(src);

  for(int i = 0; i < src_len; i+=3) {
     int x,a,b;

     if (src_len - i < 2) 
	return -1;

     if ((255 == (a = c2i(src[i]))) || (255 == (b = c2i(src[i+1])))) 
	return -1;

     x = a + 45 * b;

     if (src_len - i >= 3) {
        if (255 == (a = c2i(src[i+2]))) 
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
