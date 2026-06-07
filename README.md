# Bubble Universe
Epic animation: easy to implement, hard to understand!

## TL;DR Live demo
Just click the link:

[https://helderman.github.io/bubble-universe/html5/bu.html](https://helderman.github.io/bubble-universe/html5/bu.html)

I wrote the JavaScript code, but the math behind it came from people smarter than me.
What follows is a brief history of the animation through various programming languages.

## Processing
A digital artist named [yuruyurau](https://x.com/yuruyurau)
publishes beautiful animations on X,
complete with (surprisingly compact) source code written in a language called 'Processing'.

[Bubble Universe](https://x.com/yuruyurau/status/1226846058728177665)
was posted there on February 10, 2020.

```
W=540;N=200;x,y,t=0,0,0
def setup():size(W,W);noStroke()
def draw():global t;clear();[F(i,j)for i in range(N)for j in range(N)];t+=.1
def F(i,c):global x,y;r=TAU/N;u=sin(i+y)+sin(r*i+x);v=cos(i+y)+cos(r*i+x);x=u+t;y=v;fill(i,c,99);circle(u*N/2+W/2,y*N/2+W/2,2)
```

## BASIC
[Sinclair BASIC](https://en.wikipedia.org/wiki/Sinclair_BASIC)
would have been too slow to render the animation in real time on the original hardware,
but a good emulator can make it happen,
as demonstrated [on YouTube](https://www.youtube.com/watch?v=G5aOBh7qQe8)
by Paul Dunn in 2022.

Over time, the program got ported to many different BASIC dialects.

## Python
Mike Markowski wrote a short [article](https://udel.edu/~mm/bubble/)
about the underlying math, and posted about it
[on Facebook](https://www.facebook.com/groups/2057165187928233/posts/3982655872045812/)
in 2025.

## Scratch
RokCoder made [this nice implementation](https://scratch.mit.edu/projects/1314640592/) in 2026.
Instantly playable in the browser.   
Looks even better in [TurboWarp](https://turbowarp.org/1314640592).

## JavaScript
For online use, I made an implementation in
[HTML5 using canvas](https://github.com/helderman/bubble-universe/tree/main/html5/);
instantly playable in every modern web browser.
See the link at the top of this document!

## p5.js
Staying close to the Processing language used for the original implementation.
Not only is this
[instantly playable in the browser](https://editor.p5js.org/dojadragon/sketches/QJzoakkVa),
everybody can make their own adjustments and immediately see the result.

## SDL2
For desktop use, I made an implementation in
[C using SDL2](https://github.com/helderman/bubble-universe/tree/main/c/);
simple, portable and efficient.

## Direct2D
[C++ using Direct2D](https://github.com/helderman/bubble-universe/tree/main/direct2d/);
sample code from Microsoft helped me make this implementation for Windows desktop.

## WinForms
[C# using WinForms](https://github.com/helderman/bubble-universe/tree/main/csharp/);
efficient, but Windows-only.

## Liberty BASIC 4
Much too slow for real-time rendering, so I
[implemented](https://github.com/helderman/bubble-universe/tree/main/libertybasic/)
pre-rendering instead.
