// "Bubble Universe (naive)" - Ruud Helderman, June 2026
// https://www.shadertoy.com/view/N3sSW8
// Based on "Bubble Universe", written in Processing by yuruyurau
// https://x.com/yuruyurau/status/1226846058728177665

// The algorithm is great for a CPU-driven canvas, but pretty bad for a shader.
// The color dictates the coordinates; a shader shines when the coordinates dictate the color.
// In an attempt to mitigate the impact of all those redundant calculations on the framerate,
// I am letting the inner loop take bigger steps when far away from a star.

const fragmentShaderSource = `#version 300 es

precision lowp float;

out vec4 FragColor;

uniform vec2 u_resolution;
uniform float u_time;

void main()
{
    float zoom = smoothstep(-0.2, 4.0, u_time);    // 'big bang': zoom out in the first seconds
    vec2 r = u_resolution.xy;
    vec2 uv = (2.0 * gl_FragCoord.xy - r) / min(r.x, r.y) * zoom * 2.1;  // centered, normalized

    vec3 color = vec3(0);         // black background
    if (length(uv) < 2.2)         // optimization, outside the bubble it's black anyway
    {
        for (float i = 0.0; i < 1.0; i += 0.011)                   // loop galaxies
        {
            vec2 pos = vec2(0);   // position of star
            float d;              // (squared) distance to star

            for (float j = 0.0; j < 1.0; j += 0.01 + d * 0.04)     // loop stars per galaxy
            {
                pos += i * vec2(255, 8);                           // some chaos
                pos.y -= u_time * 0.1;                             // speed of animation
                pos = vec2(1) * mat2(cos(pos), sin(pos));          // yuruyurau's magic
                d = dot(pos - uv, pos - uv);
                color += vec3(i, j, 0.4) / (1.0 + d * 4e4);        // RGB and brightness
            }
        }
    }
    FragColor = vec4(tanh(color), 1);                              // keep the contrast
}
`;

// ------------------------------------
// JavaScript code
// ------------------------------------

// In our HTML page, there should be a canvas with ID 'bu'.
const canvas = document.getElementById('bu');
const gl = canvas.getContext('webgl2');
if (!gl) throw 'webgl2 not supported here';

// Create a 'program' with a vertex shader and a fragment shader.
const program = gl.createProgram();
const loadShader = function(gl, type, source) {
	const shader = gl.createShader(type);
	gl.shaderSource(shader, source);
	gl.compileShader(shader);
	if (!gl.getShaderParameter(shader, gl.COMPILE_STATUS)) {
		throw 'Compile error: ' + gl.getShaderInfoLog(shader);
	}
	return shader;
};
gl.attachShader(program, loadShader(gl, gl.VERTEX_SHADER, `#version 300 es
	in vec2 a_position;	// clip space [-1, +1]
	out vec2 v_texCoord;	// texel coordinates [0, 1]
	void main() {
		gl_Position = vec4(a_position, 0.0, 1.0);
		v_texCoord = (a_position + 1.0) * 0.5;
	}
`));
gl.attachShader(program, loadShader(gl, gl.FRAGMENT_SHADER, fragmentShaderSource));
gl.linkProgram(program);
if (!gl.getProgramParameter(program, gl.LINK_STATUS)) {
	throw 'Link error: ' + gl.getProgramInfoLog(program);
}

// 'Attributes' and 'uniforms' are used to pass parameters into the shaders.
// Each one has a unique number called a 'location'. Collecting those here.
const locations = {
	attribute: function(name) {
		this[name] = gl.getAttribLocation(program, name);
	},
	uniform: function(name) {
		this[name] = gl.getUniformLocation(program, name);
	}
};
locations.uniform('u_resolution');
locations.uniform('u_time');
locations.attribute('a_position');

// Prepare the vertex shader with a buffer containing 6 vertices (2 triangles).
const vertexShader = {
	positionBuffer: gl.createBuffer(),
	draw: function(positionLocation) {
		gl.enableVertexAttribArray(positionLocation);
		gl.bindBuffer(gl.ARRAY_BUFFER, this.positionBuffer);
		gl.vertexAttribPointer(positionLocation, 2, gl.FLOAT, false, 0, 0);
		gl.drawArrays(gl.TRIANGLES, 0, 6);
	}
};
gl.bindBuffer(gl.ARRAY_BUFFER, vertexShader.positionBuffer);
gl.bufferData(
	gl.ARRAY_BUFFER,
	new Float32Array([ -1, -1, 1, -1, -1, 1, -1, 1, 1, -1, 1, 1 ]),
	gl.STATIC_DRAW);

// Canvas size will always follow window size.
(window.onresize = function() {
	canvas.width = window.innerWidth;
	canvas.height = window.innerHeight;
})();

// Animation loop.
function step(time) {
	gl.viewport(0, 0, canvas.width, canvas.height);
	gl.useProgram(program);
	gl.uniform2f(locations.u_resolution, canvas.width, canvas.height);
	gl.uniform1f(locations.u_time, time / 1000);
	vertexShader.draw(locations.a_position);
	requestAnimationFrame(step);
}
requestAnimationFrame(step);
