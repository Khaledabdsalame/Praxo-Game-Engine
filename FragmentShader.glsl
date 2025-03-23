#version 330 core
out vec4 FragColor;
uniform vec4 color;
uniform vec4 color1;
uniform vec4 color2;
uniform bool useTexture;
uniform sampler2D tex0;

in vec2 TexCoord; // Changed to vec2

void main()
{
   
    bool isColor1Unset = (color1.a == 0.0);
    bool isColor2Unset = (color2.a == 0.0);

    if (!useTexture) { 
        if (isColor1Unset && isColor2Unset) {     
            FragColor = color;
        } else if (isColor2Unset) {
            FragColor = mix(color, color1, TexCoord.y);
        } else {
            float factor = TexCoord.y;
            vec4 mix1 = mix(color, color1, factor);
            vec4 mix2 = mix(mix1, color2, factor);
            FragColor = mix2;
        }
    } else {
        FragColor = texture(tex0, TexCoord); // Corrected TexCoord to vec2
    }
}
