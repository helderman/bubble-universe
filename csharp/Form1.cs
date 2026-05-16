// Form1.cs - Ruud Helderman, May 2026 - MIT License
// Graphical animation running inside a window, implemented in C# + WinForms
// Based on "Bubble Universe", written in Processing by yuruyurau
// https://x.com/yuruyurau/status/1226846058728177665

using System.Runtime.InteropServices;   // GCHandle

namespace bu;

public partial class Form1 : Form
{
    public Form1()
    {
        InitializeComponent();
        DoubleBuffered = true;      // prevent flickering
        BackColor = Color.Black;    // blend our images with a black background
        Paint += Form1_Paint;
    }

    private void Form1_Paint(object? sender, PaintEventArgs e)
    {
        int w = ClientSize.Width;
        int h = ClientSize.Height;
        double x0 = 0.5 * w - 0.5;      // accurately center the image
        double y0 = 0.5 * h - 0.5;
        double scale = Math.Min(w, h) * 0.24;                          // zoom factor
        double time = DateTime.UtcNow.TimeOfDay.TotalSeconds * 0.01;   // animation speed

        // Direct pixel manipulation, MUCH faster than Graphics methods
        var pixels = new int[w * h];    // array auto-initialized with 0 = transparent black
        GCHandle handle = GCHandle.Alloc(pixels, GCHandleType.Pinned);
        using (var bitmap = new Bitmap(w, h, w * 4, System.Drawing.Imaging.PixelFormat.Format32bppArgb, handle.AddrOfPinnedObject()))
        {
            for (int i = 0; i < 256; i++)       // yuruyurau's magic starts here
            {
                double x = 0, y = 0;
                for (int j = 0; j < 256; j++)
                {
                    double a = x + i;
                    double b = y + i * 0.031 - time;
                    x = Math.Cos(a) + Math.Cos(b);
                    y = Math.Sin(a) + Math.Sin(b);
                    DrawPoint(x0 - x * scale, y0 - y * scale, i, j, 99);
                }
            }
            e.Graphics.DrawImageUnscaled(bitmap, 0, 0);    // dump pixels to window
        }
        handle.Free();
        Invalidate();       // next frame please

        // Local functions
        void DrawPoint(double x, double y, int red, int green, int blue)
        {
            int xi = (int)Math.Floor(x);
            int yi = (int)Math.Floor(y);
            DrawPixel(xi  , yi  );
            DrawPixel(xi+1, yi  );
            DrawPixel(xi  , yi+1);
            DrawPixel(xi+1, yi+1);

            void DrawPixel(int xi, int yi)
            {
                // Bounds check; really only necessary for zoom factor > 0.25
                if (xi >= 0 && xi < w && yi >= 0 && yi < h)
                {
                    // Simple anti-aliasing: the closer the pixel (xi, yi) is
                    // to the real coordinates (x, y), the higher the alpha
                    double a = (1 - Math.Abs(x - xi)) * (1 - Math.Abs(y - yi));

                    // Bit-wise 'or' as a poor man's blend;
                    // very inaccurate, but OK-ish as the points are sparse
                    pixels[xi + yi * w] |= Color.FromArgb((int)(a * 255), red, green, blue).ToArgb();
                }
            };
        }
    }
}
