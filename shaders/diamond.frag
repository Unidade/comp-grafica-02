#version 120

uniform float uTime;        // tempo em segundos
uniform float uPulseSpeed;   // velocidade da pulsação
uniform float uGlowIntensity; // intensidade do brilho

varying vec2 vTexCoord;
varying vec3 vPosition;

void main()
{
    // calcula a distância do centro para criar efeito radial
    float dist = length(vPosition.xy);
    
    // pulsação baseada no tempo
    float pulse = sin(uTime * uPulseSpeed) * 0.5 + 0.5; // 0.0 a 1.0
    
    // múltiplas ondas de pulsação para efeito mais interessante
    float pulse1 = sin(uTime * uPulseSpeed);
    float pulse2 = sin(uTime * uPulseSpeed * 1.5 + 1.0);
    float pulse3 = cos(uTime * uPulseSpeed * 0.8);
    
    // combina as pulsações
    float combinedPulse = (pulse1 + pulse2 + pulse3) / 3.0;
    pulse = combinedPulse * 0.5 + 0.5;
    
    // cor base verde (tema DOOM)
    vec3 baseColor = vec3(0.2, 0.8, 0.3);
    
    // transição de cor baseada no tempo (verde -> amarelo -> laranja)
    float colorShift = sin(uTime * 0.8) * 0.5 + 0.5;
    vec3 color1 = vec3(0.2, 0.8, 0.3);  // verde
    vec3 color2 = vec3(0.9, 0.7, 0.2);  // amarelo/laranja
    vec3 dynamicColor = mix(color1, color2, colorShift * 0.6);
    
    // efeito de brilho que varia com a pulsação
    float glow = pulse * uGlowIntensity;
    
    // brilho mais intenso nas bordas (efeito de energia)
    float edgeGlow = 1.0 - smoothstep(0.3, 1.0, dist);
    glow += edgeGlow * 0.5;
    
    // cor final com brilho
    vec3 finalColor = dynamicColor * (1.0 + glow);
    
    // adiciona um brilho branco/amarelo nas áreas mais brilhantes
    vec3 highlightColor = vec3(1.0, 1.0, 0.7);
    finalColor = mix(finalColor, highlightColor, glow * 0.3);
    
    // garante que as cores não ultrapassem 1.0
    finalColor = clamp(finalColor, 0.0, 1.0);
    
    gl_FragColor = vec4(finalColor, 1.0);
}

