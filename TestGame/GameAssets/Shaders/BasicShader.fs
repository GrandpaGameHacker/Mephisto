#version 400
out vec4 FragColor;
  
in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
void main()
{
    FragColor = texture(texture_specular1, TexCoord);
}