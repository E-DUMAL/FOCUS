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

O projeto tem como objetivo desenvolver um jogo utilizando uma biblioteca Visão Computacional em C++ que consiga proporcionar uma boa jogabilidade a partir de entradas da câmera do usuário. O jogo deve ter como requisito:[^1][^2] 

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

Resolvemos desenvolver o nosso jogo se baseando na temática do [Minecraft](https://pt.wikipedia.org/wiki/Minecraft)[^3][^4], mas com a perspectiva disponível (2D). O jogador deve tentar pegar o maior número de minérios, que concedem pontuações diferentes de acordo com sua raridade, enquanto evita ao máximo as TNTs, que reduzem sua pontuação.

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


## :cut_of_meat: Dependências

Este projeto foi desenvolvido utilizando apenas funcionalidades da biblioteca padrão do C++ e do sistema operacional[^3][^4], ou seja, nenhuma biblioteca externa foi incluída no código. Abaixo, dissertamos sobre a utilidade de cada uma das funcionalidades da biblioteca padrão implementadas, e exemplos de sua utilidade dentro do projeto:

[^3]: ***[Biblioteca Padrão do C++](https://en.cppreference.com/w/cpp/header)***
[^4]: ***[Sistema Operacional](https://tecnoblog.net/responde/o-que-e-um-sistema-operacional/)***

### Biblioteca Padrão

- **`<iostream>`**: Usada para operações básicas de entrada e saída, como leitura de dados do teclado (`std::cin`) e escrita de dados na tela (`std::cout`).
  - **Implementação:** Em interações direta com um usuário, via terminal.

- **`<string>`**: Fornece suporte à manipulação de strings, permitindo o uso da classe `std::string` para trabalhar com cadeias de caracteres de maneira mais conveniente a partir dos métodos fornecidos pela classe.
  - **Implementação:** Em validações de entradas do tipo `std::string` pelo usuário, em comparações utilizando `.find()` e em conversões entre tipos utilizando dos métodos `.toString()` e `.stoi()`.

- **`<vector>`**: Fornece a classe `std::vector`, facilitando armazenar e manipular coleções de dados dinâmicos em formato de lista. A classe `std::vector` permite criar arrays dinâmicos que podem redimensionar automaticamente à medida que novos elementos são adicionados, que tornam sua utilização, a priori, intuitiva com os métodos da classe.
  - **Implementação:** Em compor o Objeto de GerencimentoOlimpada com dois arrays dinâmicos primordiais para o funcionamento de todo o CRUD, que teve como auxílio métodos como `push_back()` e `erase()`

- **`<fstream>`**: Fornece as classes para leitura e escrita em arquivos, como `std::fstream`, e seus respectivos métodos.
  - **Implementação:** Ao ler e escrever os dados em arquivos `.txt`, ou seja, é primordial para a execução sequencial do programa, utilizando os métodos `.open()` e `.close` para a disposição do arquivo.

- **`<limits>`**: Fornece informações sobre os limites das variáveis numéricas (como o valor máximo e mínimo que um tipo de dado pode armazenar) usando a classe `std::numeric_limits`.
  - **Implementação:** Ao tratar as entradas númericas do usuário, como ao utilizar o código `std::numeric_limits<std::streamsize>::max()` para receber no `cin.ignore()` valores irrelevantes.

- **`<algorithm>`**: Inclui uma variedade de algoritmos genéricos, como ordenação (`std::sort`), busca (`std::find`), entre outros, que podem ser aplicados a contêineres como vetores.
  - **Implementação:** Ao converter os ìndices (`char`) de uma determinada `std::string` em minúsculo utilizando do método `std::transform`.

- **`<locale>`**: Usada para trabalhar com diferentes configurações regionais, como formatos de números, datas, e outros parâmetros que podem variar conforme a localidade. A classe `std::locale` permite adaptar o comportamento do programa ao ambiente cultural do usuário.
  - **Implementação:** Ao formatar todo nosso código para o idioma Português brasileiro, para, assim, utilizar de acentos e caracteres especifícos do idioma, como `ç, ~`, utilizamos do método `std::setlocale()`.

### Bibliotecas Específicas de Plataforma
- O uso de `<unistd.h>` e `<windows.h>` depende da plataforma em que o programa está sendo compilado.

- **`<unistd.h>`** (Linux/Unix): Usada para acessar várias funcionalidades do sistema operacional POSIX, como chamadas de sistema para manipulação de arquivos, gerenciamento de processos, e manipulação de diretórios. Esta biblioteca é exclusiva para sistemas Unix-like.
  - **Implementação:** Ao decorar o terminal com uma pequena pausa utilizando do método `.sleep()`
  
- **`<windows.h>`** (Windows): Inclui funcionalidades específicas para o sistema operacional Windows, como manipulação de janelas, interação com hardware, gerenciamento de processos e threads, e muito mais. Esta biblioteca é exclusiva para o sistema operacional Windows.
  - **Implementação:** Ao decorar o terminal com uma pequena pausa utilizando do método `.sleep()`

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
