
%.rgb : %-source.png
	@convert -size 960x960 $^ -depth 16 $@

all : /tmp/output_audiostage1-float32.raw /tmp/output_audiostage2-float32.raw

/tmp/output_audiostage2-float32.raw : /tmp/output_appliedfilter-float64.raw
	@sox --endian little --bits 64 --encoding floating-point --channels 1 --rate 48000 $^ --bits 32 $@

/tmp/output_audiostage1-float32.raw : /tmp/output_audiofile-float64.raw
	@sox --endian little --bits 64 --encoding floating-point --channels 1 --rate 48000 $^ --bits 32 $@

/tmp/output_appliedfilter-float64.raw : TEXTURE_FN=./input_signal1.rgb

/tmp/output_appliedfilter-float64.raw : INV_SF=0.25

/tmp/output_appliedfilter-float64.raw : INPUT_AUDIO=/tmp/output_audiofile-float64.raw

/tmp/output_appliedfilter-float64.raw : $(INPUT_AUDIO) applyenv
	@./applyenv $(TEXTURE_FN) $(INV_SF) < $(INPUT_AUDIO) > $@.new
	mv $@.new $@

/tmp/output_audiofile-float64.raw : gen_audio
	@./$^ > $@.new
	mv $@.new $@

gen_audio : LIBS=-lm

gen_audio : gen_audio.o
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS) $(LIBS)

applyenv : CFLAGS+=-O3 -Wall -g -pg -I$(WORK1)/src/libgxkit

applyenv : LIBS=-lm

applyenv : applyenv.o
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS) $(LIBS)
