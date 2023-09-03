# INF01047-FCG-Trab-Final
Repositório para o Trabalho Final da disciplina de Fundamentos de Computação Gráfica

**Alunos:**<br/>
Laura Keidann Rodrigues da Silva - 217870<br/>
Matheus Henrique Sabadin - 228729<br/>

**Proposta inicial:** Será criado um programa no estilo do SketchUp em que será possível visualizar modelos, lançar algumas geometrias específicas em 2D a partir de linhas e pontos e extrudá-las ortogonalmente ao seu plano para formar uma geometria 3D.

**Proposta executada:** Programa em que é possível selecionar objetos, movimentá-los, inserir nova sinstâncias e analisar dados de seu centro e escala. Também é possível visualizar animações de luz e de objetos selecionados.

~Relatório em formato de README.md colocado no Github, contendo uma descrição simples sobre o desenvolvimento do trabalho, o qual deve obrigatoriamente incluir:<br/>~
**Descrição:**
    O objetivo inicial do trabalho era criar um programa no estilo do SketchUp em que será possível visualizar modelos, lançar algumas geometrias específicas em 2D a partir de linhas e pontos e extrudá-las ortogonalmente ao seu plano para formar uma geometria 3D.<br/>
    O trabalho foi desenvolvido de forma colaborativa, em constante diálogo. Para facilitar a elaboração, dividimos as tarefas na etapa de planejamento, de acordo com os interesses e habilidades de cada integrante, mas mantivemos constante troca de informações e atualizações, assim como discussões sobre os passos seguintes. <br/>
    Separamos os requisitos em metas, definimos prioridades de entrega (dividindo entre apresentação parcial e apresentação final), e fomos atualizando o progresso em cada item. <br/>
<br/>

~Parágrafo listando as contribuição de cada membro da dupla para o trabalho:<br/>~
~E Descrição de onde em sua aplicação você utilizou os conceitos de Computação Gráfica listado nos requisitos<br/>~
**Laura ficou responsável por implementar os seguintes requisitos:** <br/>
- Objetos da cena virtual (cow, bunny, plane, spheres, cube, e "rectangle");
- Modelos de Iluminacao Difusa (cuboides e plane) e Blinn-Phong (cow, bunny, spheres);
- Modelos de Interpolação de Phong (sphere, plane, cuboides, cow) e Gouraud (bunny);
- Mapeamento de texturas (5 images) em todos os objetos;
- Movimentação com curva Bézier cúbica para uma esfera e a fonte de luz;<br/>
<br/>

**Matheus ficou responsável pelos seguintes requisitos:** <br/>
- Transformações geométrica controladas pelo usuário: movimentação dos objetos;
- Câmera Livre e Câmera Look-at, possibilitando ao usuário alterar o tipo;
- Três tipos de testes de intersecção: testes raio-esfera, raio-bounding box e raio-triângulo;
- Animações baseadas no tempo: ao clicar em objeto com a funcionalidade de animação ativada, o usuário observa a animação na tela. <br/>
<br/>

**Além disso, Matheus implementou as seguintes funcionalidades extras, que faziam sentido para a aplicação desenvolvidas, assim como outras mudanças:** <br/>
- Seleção de objetos virtuais com o mouse (picking);
- Interface: janela para inserir instâncias novas dos objetos, janela com informações do objeto, checkboxes para alterar o tipo de projeção, desenhar raios do mouse ao clicar, exibir animações no clique de objetos selecionados e exibir animação na iluminação do modelo;
- Refatoração do código, modificação de estruturas de dados, criação de diversas classes, structs e arquivos novos. <br/>
<br/>

~Parágrafo curto indicando se a dupla fez uso do ChatGPT (ou alguma outra ferramenta similar, como Github Copilot, OpenAI Codex, etc.) para desenvolvimento do trabalho, descrevendo como a ferramenta foi utilizada e para quais partes do trabalho. O parágrafo deve também incluir uma análise crítica descrevendo quão útil a dupla achou a ferramenta, onde ela auxiliou e onde ela não auxiliou adequadamente;~ <br/>

**COPILOT:** <br/>
Foi feito uso do Copilot principalmente para ajudar no debug do código. Diversas vezes a úncia maneira de exibir as variáveis que estavam sendo executadas era mostrar seus valores no console ou em uma interface gráfica criada com essa intenção. O copilot ajudava muito ao transformar vetores em string ou exibir matrizes de vários elementos em loop, como por exemplo:<br/>
<br/>
Se digitava em uma função da GUI _**"// show SceneInformation::camera_movement"**_ e o copilot automaticamente preenchia uma linha de código que mostrava todos os dados da variável: <br/>
_**ImGui::Text("camera_movement: x=%.3f, y=%.3f, z=%.3f", SceneInformation::camera_movement.x, SceneInformation::camera_movement.y, SceneInformation::camera_movement.z);**_ <br/>
Esta interação ajudava muito para a criação de código repetitivo e de fácil erro na digitação. O copilot algumas vezes também ajudou na documentação em português do código.<br/>
<br/>
<br/>
**CHATGPT:** <br/>
Fizemos uso do ChatGPT e percebemos que sua melhor contribuição foi para resolver questões pontuais de problemas no código, mas mesmo assim com alguns equívocos iniciais. <br/>
<br/>
**Exemplos de pontos em que o ChatGPT foi útil:** <br/>
- Ajudou na implementação da curva de Bézier, após fornecermos explicações detalhadas e exemplo do nosso código. ChatGPT forneceu coordenadas para os pontos de forma a gerar a curva que desejávamos.
- Quando tentamos atualizar a fonte de iluminação a partir de um "in" no shader fragment, o ChatGPT respondeu que iria funcionar, mas não funcionou. Após, ele sugeriu a utilização de uma variável "uniform", que deu certo.
- Ajudou na implementação dos códigos de intersecção, explicando as equações utilziadas matematicamente, demonstrando o passo a passo do algoritmo e ajudando a adaptar interativamente as mesmas para exigências pontuais do progrmaa construído.
- Ajudou a bebugar problemas de alocação de memória relacionados aos VAOs e VBOs.<br/>
<br/>

**Exemplos de pontos em que o ChatGPT não foi útil:** <br/> 
- Não conseguiu ajudar a consertar o problema do cálculo das normais dos cuboides;
- Não conseguiu solucionar a implementação de iluminação junto com mapeamento de textura;
(Estes problemas foram solucionados com a ajuda do professor)
- Tinha muita dificuldade de encontrar problemas relacionados aos shaders, visto que precisava ver o contexto do código em c++ e o glsl
<br/>
<br/>
~Descrição do processo de desenvolvimento e do uso em sua aplicação dos conceitos de Computação Gráfica estudados e listados nos requisitos acima;~ <br/>
**Descrição do processo de desenvolvimento e do uso em sua aplicação dos conceitos de Computação Gráfica estudados:** <br/>
Iniciamos o desenvolvimento do trabalho a partir do código fonte do Laboratório 5. Além disso, consultamos os laboratórios anteriores e adaptamos os códigos para servir aos propósitos de nossa aplicação. Durante a elaboração, foi possível entender melhor os conceitos estudados na disciplina, em especial os requisitos que implementamos. Constantemente revisitamos os slides e as vídeo aulas para solucionar dúvidas e relembrar explicações. Muito do que foi visto de forma teórica passou a fazer mais sentido no momento da aplicação. Os próprios laboratórios foram ressignificados no contexto de uma aplicação que unia tudo o que havia sido implementado de forma isolada.<br/>
<br/>
Como já mencionamos anteriormente, na seção sobre as contribuições dos integrantes, os conceitos listados nos requisitos foram utilizados da forma que julgamos fazer mais sentido para o propósito da aplicação.

- No mínimo duas imagens mostrando o funcionamento da aplicação;

- Um manual descrevendo a utilização da aplicação (atalhos de teclado, etc.);
    Ao abrir a aplicação, o usuário vê a cena virtual com os objetos.

    Atalhos de teclado:
    - Teclas W, A, S, D: movimentação da câmera para frente (W), esquerda (A), trás (S), direita (D), respectivamente;
    - Teclas Q e E: movimentação da câmera para cima (Q) e para baixo (E), respectivamente;
    - Teclas E e Q: movimentação dos objetos ??? para cima e para baixo, respectivamente;
    - Setas para direita, esquerda, cima e baixo: movimentação de objetos selecionados;
    - ESC: fecha a janela;


    Movimentações e seleções com o mouse:
    - Botão esquerdo: seleção e movimentação de objetos;
    - Botão direito: pan da câmera; 
    - Rodinha pressionada: movimentação da câmera look-at;
    - Rodinha movimentada:  zoom in e out.

    Opções disponíveis na interface:
    - Add items to the scene: adicionar sphere, cuboid, cow, bunny, e plane através de seleção no menu;
    - Pick animation: checkbox para ativação de animação ao clicar em objeto;
    - Draw mouse ray: desenhar raio do mouse;
    - Show B. Boxes: ativar/desativar display das Bounding Boxes;
    - Perspective / Orthographic: alteração do tipo de projeção (Perspectiva ou Ortográfica);
    - Debug window? 
    - Object information ?



- Explicação de todos os passos necessários para compilação e execução da aplicação;



CRITERIOS TECNICOS (RESUMO):

- Malhas poligonais complexas - ok
- Transformacoes geometricas controladas pelo usuario - ok
- Camera Livre e Camera Look-at - ok 
- Instancias de objetos - ok
- Tres tipos de testes de interseccao
- Modelos de Iluminacao Difusa e Blinn-Phong - ok 
- Modelos de Interpolacao de Phong e Gouraud - ok
- Mapeamento de texturas em todos os objetos - ok 
- Movimentacao com curva Bezier cubica - ok
- Animacoes baseadas no tempo ($\Delta t$) - ok

Requisitos minimos - explicacao:

- interacao em tempo real
- objetivo e logica de controle nao-trivial
- Model matrix, projection matrix, view matrix
- Mouse e Teclado
- Objetos virtuais representados atraves de malhas poligonais complexas (malhas de triangulo):
    - no minimo de complexidade igual ou maior que o modelo cow.obj
    - carregar este e outros modelos geometricos no formato obj - utilizar bibliotecas existentes (por exemplo: tinyobjloader (C++) e tinyobjloader (C)).
    - maior variedade de modelos geometricos -> melhor
- Transformacoes geometricas:
    - Interacao com teclado e/ou mouse - controlar transformacoes geometricas aplicadas f OBJETOS VIRTUAIS
- Controle de Cameras virtuais:
    - no minimo uma camera look-ate uma camera livre (Lab2)
- UM (min) objeto virtual deve ser copiado com duas ou mais instâncias - utilizando duas ou mais Model matrix aplicadas ao mesmo conjunto de vértices
    - Ex. Lab 2 e Lab 3, onde o mesmo modelo geométrico (cubo) é utilizado para desenhar todas as partes do boneco, e somente as matrizes de modelagem (Model matrix) são alteradas para desenhar cada cópia do cubo.
- Testes de intersecção entre objetos virtuais - "collisions.cpp"
    - Três (min) tipos de teste de intersecção (por exemplo, um teste cubo-cubo, um teste cubo-plano, e um teste ponto-esfera).
    - com propósito 
- Modelos de iluminação de objetos geométricos:
    - Modelos de iluminação: difusa (Lambert) (cube, rectangle, plane) e Blinn-Phong (spheres, bunny, cow).
    - Modelos de interpolação para iluminação:
        - Modelo de Gouraud: o modelo de iluminação é avaliado para cada vértice usando suas normais, gerando uma cor, a qual é interpolada para cada pixel durante a rasterização.
        - Modelo de Phong: as normais de cada vértice são interpoladas para cada pixel durante a rasterização, e o modelo de iluminação é avaliado para cada pixel, utilizando estas normais interpoladas.
- Mapeamento de texturas.
    - TODOS objetos virtuais de sua aplicação devem ter suas cores definidas através de texturas representadas por imagens (no mínimo três imagens distintas).
    - Imagens de texturas "esticadas" de maneira não natural receberão desconto de pontuação.
Curvas de Bézier:
    - Um objeto virtual de sua aplicação deve ter sua movimentação definida através de uma curva de Bézier cúbica. O objeto deve se movimentar de forma suave ao longo do espaço em um caminho curvo (não reto).
- Animação de Movimento baseada no tempo:
    - Todas as movimentações de objetos (incluindo da câmera) devem ser computadas baseado no tempo (isto é, movimentações devem ocorrer sempre na mesma velocidade independente da velocidade da CPU onde o programa está sendo executado).
