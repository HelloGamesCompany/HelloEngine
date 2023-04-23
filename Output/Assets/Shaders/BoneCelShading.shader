#version 410 core
#ifdef VERTEX_PROGRAM
	layout (location = 0) in vec3 aPos; 
	layout (location = 1) in vec3 normals;
	layout (location = 2) in vec2 textCoords;
	layout (location = 3) in ivec4 boneIds;
	layout (location = 4) in vec4 weights;
	
	const int MAX_BONES = 100;
	const int MAX_BONE_INFLUENCE = 4;
	
	uniform mat4 view;
	uniform mat4 projection;
	uniform mat4 model;
	uniform vec4 LightPosition;
	uniform mat4 model_rot;
	uniform mat4 finalBonesMatrices[MAX_BONES];
	
	uniform sampler2D normal_texture;
	
	out vec2 TextureCoords;
	out vec3 Normal;
	out vec3 FragPos;
	out vec4 GlobalLightPos;
	
	void main()
	{
		vec4 totalPosition = vec4(0.0f);
		for (int i = 0; i < MAX_BONE_INFLUENCE; i++)
		{
			if (boneIds[i] == -1) 
				continue;

			if (boneIds[i] >= MAX_BONES)
			{
				totalPosition = vec4(aPos, 1.0f);
				break;
			}

			vec4 localPosition = finalBonesMatrices[boneIds[i]] * vec4(aPos, 1.0f);
			totalPosition += localPosition * weights[i];
			vec3 localNormal = mat3(finalBonesMatrices[boneIds[i]]) * normals;
		}

		if (totalPosition.x == 0.0f && totalPosition.y == 0.0f && totalPosition.z == 0.0f && totalPosition.w == 0.00f)
		{
			totalPosition = vec4(aPos, 1.0f);
		}
	
		vec4 aPos4 = vec4(aPos, 1.0);
		
		//OUT
		FragPos = normalize(vec3(model * aPos4));
		TextureCoords = textCoords;
		GlobalLightPos = LightPosition;//clamp(LightPosition, -1.0, 1.0);
		//vec3(View * vec4(light.direction, 0.0))
		//Normal = normalize (vec3(model * vec4(normals, 0.0)));
		Normal = normalize(texture(normal_texture, TextureCoords).rgb * 2.0 - 1.0);
		Normal = normalize (vec3(model * vec4(normals, 0.0)));
		
		gl_Position = projection * view * model * totalPosition;
	}
#endif
#ifdef FRAGMENT_PROGRAM
	uniform sampler2D albedo_texture;
	//uniform sampler2D normal_texture;
	uniform sampler2D metallic_texture;
	
	uniform vec3 ViewPoint;
	uniform vec3 LightColor;
	uniform vec3 specularLightColor;
	float AmbientStrength = 1.0f;
	float DiffuseStrength = 1.0f;
	//float SpecularStrength = 0.5f;
	uniform float shininess = 32.0f;
	uniform float specularIntensity = 1.0f;
	
	int steps = 3;
	float scaleFactor = 1.0f/steps;
	
	in vec2 TextureCoords;
	in vec3 FragPos;
	in vec4 GlobalLightPos;
	in vec3 Normal;
	
	out vec4 FragColor;
	
	vec4 CalculateLight(vec3 direction, vec3 normal)
	{
		//Ambient
		vec4 Ambient = vec4(LightColor, 1.0f) * AmbientStrength; // * AmbientColor;
		
		//Diffuse
		float diff = max(dot(normal, direction), 0.0);
		//float diff = dot(normal, -direction);
		
		diff = floor(diff * steps) * scaleFactor;
			
		vec4 Diffuse = vec4(LightColor, 1.0f) * DiffuseStrength * diff;
		Diffuse.xyz = clamp(Diffuse.xyz, 0.25, 1.0);
		Diffuse.w = 1.0f;
	
		vec3 viewDir = normalize(ViewPoint - FragPos);
		//Specular
		vec3 reflectDir = reflect(direction, normal);
		float SpecularStrength = texture(metallic_texture, TextureCoords).r;
		
		float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
		
		vec4 Specular = vec4((SpecularStrength * specularIntensity) * spec * specularLightColor, 1.0f);
		
		//return (Ambient * (Diffuse + Specular));
		return (Ambient * (Diffuse + Specular));
	}
	
	//Light types
	vec4 DirectionalLight(vec3 normal, vec3 LightPos)
	{
		vec3 lightDir = normalize(LightPos);
		return CalculateLight(lightDir, normal);
	
	}
	
	uniform vec4 ColourTest;
	
	void main()
	{
		//vec3 norm = normalize(texture(normal_texture, TextureCoords).rgb * 2.0 - 1.0);
		//norm = norm + Normal;
		vec4 result = DirectionalLight(Normal, GlobalLightPos.xyz);
		
		
		FragColor = texture(albedo_texture, TextureCoords) * result * ColourTest;
	}
	
#endif

