# Transformações 3D em OpenGL

Demonstração educacional simplificada das matrizes **Model**, **View** e **Projection** usando o pipeline fixo do OpenGL com FreeGLUT.

---

## 🎯 Objetivo Educacional

Entender os seguintes conceitos

- **Matriz Model**: translação, rotação e escala (`glTranslatef`, `glRotatef`, `glScalef`)
- **Matriz View**: posicionamento da câmera (`gluLookAt`)
- **Matriz Projection**: perspectiva 3D (`gluPerspective`)

---

## Compilar e Executar

### Instalação das Dependências

**Windows (MSYS2 MINGW64)**:

```bash
pacman -Syu
pacman -S mingw-w64-x86_64-gcc mingw-w64-x86_64-freeglut
pacman -S mingw-w64-x86_64-gdb
pacman -S mingw-w64-x86_64-glew
pacman -S mingw-w64-x86_64-glm
```

**Linux (Ubuntu/Debian)**:

```bash
sudo apt-get install freeglut3-dev build-essential
```

## 📚 O Que o Programa Demonstra

### 1. Matriz MODEL

Transforma cada objeto individualmente:

```cpp
// Cubo à esquerda
glPushMatrix();
glTranslatef(-2.5f, 0.0f, 0.0f);        // Translação
glRotatef(cubeRotation, 0.0f, 1.0f, 0.0f);  // Rotação
glScalef(1.0f, 1.0f, 1.0f);             // Escala
drawCube();
glPopMatrix();
```

**Conceito-chave**: Cada objeto tem sua própria matriz Model!

### 2. Matriz VIEW

Define a posição e orientação da câmera:

```cpp
glMatrixMode(GL_MODELVIEW);
glLoadIdentity();

gluLookAt(0.0f, 2.0f, 8.0f,   // Posição da câmera
          0.0f, 0.0f, 0.0f,   // Para onde olha
          0.0f, 1.0f, 0.0f);  // Vetor "para cima"
```

### 3. Matriz PROJECTION

Define o tipo de projeção (perspectiva):

```cpp
glMatrixMode(GL_PROJECTION);
glLoadIdentity();

gluPerspective(45.0f,     // Campo de visão (FOV)
               aspect,     // Aspect ratio
               0.1f,       // Near plane
               100.0f);    // Far plane
```

---

## 🎨 O Que Você Verá

1. **Cubo à esquerda**: Rotacionando no eixo Y
2. **Pirâmide à direita**: Rotacionando no eixo X
3. **Grid 3×3 de cubos pequenos**: Demonstrando instanciação

**Animação**: 60 FPS, rotação contínua

---

## 🎓 Conceitos Ensinados

### Pipeline de Transformação

```
Coordenadas Locais
       ↓
[Matriz MODEL] ← glTranslatef, glRotatef, glScalef
       ↓
Coordenadas do Mundo
       ↓
[Matriz VIEW] ← gluLookAt
       ↓
Coordenadas da Câmera
       ↓
[Matriz PROJECTION] ← gluPerspective
       ↓
Tela
```

### Transformações Básicas

#### Translação (Mover)

```cpp
glTranslatef(x, y, z);
```

#### Rotação (Girar)

```cpp
glRotatef(angle, axisX, axisY, axisZ);
```

#### Escala (Redimensionar)

```cpp
glScalef(x, y, z);
```

### Salvar/Restaurar Estado

```cpp
glPushMatrix();  // Salva estado atual
// ... transformações ...
glPopMatrix();   // Restaura estado
```

---

## 🎮 Controles

- **R**: Resetar rotações
- **ESC**: Sair

---

## 📖 Para Uso em Aula

### Demonstrações Sugeridas

#### 1. Modificar Posição (5 min)

Altere a posição do cubo:

```cpp
glTranslatef(-5.0f, 0.0f, 0.0f);  // Mais à esquerda
```

#### 2. Alterar Velocidade de Rotação (5 min)

No `update()`:

```cpp
cubeRotation += 3.0f;  // 3x mais rápido
```

#### 3. Mudar Escala (5 min)

```cpp
glScalef(2.0f, 0.5f, 1.0f);  // Largo, achatado
```

#### 4. Ajustar Câmera (5 min)

```cpp
// Vista de cima
gluLookAt(0.0f, 10.0f, 0.1f,
          0.0f, 0.0f, 0.0f,
          0.0f, 1.0f, 0.0f);
```

#### 5. Alterar FOV (5 min)

```cpp
gluPerspective(90.0f, aspect, 0.1f, 100.0f);  // FOV mais amplo
```

### Exercícios Práticos

1. **Adicionar terceiro objeto**: Crie uma segunda pirâmide
2. **Criar animação orbital**: Faça objetos orbitarem um ponto
3. **Implementar zoom**: Use teclas +/- para ajustar FOV
4. **Grid maior**: Mude o loop de 3×3 para 5×5

---
