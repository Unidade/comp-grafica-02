#version 120

varying vec2 vTexCoord;
varying vec3 vPosition;

void main()
{
    // posição padrão usando a matriz fixa
    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
    
    // passa a posição do vértice no espaço do objeto para o fragment shader
    vPosition = gl_Vertex.xyz;
    
    // pega as coordenadas de textura do pipeline fixo (mesmo sem textura, para compatibilidade)
    vTexCoord = gl_MultiTexCoord0.st;
}

