# 'make' will generate an MP4 file; play it with looping.
# 'make play' will play the raw video directly to screen.
# Required software: make, ffmpeg

bu.mp4: bu.raw
	ffmpeg -f rawvideo -pixel_format rgb24 -video_size 640x640 -framerate 25 -i $< $@

bu.raw: bu_prerender
	time -p ./$< > $@

bu_prerender: bu_prerender.c
	$(CC) -O3 -Wall -Wextra -Wpedantic -Werror $< -lm -o $@

play:
	ffplay -f rawvideo -pixel_format rgb24 -video_size 640x640 -framerate 25 bu.raw

clean:
	rm bu_prerender bu.raw bu.mp4
