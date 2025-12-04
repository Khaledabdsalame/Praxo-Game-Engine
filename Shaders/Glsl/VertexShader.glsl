#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTex; // Changed to vec2
layout (location = 2) in vec3 aNormal;


uniform bool useTexture;




out vec2 TexCoord; // Changed to vec2

out vec3 Normal; 
out vec3 crntPos;

uniform mat4 camMatrix;

uniform mat4 model;

void main()
{
      
      vec4 worldPos = model * vec4(aPos, 1.0);
      crntPos = worldPos.xyz;
      gl_Position = camMatrix * worldPos;
      
    if (useTexture) {
        TexCoord = aTex; // Use provided texture coordinates
    } else {
        TexCoord = vec2(aPos.x * 0.5 + 0.5, 1.0 - (aPos.y * 0.5 + 0.5)); // Generate texture coordinates from position
    }

     Normal = aNormal;
}