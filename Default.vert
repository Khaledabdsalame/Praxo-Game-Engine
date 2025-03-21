#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTex; // Changed to vec2

uniform float size;
uniform bool useTexture;


out vec2 TexCoord; // Changed to vec2

uniform mat4 camMatrix;
void main()
{
 
      gl_Position = camMatrix*vec4(size * aPos, 1.0);
  
    

    if (useTexture) {
        TexCoord = aTex; // Use provided texture coordinates
    } else {
        TexCoord = vec2(aPos.x * 0.5 + 0.5, 1.0 - (aPos.y * 0.5 + 0.5)); // Generate texture coordinates from position
    }
}