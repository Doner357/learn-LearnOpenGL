#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D screenTexture;

// Used to control the exposure scale
uniform float exposure;

void main() {
    // Define gamma constant
    const float kGamma = 2.2;
    
    // Get the hdr color from screen texture
    vec3 hdr_color = texture(screenTexture, TexCoords).rgb;

    // Do exposure tone mapping
    vec3 mapped = vec3(1.0) - exp(-hdr_color * exposure);

    // Gamma correction
    mapped = pow(mapped, vec3(1.0 / kGamma));

    FragColor = vec4(mapped, 1.0);
}