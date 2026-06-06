bu.mp4: bu.raw
	ffmpeg -f rawvideo -pixel_format rgb24 -video_size 320x320 -framerate 25 -i $< $@

play:
	ffplay -f rawvideo -pixel_format rgb24 -video_size 320x320 -framerate 25 bu.raw

clean:
	rm bu.mp4
