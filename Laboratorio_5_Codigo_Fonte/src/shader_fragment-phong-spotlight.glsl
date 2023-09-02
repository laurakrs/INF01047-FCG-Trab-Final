#version 330 core

// Atributos de fragmentos recebidos como entrada ("in") pelo Fragment Shader.
// Neste exemplo, este atributo foi gerado pelo rasterizador como a
// interpolação da posição global e a normal de cada vértice, definidas em
// "shader_vertex.glsl" e "main.cpp".
in vec4 position_world;
in vec4 normal;

// Posição do vértice atual no sistema de coordenadas local do modelo.
in vec4 position_model;

// Coordenadas de textura obtidas do arquivo OBJ (se existirem!)
in vec2 texcoords;

// Matrizes computadas no código C++ e enviadas para a GPU
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

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

// O valor de saída ("out") de um Fragment Shader é a cor final do fragmento.
out vec4 color;

// Constantes
#define M_PI   3.14159265358979323846
#define M_PI_2 1.57079632679489661923

void main()
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
    //float U = 0.0;
    //float V = 0.0;

    if ( object_id == SPHERE || object_id == CENTRAL_SPHERE || object_id == SPHERE2 )
    {
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

        vec4 bbox_center = (bbox_min + bbox_max) / 2.0;

        //U = 0.0;
        //V = 0.0;

        // ILUMINACAO DIFUSA
        // Propriedades espectrais da esfera
        Kd = vec3(0.8,0.4,0.08);        // Refletância no modelo RGB = (0.8, 0.4, 0.08)
        Ks = vec3(0.0,0.0,0.0);         // Superfície 100% difusa
        Ka = Kd / 2;                    // Refletância ambiente no modelo RGB = metade da refletância difusa
        q = 1.0;                        // Expoente especular de Phong não especificado
        //q_linha = 1.0;
    }
    else if ( object_id == BUNNY || object_id ==  BUNNY2 )
    {
        // PREENCHA AQUI as coordenadas de textura do coelho, computadas com
        // projeção planar XY em COORDENADAS DO MODELO. Utilize como referência
        // o slides 99-104 do documento Aula_20_Mapeamento_de_Texturas.pdf,
        // e também use as variáveis min*/max* definidas abaixo para normalizar
        // as coordenadas de textura U e V dentro do intervalo [0,1]. Para
        // tanto, veja por exemplo o mapeamento da variável 'p_v' utilizando
        // 'h' no slides 158-160 do documento Aula_20_Mapeamento_de_Texturas.pdf.
        // Veja também a Questão 4 do Questionário 4 no Moodle.

        float minx = bbox_min.x;
        float maxx = bbox_max.x;

        float miny = bbox_min.y;
        float maxy = bbox_max.y;

        float minz = bbox_min.z;
        float maxz = bbox_max.z;

        //U = 0.0;
        //V = 0.0;

        // Propriedades espectrais do coelho
        Kd = vec3(0.08,0.4,0.8);         // Refletância difusa no modelo RGB = (0.08, 0.4, 0.8)
        Ks = vec3(0.8,0.8,0.8);          // Refletância especular no modelo RGB = (0.8, 0.8, 0.8)
        Ka = Kd / 2;                     // Refletância ambiente no modelo RGB = metade da refletância difusa
        q = 32.0 
        //q_linha = 80.0;                      // Expoente especular de Phong = 32.0
    }
    else if( object_id == COW )
    {
        // as coordenadas de textura da vaca
        // IGUAL AO COELHO POR ENQUANTO
        float minx = bbox_min.x;
        float maxx = bbox_max.x;

        float miny = bbox_min.y;
        float maxy = bbox_max.y;

        float minz = bbox_min.z;
        float maxz = bbox_max.z;

        //U = 0.0;
        //V = 0.0;

        // Propriedades espectrais da vaca
        Kd = vec3(0.08,0.4,0.8);         // Refletância difusa no modelo RGB = (0.08, 0.4, 0.8)
        Ks = vec3(0.8,0.8,0.8);          // Refletância especular no modelo RGB = (0.8, 0.8, 0.8)
        Ka = Kd / 2;                     // Refletância ambiente no modelo RGB = metade da refletância difusa
        q = 32.0;                        // Expoente especular de Phong = 32.0
       // q_linha = 80.0;

    }
    else if( object_id == CUBE )
    {
        // as coordenadas de textura do cubo
        // IGUAL AO COELHO POR ENQUANTO
        float minx = bbox_min.x;
        float maxx = bbox_max.x;

        float miny = bbox_min.y;
        float maxy = bbox_max.y;

        float minz = bbox_min.z;
        float maxz = bbox_max.z;

        //U = 0.0;
        //V = 0.0;

        // Propriedades espectrais do cubo
        Kd = vec3(0.8,0.4,0.08);        // Refletância no modelo RGB = (0.8, 0.4, 0.08)
        Ks = vec3(0.0,0.0,0.0);         // Superfície 100% difusa
        Ka = Kd / 2;                    // Refletância ambiente no modelo RGB = metade da refletância difusa
        q = 1.0;                        // Expoente especular de Phong não especificado
        //q_linha = 1.0;

    }
    else if( object_id == RECTANGLE )
    {
        // as coordenadas de textura do retangulo
        // IGUAL AO COELHO POR ENQUANTO
        float minx = bbox_min.x;
        float maxx = bbox_max.x;

        float miny = bbox_min.y;
        float maxy = bbox_max.y;

        float minz = bbox_min.z;
        float maxz = bbox_max.z;

        //U = 0.0;
        //V = 0.0;

        // Propriedades espectrais do retangulo
        Kd = vec3(0.8,0.4,0.08);        // Refletância no modelo RGB = (0.8, 0.4, 0.08)
        Ks = vec3(0.0,0.0,0.0);         // Superfície 100% difusa
        Ka = Kd / 2;                    // Refletância ambiente no modelo RGB = metade da refletância difusa
        q = 1.0;                        // Expoente especular de Phong não especificado
        //q_linha = 1.0;
    }
    else if ( object_id == PLANE )
    {
        // Coordenadas de textura do plano, obtidas do arquivo OBJ.
        //U = texcoords.x;
        //V = texcoords.y;

        // Propriedades espectrais do plano
        Kd = vec3(0.2,0.2,0.2);         // Refletância difusa no modelo RGB = (0.2, 0.2, 0.2)
        Ks = vec3(0.3,0.3,0.3);         // Refletância especular no modelo RGB = (0.3, 0.3, 0.3)
        Ka = vec3(0.0,0.0,0.0);         // Refletância ambiente no modelo RGB = zero.
        q = 20.0;                       // Expoente especular de Phong = 20.0
        //q_linha = 20.00;
    }
    else // Objeto desconhecido = preto
    {
        Kd = vec3(0.0,0.0,0.0);
        Ks = vec3(0.0,0.0,0.0);
        Ka = vec3(0.0,0.0,0.0);
        q = 1.0;
        //q_linha = 1.0;
    }


    // Espectro da fonte de iluminação
    vec3 I = vec3(1.0,1.0,1.0); // PREENCHA AQUI o espectro da fonte de luz

     // Propriedades da iluminaão spotligh no slide 204 das aulas 17 e 18 - Modelos de Iluminação
    vec4 spotlight_position_l = vec4(0.0f,2.0f,1.0f,1.0f);     // Luz posicionada em (0.0,2.0,1.0)
    vec4 spotlight_direction_v = vec4(0.0f,-1.0f,0.0f,0.0f);   // Com vetor de sentido (0.0,-1.0,0.0)
    float spotlight_opening_angle_alpha = radians(30.0f);      // Luz do tipo spotlight, com ângulo de abertura de 30°

    float cos_beta = dot(normalize(p - spotlight_position_l), normalize(spotlight_direction_v));

    if (cos_beta < cos(spotlight_opening_angle_alpha))         // Slide 211 e 212
    {
        I = vec3(0.0,0.0,0.0);                                 // Zera o espectro da fonte de luz
    }

    // Espectro da luz ambiente
    vec3 Ia = vec3(0.2,0.2,0.2);    // Fonte de luz ambiente com espectro no modelo RGB = (0.2, 0.2, 0.2)

    // Termo difuso utilizando a lei dos cossenos de Lambert
    float clamping_lambert_diffuse = max(0, dot(n,l));                      // Slide 102 das Aulas 17 e 18 - Modelos de Iluminação
    vec3 lambert_diffuse_term = Kd * I * clamping_lambert_diffuse;          // Slide 102 das Aulas 17 e 18 - Modelos de Iluminação

    // Termo ambiente
    vec3 ambient_term = Ka * Ia;                                            // Slide 104 das Aulas 17 e 18 - Modelos de Iluminação

    // Termo especular utilizando o modelo de iluminação de Phong
    float clamping_phong_specular = max(0, dot(r,v));                       // Slide 129 das Aulas 17 e 18 - Modelos de Iluminação
    vec3 phong_specular_term  = Ks * I * pow(clamping_phong_specular, q);   // Slide 129 das Aulas 17 e 18 - Modelos de Iluminação


    // MODELO DE BLINN-PHONG - DIFERENTE:
    // Termo especular utilizando o modelo de iluminacao de Blinn-Phong:
    // Slide 150
    //vec3 blinn_phong_specular_term  = Ks*I*pow(max(0,dot(n,h)),q_linha);


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
    color.rgb = lambert_diffuse_term + ambient_term + phong_specular_term;



    // Cor final com correção gamma, considerando monitor sRGB.
    // Veja https://en.wikipedia.org/w/index.php?title=Gamma_correction&oldid=751281772#Windows.2C_Mac.2C_sRGB_and_TV.2Fvideo_standard_gammas
    color.rgb = pow(color.rgb, vec3(1.0,1.0,1.0)/2.2);
}










