' bu.bas - Ruud Helderman, June 2026 - MIT License
' Based on "Bubble Universe", written in Processing by yuruyurau
' https://x.com/yuruyurau/status/1226846058728177665

' Running this program in Liberty BASIC 4 takes ~2 hours
' and generate a raw video file of 230,400,000 bytes.
' It takes ffmpeg 5 seconds to convert it to MP4, ~11 MB.
' ffmpeg -f rawvideo -pixel_format rgb24 -video_size 320x320 -framerate 25 -i bu.raw bu.mp4
' You can watch the MP4 file in a video player like VLC.

global width
width = 320
height = 320
frames = 750

filedialog "Save raw video file", "*.raw", outfile$

' Truncate output file if it already exists
open outfile$ for output as #1
close #1

' Create/overwrite output file
open outfile$ for binary as #1

ds = width * height * 3
dim d(ds)
x0 = (width - 1) / 2
y0 = (height - 1) / 2
xs = x0 / 2 - 2
ys = y0 / 2 - 2

tau = 6.28318530718
s = tau / frames
for t = s/2 to tau step s
    print t
    for n = 0 to ds-1
        d(n) = 0
    next
    for i = 0 to 255
        x = 0
        y = 0
        for j = 0 to 255
            a = x + i
            b = y + i * 0.031 - t
            x = cos(a) + cos(b)
            y = sin(a) + sin(b)
            call DrawPoint x0 - x * xs, y0 - y * ys, i, j, 99
        next
    next
    for n = 0 to ds-1 step 3
        print #1, chr$(d(n)); chr$(d(n+1)); chr$(d(n+2))
    next
next
print "Done"

close #1

sub DrawPoint xf, yf, r, g, b
    xi = int(xf)
    yi = int(yf)
    call DrawPixel xf, yf, xi  , yi  , r, g, b
    call DrawPixel xf, yf, xi+1, yi  , r, g, b
    call DrawPixel xf, yf, xi  , yi+1, r, g, b
    call DrawPixel xf, yf, xi+1, yi+1, r, g, b
end sub

sub DrawPixel xf, yf, xi, yi, r, g, b
    n = (xi + yi * width) * 3
    a = (1 - abs(xf - xi)) * (1 - abs(yf - yi))
    r = r * a : if d(n  ) < r then d(n  ) = r
    g = g * a : if d(n+1) < g then d(n+1) = g
    b = b * a : if d(n+2) < b then d(n+2) = b
end sub
