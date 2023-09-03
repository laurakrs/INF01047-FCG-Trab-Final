#version 330 core

// Atributos de vértice recebidos como entrada ("in") pelo Vertex Shader.
// Veja a função BuildTrianglesAndAddToVirtualScene() em "main.cpp".
layout (location = 0) in vec4 model_coefficients;
layout (location = 1) in vec4 normal_coefficients;
layout (location = 2) in vec2 texture_coefficients;

// Matrizes computadas no código C++ e enviadas para a GPU
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform bool isBoundingBoxVertex;  // Verdadeiro para vértices de bounding box

// Atributos de vértice que serão gerados como saída ("out") pelo Vertex Shader.
// ** Estes serão interpolados pelo rasterizador! ** gerando, assim, valores
// para cada fragmento, os quais serão recebidos como entrada pelo Fragment
// Shader. Veja o arquivo "shader_fragment.glsl".
out vec4 position_world;
out vec4 position_model;
out vec4 normal;
out vec2 texcoords;

// para o coelho = Gouraud
out vec4 color_bunny;

// Identificador que define qual objeto está sendo desenhado no momento
#define BUNNY  3
#define BUNNY2 4

uniform int object_id;

// Parâmetros da axis-aligned bounding box (AABB) do modelo
uniform vec4 bbox_min;
uniform vec4 bbox_max;

// Variáveis para acesso da imagem de textura do coelho
uniform sampler2D TextureImage4;

// Identificador que define qual objeto está sendo desenhado no momento

void main()
{
    

    // A variável gl_Position define a posição final de cada vértice
    // OBRIGATORIAMENTE em "normalized device coordinates" (NDC), onde cada
    // coeficiente estará entre -1 e 1 após divisão por w.
    // Veja {+NDC2+}.
    //
    // O código em "main.cpp" define os vértices dos modelos em coordenadas
    // locais de cada modelo (array model_coefficients). Abaixo, utilizamos
    // operações de modelagem, definição da câmera, e projeção, para computar
    // as coordenadas finais em NDC (variável gl_Position). Após a execução
    // deste Vertex Shader, a placa de vídeo (GPU) fará a divisão por W. Veja
    // slides 41-67 e 69-86 do documento Aula_09_Projecoes.pdf.

    gl_Position = projection * view * model * model_coefficients;

    // Como as variáveis acima  (tipo vec4) são vetores com 4 coeficientes,
    // também é possível acessar e modificar cada coeficiente de maneira
    // independente. Esses são indexados pelos nomes x, y, z, e w (nessa
    // ordem, isto é, 'x' é o primeiro coeficiente, 'y' é o segundo, ...):
    //
    //     gl_Position.x = model_coefficients.x;
    //     gl_Position.y = model_coefficients.y;
    //     gl_Position.z = model_coefficients.z;
    //     gl_Position.w = model_coefficients.w;
    //


    // Agora definimos outros atributos dos vértices que serão interpolados pelo
    // rasterizador para gerar atributos únicos para cada fragmento gerado.

    // Posição do vértice atual no sistema de coordenadas global (World).
    position_world = model * model_coefficients;

    // Posição do vértice atual no sistema de coordenadas local do modelo.
    position_model = model_coefficients;

    // Normal do vértice atual no sistema de coordenadas global (World).
    // Veja slides 123-151 do documento Aula_07_Transformacoes_Geometricas_3D.pdf.
    normal = inverse(transpose(model)) * normal_coefficients;
    normal.w = 0.0;

    // Coordenadas de textura obtidas do arquivo OBJ (se existirem!)
    texcoords = texture_coefficients;

    // PARA INTERPOLACAO DE GOURAUD
    // o que estava no fragment shader deve estar aqui:

    if (object_id == BUNNY || object_id == BUNNY2 )
    {
        // Obtemos a posição da câmera utilizando a inversa da matriz que define o
        // sistema de coordenadas da câmera.
        vec4 origin = vec4(0.0, 0.0, 0.0, 1.0);
        vec4 camera_position = inverse(view) * origin;

        // O fragmento atual é coberto por um ponto que percente à superfície de um
        // dos objetos virtuais da cena. Este ponto, p, possui uma posição no
        // sistema de coordenadas global (World coordinates). Esta posição é obtida
        // através da interpolação, feita pelo rasterizador, da posição de cada
        // vértice.
        vec4 p = position_world;

        // Normal do fragmento atual, interpolada pelo rasterizador a partir das
        // normais de cada vértice.
        vec4 n = normalize(normal);

        // Vetor que define o sentido da fonte de luz em relação ao ponto atual.
        vec4 l = normalize(vec4(1.0,1.0,0.5,0.0));

        // Vetor que define o sentido da câmera em relação ao ponto atual.
        vec4 v = normalize(camera_position - p);

        // Vetor que define o sentido da reflexão especular ideal.
        vec4 r = -l + 2 * n * dot(n, l);    // Slide 115 das Aulas 17 e 18 - Modelos de Iluminação

        // Vetor no meio do caminho entre v e l (half-vector) para Blinn-Phong
        // h = (v+l)/||v+l||

        vec4 h = normalize(v+l);

        // Parâmetros que definem as propriedades espectrais da superfície
        vec3 Kd; // Refletância difusa
        vec3 Ks; // Refletância especular
        vec3 Ka; // Refletância ambiente
        float q_linha; // Expoente especular para o modelo de iluminação de Blinn-Phong
        
        // Coordenadas de textura U e V
        float U = 0.0;
        float V = 0.0;

        float minx = bbox_min.x;
        float maxx = bbox_max.x;

        float miny = bbox_min.y;
        float maxy = bbox_max.y;

        float minz = bbox_min.z;
        float maxz = bbox_max.z;

        float x_range = (maxx - minx);
        float y_range = (maxy - miny);

        float relative_x_position = (position_model.x - minx);
        float relative_y_position = (position_model.y - miny);

        U = relative_x_position / x_range;
        V = relative_y_position / y_range;

        vec3 Kd0 = texture(TextureImage4, vec2(U,V)).rgb; // white 

        // Propriedades espectrais do coelho
        //Kd = vec3(0.08,0.4,0.8);         // Refletância difusa no modelo RGB = (0.08, 0.4, 0.8)
        Ks = vec3(0.8,0.8,0.8);          // Refletância especular no modelo RGB = (0.8, 0.8, 0.8)
        Ka = Kd0 / 2;                     // Refletância ambiente no modelo RGB = metade da refletância difusa
        q_linha = 80.0;                      // Expoente especular de Phong = 32.0

        // Espectro da fonte de iluminação
        vec3 I = vec3(1.0,1.0,1.0); // PREENCHA AQUI o espectro da fonte de luz

        // Espectro da luz ambiente
        vec3 Ia = vec3(0.2,0.2,0.2); // PREENCHA AQUI o espectro da luz ambiente

        // Termo difuso utilizando a lei dos cossenos de Lambert
        // Aula 17 e 18 - Modelos de Iluminação - Slide 103
        // SUBSTITUI O Kd pelo Kd0 da textura
        vec3 lambert_diffuse_term = Kd0*I*max(0,dot(n,l)); // PREENCHA AQUI o termo difuso de Lambert

        // Termo ambiente
        // Slide 103
        vec3 ambient_term = Ka*Ia; // PREENCHA AQUI o termo ambiente
        
        // MODELO DE BLINN-PHONG - DIFERENTE:
        // Termo especular utilizando o modelo de iluminacao de Blinn-Phong:
        // Slide 150
        vec3 blinn_phong_specular_term  = Ks*I*pow(max(0,dot(n,h)),q_linha);
        
        // NOTE: Se você quiser fazer o rendering de objetos transparentes, é
        // necessário:
        // 1) Habilitar a operação de "blending" de OpenGL logo antes de realizar o
        //    desenho dos objetos transparentes, com os comandos abaixo no código C++:
        //      glEnable(GL_BLEND);
        //      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        // 2) Realizar o desenho de todos objetos transparentes *após* ter desenhado
        //    todos os objetos opacos; e
        // 3) Realizar o desenho de objetos transparentes ordenados de acordo com
        //    suas distâncias para a câmera (desenhando primeiro objetos
        //    transparentes que estão mais longe da câmera).
        // Alpha default = 1 = 100% opaco = 0% transparente
        color_bunny.a = 1;

        // Cor final do fragmento calculada com uma combinação dos termos difuso,
        // especular, e ambiente. Veja slide 129 do documento Aula_17_e_18_Modelos_de_Iluminacao.pdf.
        //color.rgb = lambert_diffuse_term + ambient_term + phong_specular_term;

        // OU - PARA BLINN-PHONG:
        color_bunny.rgb = lambert_diffuse_term + ambient_term + blinn_phong_specular_term;


        // Cor final com correção gamma, considerando monitor sRGB.
        // Veja https://en.wikipedia.org/w/index.php?title=Gamma_correction&oldid=751281772#Windows.2C_Mac.2C_sRGB_and_TV.2Fvideo_standard_gammas
        color_bunny.rgb = pow(color_bunny.rgb, vec3(1.0,1.0,1.0)/2.2);  
    }


    // PARA INTERPOLACAO DE GOURAUD
    // PARA GOURAUD, O QUE ESTÁ NO FRAGMENT SHADER VAI VIR PRA CÁ

    if (isBoundingBoxVertex)
    {
        gl_Position = projection * view * model * model_coefficients;
        return;  // Exit the vertex shader early; we don't need the rest for bounding box vertices
    }
}



