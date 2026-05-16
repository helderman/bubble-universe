// bu.js - Ruud Helderman, May 2026 - MIT License
// Based on "Bubble Universe", written in Processing by yuruyurau
// https://x.com/yuruyurau/status/1226846058728177665

const canvas = document.getElementById('bu');
const context = canvas.getContext('2d');

(window.onresize = function() {
    canvas.width = canvas.height = Math.min(window.innerWidth, window.innerHeight);
})();

const galaxy = {
    spiral: function(i, t) {
        const a = this.x + i;
        const b = this.y + i * 0.031 - t;
        this.x = Math.cos(a) + Math.cos(b);
        this.y = Math.sin(a) + Math.sin(b);
    }
}

function step(time) {
    const t = time * 0.0001;        // animation speed
    const w = canvas.width;
    const h = canvas.height;
    const x0 = 0.5 * (w - 1);       // accurately center the image
    const y0 = 0.5 * (h - 1);
    const scale = Math.min(w, h) * 0.24;            // zoom factor
    const image = context.createImageData(w, h);
    const pixels = image.data;

    const blend = function(i, v) {            // there's 1M ways to blend, but
        pixels[i] = Math.max(pixels[i], v);   // there's only one way to rock
    };
    const paint = function(x, y, xi, yi, w, red, green, blue) {
        if (xi >= 0 && xi < w && yi >= 0 && yi < h) {
            let i = 4 * (xi + yi * w);
            blend(i++, red);
            blend(i++, green);
            blend(i++, blue);
            blend(i, 255 * (1 - Math.abs(x - xi)) * (1 - Math.abs(y - yi)));
        }
    };
    for (let i = 0; i < 256; i++) {
        galaxy.x = galaxy.y = 0;
        for (let j = 0; j < 256; j++) {
            galaxy.spiral(i, t);
            // paint a dot with anti-aliasing
            const x = x0 - galaxy.x * scale, xi = Math.floor(x);
            const y = y0 - galaxy.y * scale, yi = Math.floor(y);
            paint(x, y, xi  , yi  , w, i, j, 99);
            paint(x, y, xi+1, yi  , w, i, j, 99);
            paint(x, y, xi  , yi+1, w, i, j, 99);
            paint(x, y, xi+1, yi+1, w, i, j, 99);
        }
    }
    context.putImageData(image, 0, 0);
    requestAnimationFrame(step);
}
requestAnimationFrame(step);
