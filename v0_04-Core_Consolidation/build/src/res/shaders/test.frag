#version 330 core

struct Material {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};

struct Light {
	vec3 position;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

in vec3 fragPos;
in vec3 normal;
in vec2 textCoord;

uniform Material material;
uniform Light light;

uniform vec3 viewPos;

uniform sampler2D myTexture;

out vec4 fragColor;

void main() {
	vec3 ambient = light.ambient * material.ambient;

	vec3 norm = normalize(normal);
	vec3 lightDir = normalize(light.position - fragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = light.diffuse * (diff * material.diffuse);

	vec3 viewDir = normalize(viewPos - fragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = light.specular * (spec * material.specular);
	
	vec4 result = vec4((ambient + diffuse + specular), 1.0);
	vec4 textColor = texture(myTexture, textCoord) * result;
	fragColor = textColor;

	/*tutorial:
	vec3 result = ambient + diffuse + specular;
	fragColor = vec4(result, 1.0);
	*/
}
