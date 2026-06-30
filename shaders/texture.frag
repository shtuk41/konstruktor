#version 330

in vec2 UV;
out vec4 color_out;
uniform sampler2D textureSampler;


void main()
{
	color_out = texture(textureSampler, UV).rgba;
	//color_out = vec4(1.0,1.0,0.0,1.0);
}