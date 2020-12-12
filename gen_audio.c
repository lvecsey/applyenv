
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include <math.h>

int main(int argc, char *argv[]) {

  double duration;

  long int samplerate;

  long int num_channels;

  double sample;

  long int num_samples;
  
  long int sampleno;

  double radians;
  
  double vol;

  int retval;

  double freq;
  
  duration = 20.0;

  samplerate = 48000;

  num_channels = 1;

  num_samples = samplerate * num_channels * duration;

  vol = 0.9875;

  freq = 440.0;
  
  for (sampleno = 0; sampleno < num_samples; sampleno++) {

    radians = (2.0 * M_PI * freq * duration * sampleno) / num_samples;
    
    sample = vol * cos(radians);

    retval = write(1, &sample, sizeof(double));
    if (retval != sizeof(double)) {
      perror("write");
      return -1;
    }
    
  }
  
  return 0;

}
