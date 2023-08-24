# INF01047-FCG-Trab-Final
Repositório para o Trabalho Final da disciplina de Fundamentos de Computação Gráfica

Proposta: Será criado um programa no estilo do SketchUp em que será possível visualizar modelos, lançar algumas geometrias específicas em 2D a partir de linhas e pontos e extrudá-las ortogonalmente ao seu plano para formar uma geometria 3D.

Tarefas futuras:
Relatório em formato de README.md colocado no Github, contendo uma descrição simples sobre o desenvolvimento do trabalho, o qual deve obrigatoriamente incluir:
- Parágrafo listando as contribuição de cada membro da dupla para o trabalho;
- Parágrafo curto indicando se a dupla fez uso do ChatGPT (ou alguma outra ferramenta similar, como Github Copilot, OpenAI Codex, etc.) para desenvolvimento do trabalho, descrevendo como a ferramenta foi utilizada e para quais partes do trabalho. O parágrafo deve também incluir uma análise crítica descrevendo quão útil a dupla achou a ferramenta, onde ela auxiliou e onde ela não auxiliou adequadamente;
- Descrição do processo de desenvolvimento e do uso em sua aplicação dos conceitos de Computação Gráfica estudados e listados nos requisitos acima;
- No mínimo duas imagens mostrando o funcionamento da aplicação;
- Um manual descrevendo a utilização da aplicação (atalhos de teclado, etc.);
- Explicação de todos os passos necessários para compilação e execução da aplicação;

CRITERIOS TECNICOS (RESUMO):

- Malhas poligonais complexas - ok
- Transformacoes geometricas controladas pelo usuario - ok
- Camera Livre e Camera Look-at - ok 
- Instancias de objetos - ok
- Tres tipos de testes de interseccao
- Modelos de Iluminacao Difusa e Blinn-Phong - +/-
- Modelos de Interpolacao de Phong e Gouraud
- Mapeamento de texturas em todos os objetos - ok 
- Movimentacao com curva Bezier cubica
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
    - Modelos de iluminação: difusa (Lambert) e Blinn-Phong.
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
