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
	
	uniform mat4 finalBonesMatrices[MAX_BONES];
	
	uniform vec3 LightPosition;
	
	out vec2 TextureCoords;
	out vec3 Normal;
	out vec3 FragPos;
	out vec3 LightPos;
	
	void main()
	{
		//Bones
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
	
		FragPos = vec3(model * vec4(aPos, 1.0f));
		TextureCoords = textCoords;
		Normal = normalize(mat3(transpose(inverse(model))) * normals);
		//LightPos = vec3(view * vec4(LightPosition, 1.0));
		LightPos = vec3(vec4(LightPosition, 1.0));
		
		gl_Position = projection * view * model * totalPosition;
	}
#endif
#ifdef FRAGMENT_PROGRAM

	uniform float AmbientStrength;
	uniform float SpecularStrength;
	uniform vec3 LightColor;
	uniform vec3 ViewPoint;

	uniform sampler2D albedo_texture;
	uniform sampler2D normal_texture;
	uniform sampler2D roughness_texture;
	uniform sampler2D metallic_texture;
	
	uniform int steps;
	
	uniform vec3 meshColor;
	
	in vec2 TextureCoords;
	in vec3 Normal;
	in vec3 FragPos;
	in vec3 LightPos;
	out vec4 FragColor;
	
	void main()
	{
		vec4 albedo = texture(albedo_texture, TextureCoords);
		vec3 norm = normalize(texture(normal_texture, TextureCoords).rgb);
		
		float metallic = texture(metallic_texture, TextureCoords).r;
		//float roughness = texture(roughness_texture, TextureCoords).r;
		
		vec3 viewDir = normalize(ViewPoint - FragPos);
		//vec3 lightDir = normalize(clamp(LightPos, -1, 1) - FragPos);
		vec3 lightDir = normalize(LightPos - FragPos);
		
		
		//Ambient
		 vec3 Ambient = (LightColor * 1);
		
		//Diffuse
		float diff = max(dot(norm, lightDir), 0.0);
	
		vec3 Diffuse = (diff * LightColor);
		
		//Specular
		vec3 reflectDir = reflect(-lightDir, norm);
		
		float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
		
		vec3 Specular = (metallic * spec * LightColor);
		

		//Ramp
		float intensity = dot(norm, lightDir);
		float colorIndex = clamp(floor(intensity * float(steps)), -1.0, 1.0);
		float stepSize = 1 / float(steps);
		
		//Final
		vec3 result = ((Ambient + Specular) * vec3((colorIndex + 1.5) * stepSize));
	
		//vec3 result = Ambient() + vec3(((colorIndex + 0.5) * stepSize) * Diffuse(lightDir));
	
		FragColor = vec4(result, 1.0f) * albedo;
	} 
#endif

