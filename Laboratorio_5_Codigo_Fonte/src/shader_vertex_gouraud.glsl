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

// Atributos de vértice que serão gerados como saída ("out") pelo Vertex Shader.
// ** Estes serão interpolados pelo rasterizador! ** gerando, assim, valores
// para cada fragmento, os quais serão recebidos como entrada pelo Fragment
// Shader. Veja o arquivo "shader_fragment.glsl".
out vec4 position_world;
out vec4 position_model;
out vec4 normal;
out vec2 texcoords;

// para a esfera => Gouraud
out vec3 color_sphere;


// Identificador que define qual objeto está sendo desenhado no momento
#define CENTRAL_SPHERE 0
#define SPHERE 1
#define SPHERE2 2
#define BUNNY  3
#define BUNNY2 4
#define PLANE  5
#define COW    6
#define CUBE   7
#define RECTANGLE 8

uniform int object_id;

// Parâmetros da axis-aligned bounding box (AABB) do modelo
uniform vec4 bbox_min;
uniform vec4 bbox_max;

// Variáveis para acesso das imagens de textura
uniform sampler2D TextureImage0;
uniform sampler2D TextureImage1;
uniform sampler2D TextureImage2;
uniform sampler2D TextureImage3;
uniform sampler2D TextureImage4;
uniform sampler2D TextureImage5;

// Constantes
#define M_PI   3.14159265358979323846
#define M_PI_2 1.57079632679489661923


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
    // PARA GOURAUD, O QUE ESTÁ NO FRAGMENT SHADER VAI VIR PRA CÁ

    if ( object_id == SPHERE || object_id == CENTRAL_SPHERE || object_id == SPHERE2 )
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
        float q; // Expoente especular para o modelo de iluminação de Phong
        float q_linha; // Expoente especular para o modelo de iluminação de Blinn-Phong
        
        
        // Coordenadas de textura U e V
        float U = 0.0;
        float V = 0.0;


        vec3 Kd0 = texture(TextureImage0, vec2(U,V)).rgb;

        // PREENCHA AQUI as coordenadas de textura da esfera, computadas com
        // projeção esférica EM COORDENADAS DO MODELO. Utilize como referência
        // o slides 134-150 do documento Aula_20_Mapeamento_de_Texturas.pdf.
        // A esfera que define a projeção deve estar centrada na posição
        // "bbox_center" definida abaixo.

        // Você deve utilizar:
        //   função 'length( )' : comprimento Euclidiano de um vetor
        //   função 'atan( , )' : arcotangente. Veja https://en.wikipedia.org/wiki/Atan2.
        //   função 'asin( )'   : seno inverso.
        //   constante M_PI
        //   variável position_model

        // TEXTURA
        vec4 bbox_center = (bbox_min + bbox_max) / 2.0;

        // Slide 150 da Aula 20 - Mapeamento de Texturas
        vec4 p = position_model - bbox_center;
        float theta = atan(p.x, p.z);       // Range: [-PI, PI)
        float phi = asin(p.y / length(p));  // Range: [-PI/2, PI/2)

        U = (theta + M_PI) / (2 * M_PI);    // Range: [0,1)
        V = (phi + M_PI / 2) / M_PI;        // Range: [0, 1)

        Kd0 = texture(TextureImage0, vec2(U,V)).rgb; // planet

        // ILUMINACAO DIFUSA
        // Propriedades espectrais da esfera
        Kd = vec3(0.8,0.4,0.08);        // Refletância no modelo RGB = (0.8, 0.4, 0.08)
        Ks = vec3(0.0,0.0,0.0);         // Superfície 100% difusa
        Ka = Kd / 2;                    // Refletância ambiente no modelo RGB = metade da refletância difusa
        q = 1.0;                        // Expoente especular de Phong não especificado
        q_linha = 1.0;


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

        // Termo especular utilizando o modelo de iluminação de Phong
        // Slide 128
        vec3 phong_specular_term  = Ks*I*pow(max(0,dot(r,v)),q); // PREENCHA AQUI o termo especular de Phong

        // MODELO DE BLINN-PHONG - DIFERENTE:
        // Termo especular utilizando o modelo de iluminacao de Blinn-Phong:
        // Slide 150
        vec3 blinn_phong_specular_term  = Ks*I*pow(max(0,dot(n,h)),q_linha);


        // ANTES - NO ARQUIVO ORIGINAL DO LAB 5
        // Obtemos a refletância difusa a partir da leitura da imagem TextureImage0
        // vec3 Kd0 = texture(TextureImage0, vec2(U,V)).rgb;

        // Equação de Iluminação
        // float lambert = max(0,dot(n,l));

        // color.rgb = Kd0 * (lambert + 0.01);

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
        color.a = 1;

        // Cor final do fragmento calculada com uma combinação dos termos difuso,
        // especular, e ambiente. Veja slide 129 do documento Aula_17_e_18_Modelos_de_Iluminacao.pdf.
        //color.rgb = lambert_diffuse_term + ambient_term + phong_specular_term;

        // OU - PARA BLINN-PHONG:
        color_sphere = lambert_diffuse_term + ambient_term + blinn_phong_specular_term;



        // Cor final com correção gamma, considerando monitor sRGB.
        // Veja https://en.wikipedia.org/w/index.php?title=Gamma_correction&oldid=751281772#Windows.2C_Mac.2C_sRGB_and_TV.2Fvideo_standard_gammas
        color_sphere = pow(color.rgb, vec3(1.0,1.0,1.0)/2.2);


    }

    


}



