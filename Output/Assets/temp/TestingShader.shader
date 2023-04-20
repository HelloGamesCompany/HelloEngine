#version 410 core
#ifdef VERTEX_PROGRAM
	layout (location = 0) in vec3 aPos; 
	layout (location = 1) in vec3 normals;
	layout (location = 2) in vec2 textCoords;
	
	uniform mat4 view;
	uniform mat4 projection;
	uniform mat4 model;
	uniform mat4 model_rot;
	
	uniform sampler2D normal_texture;
	
	out vec2 TextureCoords;
	out vec3 Normal;
	out vec3 FragPos;
	
	void main()
	{
		vec4 aPos4 = vec4(aPos, 1.0);
		
		//OUT
		FragPos = normalize(vec3(model * aPos4));
		TextureCoords = textCoords;

		Normal = normalize(texture(normal_texture, TextureCoords).rgb * 2.0 - 1.0);
		Normal = normalize (vec3(model * vec4(normals, 0.0)));
		
		gl_Position = projection * view * model * aPos4;
	}
#endif
#ifdef FRAGMENT_PROGRAM
	const uint MAX_POINT = 32;
	const uint MAX_SPOT = 32;

	struct Light
	{
		vec3 Color;
		float AmbientIntensity;
		float DiffuseIntensity;
	};
	
	struct DirectionalLight
	{
		Light Base;
		vec3 Direction;
	};
	
	struct PointLight
	{
		Light Base;
		
		float Constant;
		float Linear;
		float Exp;
		
		vec3 Position;
	};
	
	struct SpotLight
	{
		Light Base;
		
		float Constant;
		float Linear;
		float Exp;
		float Cutoff;
		
		vec3 Position;
		vec3 Direction;
		
	};
	
	uniform DirectionalLight Light_Directional;
	uniform PointLight[MAX_POINT] Light_Point;
	uniform SpotLight[MAX_SPOT] Light_Spot; 
	
	uniform int Actual_Point;
	uniform int Actual_Spot;
	
	uniform sampler2D albedo_texture;
	//uniform sampler2D normal_texture;
	//uniform sampler2D metallic_texture;
	
	uniform vec3 ViewPoint;
	//uniform vec3 LightColor;
	//uniform vec3 specularLightColor;

	//uniform float shininess = 32.0f;
	//uniform float specularIntensity = 1.0f;
	
	int steps = 3;
	float scaleFactor = 1.0f/steps;
	
	in vec2 TextureCoords;
	in vec3 FragPos;
	in vec3 Normal;
	
	out vec4 FragColor;
	
	vec4 CalculateLight(Light light, vec3 direction, vec3 normal)
	{
		//Ambient
		vec4 Ambient = vec4(light.Color, 1.0f) * light.AmbientIntensity; // * AmbientColor;
		
		//Diffuse
		float diff = max(dot(normal, direction), 0.0);
		//float diff = dot(normal, -direction);
		
		diff = floor(diff * steps) * scaleFactor;
			
		vec4 Diffuse = vec4(light.Color, 1.0f) * light.DiffuseIntensity * diff;
		Diffuse.xyz = clamp(Diffuse.xyz, 0.25, 1.0);
		Diffuse.w = 1.0f;
	
		vec3 viewDir = normalize(ViewPoint - FragPos);
		//Specular
		//vec3 reflectDir = reflect(direction, normal);
		//float SpecularStrength = texture(metallic_texture, TextureCoords).r;
		
		//float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
		
		//vec4 Specular = vec4((SpecularStrength * specularIntensity) * spec * specularLightColor, 1.0f);
		
		//return (Ambient * (Diffuse + Specular));
		return (Ambient * (Diffuse));
	}
	
	//Calculate Lights
	vec4 CalculateDirectionalLight(vec3 normal)
	{
		vec3 dir = normalize(Light_Directional.Direction);
		return CalculateLight(Light_Directional.Base, dir, normal);
	}
	
	vec4 CalculatePointLight(PointLight light, vec3 normal)
	{
		vec3 lightPos = FragPos - light.Position;
		float dist = length(lightPos);
		lightPos = normalize(lightPos) * -1;
		
		vec4 color = CalculateLight(light.Base, normalize(lightPos), normal);
		
		float attenuation = light.Constant + (light.Linear * dist) * (light.Exp * dist) * dist;
		
		return (color / attenuation);
	}
	
	vec4 CalculateSpotLight(SpotLight light, vec3 normal)
	{
		vec3 lightToPixel = normalize(FragPos - light.Position);
		float spotFactor = dot(lightToPixel, light.Direction);
		
		if (spotFactor > light.Cutoff)
		{
			vec3 lightPos = FragPos - light.Position;
			float dist = length(lightPos);
			lightPos = normalize(lightPos) * -1;
		
			vec4 color = CalculateLight(light.Base, normalize(lightPos), normal);
		
			float attenuation = light.Constant + (light.Linear * dist) * (light.Exp * dist) * dist;
			float spotLightIntensity = (1.0 - (1.0 - spotFactor) / (1.0 - light.Cutoff));
			
			return (color / attenuation) * spotLightIntensity;
		}
	}
	
	uniform vec3 ColourTest;
	
	void main()
	{
		//vec3 norm = normalize(texture(normal_texture, TextureCoords).rgb * 2.0 - 1.0);
		//norm = norm + Normal;
		
		//Directional
		vec4 result = CalculateDirectionalLight(Normal);
		
		//Point
		for (int i = 0; i < Actual_Point; i++)
		{
			result += CalculatePointLight(Light_Point[i], Normal);
		}
		
		//Spot
		for (int i = 0; i < Actual_Spot; i++)
		{
			result += CalculateSpotLight(Light_Spot[i], Normal);
		}
		
		
		vec3 texDiffCol = texture2D(albedo_texture, TextureCoords).rgb;
		if (length(texDiffCol) != 0.0)
		{
			FragColor = texture(albedo_texture, TextureCoords) * result * vec4(ColourTest, 1.0f);
		}
		else
		{
		FragColor = result * vec4(ColourTest, 1.0f);
		}
		
	}
	
#endif







