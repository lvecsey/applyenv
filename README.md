
Takes the texture output from [envgd](https://github.com/lvecsey/envgd) as the filter to apply.

Start by using .png file as created from the envgd project. For example, place the file as input_signal1-source.png and then generate the raw .rgb file:

```console
make input_signal1.rgb
```

In the example Makefile, we compute everything with double values (float64)
and then use sox audio utility to convert everything to float values (float32)

Additionally we have a stage1 audio output file which is just the original file unaltered, and then a stage2 audio output which is the filtered output.

The filter is applied to each second of audio.

You can use a program like audacity to import and play back the audio file.
For example, File menu, Import, Raw Audio, Select the file, and then set the parameters to correctly match:

```
Encoding: 32-bit float
Byte order: Little-endian
Channels: 1 Channel (Mono)
Start offset: 0 bytes
Amount to import: 100 %
Sample rate: 48000 Hz
```

![Image of output](https://phrasep.com/~lvecsey/software/applyenv/output_applyenv-audacity.png)
