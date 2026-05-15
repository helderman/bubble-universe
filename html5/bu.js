// bu.js - Ruud Helderman, May 2026 - MIT License
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
        const b = this.y + i * 0.031 - t;
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

    const blend = function(i, v) {            // there's 1M ways to blend, but
        pixels[i] = Math.max(pixels[i], v);   // there's only one way to rock
    };
    const paint = function(x, y, xi, yi, w, red, green, blue) {
        let i = 4 * (xi + yi * w);
        blend(i++, red);
        blend(i++, green);
        blend(i++, blue);
        blend(i, 255 * (1 - Math.abs(x - xi - 0.5)) * (1 - Math.abs(y - yi - 0.5)));
    };
    for (let i = 0; i < 256; i++) {
        galaxy.x = galaxy.y = 0;
        for (let j = 0; j < 256; j++) {
            galaxy.spiral(i, t);
            // paint a dot with anti-aliasing
            const x = (0.5 - galaxy.x * 0.24) * w, xi = Math.round(x);
            const y = (0.5 - galaxy.y * 0.24) * h, yi = Math.round(y);
            if (xi > 0 && xi < w && yi > 0 && yi < h) {
                paint(x, y, xi  , yi  , w, i, j, 99);
                paint(x, y, xi-1, yi  , w, i, j, 99);
                paint(x, y, xi  , yi-1, w, i, j, 99);
                paint(x, y, xi-1, yi-1, w, i, j, 99);
            }
        }
    }
    ctx.putImageData(image, 0, 0);
    requestAnimationFrame(step);
}
requestAnimationFrame(step);
