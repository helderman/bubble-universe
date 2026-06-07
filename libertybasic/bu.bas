' bu.bas - Ruud Helderman, June 2026 - MIT License
' Based on "Bubble Universe", written in Processing by yuruyurau
' https://x.com/yuruyurau/status/1226846058728177665

' Running this program in Liberty BASIC v4 takes about 3 hours
' and generates a raw video file of 921,600,000 bytes.
' It will take ffmpeg about 25 seconds to convert it to
' a 30-second MP4 file of about 27 MB.
' ffmpeg -f rawvideo -pixel_format rgb24 -video_size 640x640 -framerate 25 -i bu.raw bu.mp4
' You can watch the MP4 file in a video player like VLC. Loop it.

global width
width  = 640
height = 640
frames = 750

outfile$ = "bu.raw"
'filedialog "Save raw video file", "*.raw", outfile$
'if outfile$ = "" then end

start$ = time$()
print "Started: "; start$

' Truncate output file if it already exists
open outfile$ for output as #1
close #1

' Create/overwrite output file
open outfile$ for binary as #1

ds = width * height * 3   ' 3 bytes per pixel (RGB)
dim d(ds-1)               ' array to store 1 whole frame
x0 = (width - 1) / 2      ' center (pixel coordinates)
y0 = (height - 1) / 2
xs = x0 / 2 - 2           ' scale (zoom factor)
ys = y0 / 2 - 2

tau = 6.28318530718       ' animation repeats itself after t = 2*pi
s = tau / frames
for t = s/2 to tau step s
    print t               ' simple progress indicator
    scan                  ' handle events (so we can force-stop)
    for n = 0 to ds-1
        d(n) = 0          ' start with a black background
    next
    for i = 0 to 255      ' yuruyurau's magic starts here
        x = 0
        y = 0
        for j = 0 to 255
            a = x + i
            b = y + i * 0.031 - t
            x = cos(a) + cos(b)
            y = sin(a) + sin(b)
            call DrawStar x0 - x * xs, y0 - y * ys, i, j, 99
        next
    next
    for n = 0 to ds-1 step 3
        print #1, chr$(d(n)); chr$(d(n+1)); chr$(d(n+2))
    next
next

close #1
print "Started:  "; start$
print "Finished: "; time$()
end

sub DrawStar xf, yf, r, g, b
    xi = int(xf)
    yi = int(yf)
    call DrawPixel xf, yf, xi  , yi  , r, g, b
    call DrawPixel xf, yf, xi+1, yi  , r, g, b
    call DrawPixel xf, yf, xi  , yi+1, r, g, b
    call DrawPixel xf, yf, xi+1, yi+1, r, g, b
end sub

sub DrawPixel xf, yf, xi, yi, r, g, b
    ' Simple anti-aliasing: the closer the pixel (xi, yi) is
    ' to the real coordinates (xf, yf), the higher the alpha
    a = (1 - abs(xf - xi)) * (1 - abs(yf - yi))
    n = (xi + yi * width) * 3
    r = r * a : if d(n  ) < r then d(n  ) = r
    g = g * a : if d(n+1) < g then d(n+1) = g
    b = b * a : if d(n+2) < b then d(n+2) = b
end sub

