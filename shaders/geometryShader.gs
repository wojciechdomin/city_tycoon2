#version 330 core

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

// Input from vertex shader
in vec4 vertexColor[];
in vec3 vertexPosition[]; // You need to pass this from vertex shader


// Output to fragment shader
out vec4 gsColor;
out vec3 gsNormal;


void main() {
    // Compute triangle normal from vertex positions
    vec3 p0 = vertexPosition[0];
    vec3 p1 = vertexPosition[1];
    vec3 p2 = vertexPosition[2];

    vec3 edge1 = p1 - p0;
    vec3 edge2 = p2 - p0;

    vec3 normal = normalize(cross(edge1, edge2));

    // Emit all 3 vertices with the same normal
    for (int i = 0; i < 3; ++i) {
        gl_Position = gl_in[i].gl_Position;
        gsColor = vertexColor[i]; // Pass original color
        gsNormal = normal;        // Pass face normal (same for all)
	
        EmitVertex();
    }
    EndPrimitive();
}
