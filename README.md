# :gem: FOCUS - Facial OpenCV Unearthing System

- ### [:dart: Objetivo](#dart-objetivo-1)
- ### [:apple: Diagrama de Classes](#apple-diagrama-de-classes-1)
- ### [:cut_of_meat: Dependências](#cut_of_meat-dependências-1)
- ### [:zombie: Como rodar](#zombie-como-rodar-1)
- ### [:arrow_down: Baixar o projeto](https://github.com/E-DUMAL/FOCUS/archive/refs/heads/main.zip)

<!-- Atualizar links acima -->

## Disciplina de Linguagem de Programação I e Laboratório

Esse foi um projeto desenvolvido por discentes do curso de *Engenharia da Computação da Universidade Federal da Paraíba*, curso este que pertence ao *[Centro de Informática](http://ci.ufpb.br/)*, localizado na *[Rua dos Escoteiros S/N - Mangabeira - João Pessoa - Paraíba - Brasil](https://g.co/kgs/xobLzCE)*. O programa decorrente do desenvolvimento do código foi empregado com o objetivo de permitir ao docente, utilizando de métodos avaliativos, calcular e atribuir as respectivas notas dos participiantes do projeto. 

### :deciduous_tree: Autores:

-  :pick:  *[Eduardo Asfuri Carvalho](https://github.com/Asfuri)*
-  :axe:  *[Lucas Henrique Vieira da Silva](https://github.com/hvslucas)*
-  :dagger:  *[Marco Antonio de Vasconcelos Souza Filho](https://github.com/MarcoFilho1)*

###  :deciduous_tree: Docente:

-  	:bow_and_arrow: *[Derzu Omaia](https://github.com/derzu)*

[![wallpaperflare com_wallpaper](https://github.com/user-attachments/assets/0c1610d9-f5bd-4bd7-90b6-7613d0beb32b)](#gem-focus---facial-opencv-unearthing-system)

<!-- Colocar link na imagem -->

## :dart: Objetivo:

O projeto tem como objetivo desenvolver um jogo utilizando uma biblioteca de Visão Computacional em C++ que consiga proporcionar uma boa jogabilidade a partir de entradas da câmera do usuário. O jogo deve ter como requisito:[^1][^2] 

[^1]: ***[C++](https://www.w3schools.com/cpp/cpp_intro.asp)***

[^2]: ***[Visão Computacional](https://www.ibm.com/br-pt/topics/computer-vision)***

1. Utilizar classes e desenvolver o [Diagrama de Classes](#apple-diagrama-de-classes-1)
2. Rodar indefinidamente até que o usuário resolvar sair
3. Possuir um menu
4. Exibir a maior pontuação, a pontuação atual e sempre salvar a maior pontuação
5. Utilizar de uma Biblioteca de Visão Computacional ( OpenCV, Yolo ou OpenPose para detectar o jogador )
6. Possuir Jogabilidade/Usabilidade
7. Tocar sons de efeito

### Temática

Resolvemos desenvolver o nosso jogo baseado na temática do [Minecraft](https://pt.wikipedia.org/wiki/Minecraft)[^3][^4], mas com a perspectiva disponível (2D). O jogador deve tentar pegar o maior número de minérios, que concedem pontuações diferentes de acordo com sua raridade, enquanto evita ao máximo as TNTs, que reduzem sua pontuação.

Pontuação:

- Esmeralda:  +22 pontos
- Diamante:  +19 pontos
- Ouro:  +17 pontos
- Redstone:  +14 pontos
- Lápis-lazúli:  +12 pontos
- Ferro:  +10 pontos
- Cobre:  +7 pontos
- Carvão:  +5 pontos
- Dinamite:  -100 pontos

[^3]: ***[Minecraft](https://pt.wikipedia.org/wiki/Minecraft)***

[^4]: Todos os direitos de imagens e recursos do jogo pertencem a Mojang Studios. 
Nenhuma cópia deste jogo foi distribuída por meio de venda.
Não recomendamos a reutilização das imagens aqui utilizadas

![icons8-minecraft](https://github.com/user-attachments/assets/bcd1373c-9cde-42af-a64c-3358963fc586)

## :apple: Diagrama de Classes

![Diagrama de Classes](https://github.com/user-attachments/assets/0c7c16ec-8175-4caa-b09e-4ae48a3fa319)

## :cut_of_meat: Dependências

Este projeto foi desenvolvido utilizando funcionalidades da biblioteca padrão do C++[^5] e de duas bibliotecas externas, sendo elas a biblioteca OpenCV[^6] e a biblioteca SFML[^7]. Abaixo, dissertamos sobre a utilidade de cada uma das bibliotecas e exemplos de recursos utilizados dentro do projeto:

### Biblioteca Padrão[^5]

[^5]: ***[Biblioteca Padrão do C++](https://en.cppreference.com/w/cpp/header)***

- **`<iostream>`**: Usada para operações básicas de entrada e saída, como leitura de dados do teclado (`std::cin`) e escrita de dados na tela (`std::cout`).
  - **Implementação:** Por não precisar de entradas via terminal, foi utilizada somente para exibição de estados no terminal.

- **`<vector>`**: Fornece a classe `std::vector`, facilitando armazenar e manipular coleções de dados dinâmicos em formato de lista. A classe `std::vector` permite criar arrays dinâmicos que podem redimensionar automaticamente à medida que novos elementos são adicionados, que tornam sua utilização, a priori, intuitiva com os métodos da classe.
  - **Implementação:** Para organizar os sons, matrizes de canais, frames, etc.

- **`<fstream>`**: Fornece as classes para leitura e escrita em arquivos, como `std::fstream`, e seus respectivos métodos.
  - **Implementação:** Ao ler e escrever os dados em arquivos `.txt`, ou seja, é primordial para o salvamento dos dados de pontução, ao utilizar os métodos `.open()` e `.close` para a disposição do arquivo.

- **`<chrono>`**: Oferece funcionalidades para medição de tempo, temporizadores, e manipulação de intervalos de tempo, como o método `chrono::steady_clock::now();` que é capaz de retornar o tempo.
  - **Implementação:** Utilizada para pegar o tempo atual durante a execução do programa e contar o tempo decorrido.
 
- **`<thread>`**: Suporte à execução paralela de código com múltiplas threads, facilitando a execução não concorrente do programa.
  - **Implementação:** Para tocar múltiplos sons ao mesmo tempo, no caso de múltipla interação do usuário.

### Biblioteca OpenCV - Open Source Computer Vision Library[^6] 

[^6]: ***[Biblioteca OpenCV](https://opencv.org/about/)***

A biblioteca OpenCV foi escolhida para ser a biblioteca responsável por captar e interpretar as informações vindas da câmera do usuário, sendo os componentes utilizados no projeto:

- **`<opencv2/core.hpp>`**: Fornece funções fundamentais de manipulação de matrizes e operações matemáticas.
- **`<opencv2/highgui.hpp>`**: Responsável pela exibição de imagens e vídeos, além de criar interfaces gráficas.
- **`<opencv2/imgproc.hpp>`**: Usada para processamento de imagens, como filtragem, transformações, e detecção de bordas.
- **`<opencv2/objdetect.hpp>`**: Realiza detecção de objetos, como faces, corpos, ou outros padrões.
- **`<opencv2/videoio.hpp>`**: Lida com captura e gravação de vídeos, acessando dispositivos de captura como câmeras.
 
### Biblioteca SFML - Simple and Fast Multimedia Library[^7]

[^7]: ***[Biblioteca SFML](https://www.sfml-dev.org/)***

A biblioteca SFML foi escolhida para ser responsável pela reprodução de áudios (efeitos sonoros) no projeto. A decisão de utilizá-la foi tomada principalmente por sua capacidade de reproduzir áudios de maneira paralela (não bloqueante), o que a torna uma escolha preferível em relação à biblioteca de áudio do sistema operacional. Por ser uma biblioteca leve e importante, a mesma está importada dentro do projeto para garantir uma reprodução de áudio eficiente e sem sobrecarregar o desempenho do sistema.

Seu componente de inclusão:

- **`<SFML/Audio.hpp>`**: Utilizada para manipulação de áudio, incluindo reprodução de sons e música.

## :zombie: Como rodar

***Requisitos***
- Um compilador c++, recomendamos o `clang++` ou o `g++`
- Um terminal de linha de comando

[**Atenção:** Lembre de baixar o projeto e extraí-lo devidamente do `.zip`.](#medal_sports-projeto-crud---jogos-olímpicos)

### Bash

Utilizando `g++` para compilar

```sh
cd /path/to/Projeto-CRUD
g++ main.cpp -I include/ src/*.cpp -o olimpiadas.out
```

Utilizando `clang++` para compilar

```sh
cd /path/to/Projeto-CRUD
clang++ main.cpp -I include/ src/*.cpp -o olimpiadas.out
```

Para rodar

```sh
./olimpiadas.out
```

### No Windows

Utilizando `g++` para compilar

```sh
cd /path/to/Projeto-CRUD
g++ main.cpp -I include/ src/*.cpp -o olimpiadas.exe
```

Utilizando `clang++` para compilar

```sh
cd /path/to/Projeto-CRUD
clang++ main.cpp -I include/ src/*.cpp -o olimpiadas.exe
```

Para rodar

```sh
./olimpiadas.exe
```

### Atenção
Para warnings referentes a codificação, recomendamos o uso da seguinte flag na compilação (válido para clang++)
```sh
-Wno-invalid-source-encoding
```

**OBS.:** Utilizamos de barra normal ('/') considerando um ambiente como Git Bash, WSL e PowerShell, considere utilizar de barra invertida ('\\') em caso de não compilar
