Para executar é preciso ter o OpenGL instalado (sudo apt-get install freeglut3-dev) e rodar o executavel, ou compilar com (g++ source.c -o executavel -lGL -lGLU -lglut && ./executavel).

Jogo feito como trabalho final da disciplina de Computação Gráfica.

O objetivo do jogo é pegar as três caixas menores no chão e colocá-las em cima da caixa maior em frente, utilizando o braço robótico.

Movimenta-se a posição do seu ponto de vista, a 'câmera' pela qual você olha o ambiente, utilizando o mouse: clicando e arrastando com o botão esquerdo, move-se o ponto de vista em volta do centro do ambiente, clicando e arrastando com o botão direito, aproxima-se ou afasta-se o zoom;

Movimenta-se o braço robótico pelo ambiente utilizando as teclas 'w', 'a', 's' e 'd', representando respectivamente para frente, para trás, para a esquerda e para a direita;

A tecla 1 permite a rotação do braço robô em volta do seu eixo no sentido horário, shift+1 para movimentar no sentido anti-horário;
A tecla 2 movimenta a articulação do ombro para a frente, shift+2 para movimentar para trás;
A tecla 3 movimenta a articulação do cotovelo para a frente, shift+3 para movimentar para trás;
A tecla 4 fecha a garra do robô, pegando as caixas caso elas estejam ao alcance da garra, shift+4 para abrir a garra, soltando-se a caixa;

Divirta-se com o desafio!
