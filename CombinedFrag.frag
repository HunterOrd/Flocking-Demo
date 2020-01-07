#version 330
in vec3 ex_color;
in vec3 out_normal;
in vec4 a_Param_Transform;
in vec2 UV;

uniform float Ambient_Var;
uniform float Diffuse_Var;
uniform float Texture_mix;
uniform float SkyToggle;
uniform float ColorToggle;

uniform sampler2D texture_Colors;

out vec3 gl_FragColor;

void main(void) {
    // Pass through the interpolated color with full opacity.
	vec3 ambient = Ambient_Var * ex_color;
	vec3 Tex_ambient = Ambient_Var * texture2D(texture_Colors, UV).rgb;
	vec3 diffuse = ex_color * Diffuse_Var * max(dot(out_normal, vec3(a_Param_Transform)),0.0);
	vec3 Tex_diffuse = texture2D(texture_Colors, UV).rgb * Diffuse_Var * max(dot(out_normal, vec3(a_Param_Transform)),0.0);
	if (SkyToggle == 1.0){
		gl_FragColor = texture2D(texture_Colors, UV).rgb; 
	}
	else if (ColorToggle == 1.0){
		gl_FragColor = ambient+diffuse;
	}
	else{	
		gl_FragColor = Tex_ambient+Tex_diffuse;
	}
    
}