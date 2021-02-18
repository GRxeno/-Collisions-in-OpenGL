#version 330 core

in vec2 UV;
in vec3 Normal;  
in vec3 FragPos;

out vec4 color;

uniform sampler2D u_Texture;
uniform vec4 u_Color;
uniform int u_Toggle;

uniform vec3 u_lightPos; 
uniform vec3 u_viewPos; 
uniform vec3 u_lightColor;

void main(){

	// ambient
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * u_lightColor;
  	
    // diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(u_lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * u_lightColor;
    
    // specular
    float specularStrength = 0.5;
    vec3 viewDir = normalize(u_viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * u_lightColor;  	
	
	if (u_Toggle == 1) {
		color.xyz = (ambient + diffuse + specular) * texture( u_Texture, UV ).rgb;
		color.a = 1.0f;
	} else { 
		color.xyz = (ambient + diffuse + specular) * u_Color.rgb;
		color.a = u_Color.a;
	}
}