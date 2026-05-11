// bu.js - Ruud Helderman, 2026-05-09 - MIT License
// Based on "Bubble Universe", written in Processing by yuruyurau
// https://x.com/yuruyurau/status/1226846058728177665

const canvas = document.getElementById('bu');
const ctx = canvas.getContext('2d');

(window.onresize = function() {
    canvas.width = canvas.height = Math.min(window.innerWidth, window.innerHeight);
})();

const galaxy = {
    spiral: function(i, t) {
        const a = this.x + i;
        const b = this.y + i * 0.031 + t;
        this.x = Math.cos(a) + Math.cos(b);
        this.y = Math.sin(a) + Math.sin(b);
    }
}

function step(time) {
    const t = time * 0.0001;   // animation speed
    const w = canvas.width;
    const h = canvas.height;
    const image = ctx.createImageData(w, h);
    const pixels = image.data;

    const blend = function(i, v) {    // there's many fun ways you can do this
        const n = 1 - v/255;
        pixels[i] = 255 - (255 - pixels[i]) * n * n;
    };
    const paint = function(i, red, green, blue, a) {
        blend(i, a * red);
        blend(i+1, a * green);
        blend(i+2, a * blue);
        pixels[i+3] = 255;     // alpha: fully opaque
    };
    galaxy.x = galaxy.y = 0;
    for (let i = 0; i < 256; i++) {
        for (let j = 0; j < 256; j++) {
            galaxy.spiral(i, t);
            // paint a dot with anti-aliasing
            const x = (galaxy.x * 0.24 + 0.5) * w, xi = Math.round(x);
            const y = (galaxy.y * 0.24 + 0.5) * h, yi = Math.round(y);
            if (xi > 0 && xi < w && yi > 0 && yi < h) {
                paint(4 * (xi   +  yi    * w), i, j, 99, (0.5 + x - xi) * (0.5 + y - yi));
                paint(4 * (xi-1 +  yi    * w), i, j, 99, (0.5 + xi - x) * (0.5 + y - yi));
                paint(4 * (xi   + (yi-1) * w), i, j, 99, (0.5 + x - xi) * (0.5 + yi - y));
                paint(4 * (xi-1 + (yi-1) * w), i, j, 99, (0.5 + xi - x) * (0.5 + yi - y));
            }
        }
    }
    ctx.putImageData(image, 0, 0);
    requestAnimationFrame(step);
}
requestAnimationFrame(step);
