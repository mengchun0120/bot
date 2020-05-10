#version 120

#ifdef GL_ES
precision mediump float;
#endif

uniform bool useTex;
uniform sampler2D texture;

varying vec4 vColor;
varying float alpha;

void main()
{
    vec4 tmpColor = vec4(vColor.rgb, vColor.a * alpha);
	if(!useTex) {
        gl_FragColor = tmpColor;
    } else {
        gl_FragColor = texture2D(texture, gl_PointCoord) * tmpColor;
    }
}