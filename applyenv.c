
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <sys/mman.h>
#include <errno.h>
#include <math.h>
#include <string.h>

#include <point.h>

#include <pixel.h>
#include <image.h>

int main(int argc, char *argv[]) {

  image_t img;

  long int num_pixels;

  size_t img_sz;
  
  char *texture_fn;

  int fd;

  void *m;

  int retval;

  double sample;

  double applied_sample;
  
  ssize_t bytes_read;
  
  ssize_t bytes_written;

  int out_fd;

  double filtered_vol;

  long int sampleno;

  double elapsed;

  long int xpos, ypos;

  pixel_t black;

  long int samplerate;

  double inv_sf;
  
  texture_fn = argc>1 ? argv[1] : NULL;

  img.xres = 960;
  img.yres = 960;
  
  num_pixels = img.xres * img.yres;

  img_sz = num_pixels * sizeof(pixel_t);

  fd = open(texture_fn, O_RDWR);
  if (fd == -1) {
    perror("open");
    return -1;
  }

  m = mmap(NULL, img_sz, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
  if (m == MAP_FAILED) {
    perror("mmap");
    return -1;
  }

  img.rgb = (pixel_t*) m;
  
  out_fd = 1;

  sampleno = 0;

  samplerate = 48000;
  
  black = (pixel_t) { .r = 0, .g = 0, .b = 0 };

  inv_sf = argc>2 ? strtod(argv[2],NULL) : 1.0;
  
  for (;;) {

    elapsed = sampleno; elapsed /= samplerate;
    elapsed /= inv_sf;

    bytes_read = read(0, &sample, sizeof(double));
    if (bytes_read != sizeof(double)) {
      break;
    }

    xpos = (elapsed - floor(elapsed)) * img.xres;    

    for (ypos = 0; ypos < img.yres; ypos++) {
      if (memcmp(img.rgb + ypos * img.xres + xpos, &black, sizeof(pixel_t))) {
	break;
      }
    }

    filtered_vol = ypos;
    filtered_vol /= img.yres;
    
    applied_sample = sample * filtered_vol;
    
    bytes_written = write(out_fd, &applied_sample, sizeof(double));
    if (bytes_written != sizeof(double)) {
      perror("write");
      return -1;
    }

    sampleno++;
    
  }
  
  retval = munmap(m, img_sz);
  retval = close(fd);
  
  return 0;

}
