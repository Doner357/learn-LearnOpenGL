#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D screenTexture;

void main() {
    // Define gamma constant
    const float kGamma = 2.2;
    
    // Get the hdr color from screen texture
    vec3 hdr_color = texture(screenTexture, TexCoords).rgb;

    // Do Reinhard tone mapping
    vec3 mapped = hdr_color / (hdr_color + vec3(1.0));

    // Gamma correction
    mapped = pow(mapped, vec3(1.0 / kGamma));

    FragColor = vec4(mapped, 1.0);
}