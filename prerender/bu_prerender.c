// bu_prerender.c - Ruud Helderman, June 2026 - MIT License
// Based on "Bubble Universe", written in Processing by yuruyurau
// https://x.com/yuruyurau/status/1226846058728177665

// Generates a video rather than real-time rendering. It takes ~12 seconds.
// It is a test to set the benchmark for other languages (Liberty BASIC).
// The program writes to stdout a raw video of 921,600,000 bytes (DS * FRAMES).
// Use ffmpeg to convert it to a 30-second MP4 file:
// ffmpeg -f rawvideo -pixel_format rgb24 -video_size 640x640 -framerate 25 -i bu.raw bu.mp4
// Play the video in an infinite loop.

#include <math.h>
#include <stdio.h>
#include <string.h>

#define TAU  6.28318530718

#define WIDTH    640
#define HEIGHT   640
#define FRAMES   750

#define DS (WIDTH * HEIGHT * 3)

#define BYTE unsigned char

static BYTE data[DS];

static void draw_byte(BYTE *p, BYTE b)
{
	if (*p < b) *p = b;
}

static void draw_pixel(double x, double y, int xi, int yi, int r, int g, int b)
{
	double a = (1 - fabs(x - xi)) * (1 - fabs(y - yi));   // anti-aliasing
	BYTE *p = data + (xi + yi * WIDTH) * 3;
	draw_byte(p++, (BYTE)(r * a));
	draw_byte(p++, (BYTE)(g * a));
	draw_byte(p++, (BYTE)(b * a));
}

static void draw_star(double x, double y, int r, int g, int b)
{
	int xi = (int)x;
	int yi = (int)y;
	draw_pixel(x, y, xi  , yi  , r, g, b);
	draw_pixel(x, y, xi+1, yi  , r, g, b);
	draw_pixel(x, y, xi  , yi+1, r, g, b);
	draw_pixel(x, y, xi+1, yi+1, r, g, b);
}

int main()
{
	double x0 = 0.5 * (WIDTH  - 1), xs = 0.5 * x0 - 2;
	double y0 = 0.5 * (HEIGHT - 1), ys = 0.5 * y0 - 2;
	double s = TAU / FRAMES;
	for (double t = s/2; t < TAU; t += s)
	{
		memset(data, 0, DS);
		for (int i = 0; i < 256; i++)
		{
			double x = 0;
			double y = 0;
			for (int j = 0; j < 256; j++)
			{
				double a = x + i;
				double b = y + i * 0.031 - t;
				x = cos(a) + cos(b);
				y = sin(a) + sin(b);
				draw_star(x0 - x * xs, y0 - y * ys, i, j, 99);
			}
		}
		fwrite(data, 1, DS, stdout);
	}
	return 0;
}
