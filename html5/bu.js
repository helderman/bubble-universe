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
    const t = time * 0.0002;   // animation speed
    const w = canvas.width;
    const h = canvas.height;
    const image = ctx.createImageData(w, h);
    const pixels = image.data;

    const paint = function(red, green) {
        const x = Math.round((galaxy.x * 0.24 + 0.5) * w);
        const y = Math.round((galaxy.y * 0.24 + 0.5) * h);
        if (x >= 0 && x < w && y >= 0 && y < h) {
            const i = (x + y * w) * 4;
            pixels[i] = red;
            pixels[i + 1] = green;
            pixels[i + 2] = 99;
            pixels[i + 3] = 255;     // alpha: fully opaque
        }
    }
    galaxy.x = galaxy.y = 0;
    for (let i = 0; i < 256; i++) {
        for (let j = 0; j < 256; j++) {
            galaxy.spiral(i, t);
            paint(i, j);
        }
    }
    ctx.putImageData(image, 0, 0);
    requestAnimationFrame(step);
}
requestAnimationFrame(step);
