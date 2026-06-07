# 'make' will generate an MP4 file from the raw video produced by bu.bas
# 'make play' will play the raw video directly to screen
# Required software: make, ffmpeg

bu.mp4: bu.raw
	ffmpeg -f rawvideo -pixel_format rgb24 -video_size 640x640 -framerate 25 -i $< $@

play:
	ffplay -f rawvideo -pixel_format rgb24 -video_size 640x640 -framerate 25 bu.raw

clean:
	rm bu.mp4 buc.raw buc.mp4
