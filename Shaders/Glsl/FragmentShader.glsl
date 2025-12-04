#version 330 core
out vec4 FragColor;
uniform vec4 color;
uniform vec4 color1;
uniform vec4 color2;
uniform bool useTexture;
uniform sampler2D tex0;
uniform vec4 lightColor; 
uniform vec3 camPos;
in vec2 TexCoord; // Changed to vec2
in vec3 Normal ; 
in vec3 crntPos; 

uniform vec3 lightPos;

void main()
{
   
    float ambient = 0.20f;
    bool isColor1Unset = (color1.a == 0.0);
    bool isColor2Unset = (color2.a == 0.0);
    bool islightunset = (lightColor.a == 0.0);

    vec3 normal = normalize(Normal);
	vec3 lightDirection = normalize(lightPos - crntPos);
	float diffuse = max(dot(normal, lightDirection), 0.0f);

    float specularLight = 0.50f;
    vec3 viewDirection = normalize(camPos - crntPos);
    vec3 reflectionDirection = reflect(-lightDirection, normal);
    float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 8);
    float specular = specAmount * specularLight;

    if (!useTexture) { 
        if (isColor1Unset && isColor2Unset) { 
           if (islightunset) {
              FragColor = color;
           }
           else{
              FragColor = color * lightColor * (diffuse + ambient + specular);
           }
            
        } else if (isColor2Unset) {
           if (islightunset){
             FragColor = mix(color, color1, TexCoord.y) ;
           }
           else {
              FragColor = mix(color, color1, TexCoord.y) * lightColor * (diffuse + ambient + specular);  
           }
            
        } else {

               float factor = TexCoord.y;
               vec4 mix1 = mix(color, color1, factor);
               vec4 mix2 = mix(mix1, color2, factor);
             if (islightunset){
                       
                    FragColor = mix2 ;
              }
              else {
                 FragColor = mix2* lightColor * (diffuse + ambient + specular) ;
              
              }
            
        }
    } else {
       if(islightunset){
          FragColor = texture(tex0, TexCoord) ; // Corrected TexCoord to vec2
       } else{
            FragColor = texture(tex0, TexCoord) * lightColor * (diffuse + ambient + specular) ;
       }

        
    }
}
