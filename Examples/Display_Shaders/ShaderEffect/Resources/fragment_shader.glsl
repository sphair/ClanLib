
out vec4 FragColor;

layout(std140) uniform Uniforms
{
    vec3 resolution;
    float time;
    vec4 mouse;
    vec4 particles[30];
    int particle_count;
};

layout(origin_upper_left) in vec4 gl_FragCoord;

float sum()
{
    float sum = 0.0;

    for (int i = 0; i < particle_count; i++) {
        vec4 p = particles[i];
        sum += p.a * p.z / distance(p.xy, gl_FragCoord.xy);
    }

    return sum;
}

void main()
{
    float s = sum();
    FragColor = vec4(s * cos(mouse.y), s * sin(mouse.x) / 2.0, s * sin(mouse.y), 0.08);
}

