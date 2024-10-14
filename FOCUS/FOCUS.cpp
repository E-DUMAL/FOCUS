/*
	Se��o Inclus�o:

	- Inclus�o de Bibliotecas e Deped�ncias
	- Definido o namespace utilizado
	- Constantes e v�riaveis de controle

*/

// OpenCV - Biblioteca de vis�o computacional para processamento de imagem e v�deo
#include "opencv2/core.hpp"       // Fun��es fundamentais, manipula��o de matrizes e opera��es matem�ticas
#include "opencv2/highgui.hpp"    // Exibi��o de imagens e v�deos, interface gr�fica com o usu�rio
#include "opencv2/imgproc.hpp"    // Processamento de imagem, filtragem, transforma��es, detec��o de bordas
#include "opencv2/objdetect.hpp"  // Detec��o de objetos como faces, corpos, ou outros padr�es
#include "opencv2/videoio.hpp"    // Captura e grava��o de v�deos, acesso a dispositivos de captura (c�meras)

// SFML - Biblioteca multim�dia para manipula��o de �udio
#include <SFML/Audio.hpp>         // Manipula��o de �udio, como reprodu��o de sons e m�sica

// Bibliotecas padr�o do C++ - Utilizadas para v�rias opera��es no c�digo
#include <chrono>                 // Medi��o e manipula��o de tempo, temporizadores e intervalos
#include <fstream>                // Opera��es de entrada e sa�da de arquivos
#include <iostream>               // Opera��es b�sicas de entrada e sa�da no terminal
#include <thread>                 // Suporte � execu��o de c�digo em m�ltiplas threads
#include <vector>                 // Estrutura de dados din�mica (vetor) para armazenar cole��es de itens


using namespace std;  // Namespace da biblioteca padr�o C++ (std)
using namespace cv;   // Namespace OpenCV (cv)


int highScore = INT_MIN;	// Inicializa a vari�vel "Maior Score" com o menor valor poss�vel dos inteiros
bool isSaved = false;		// Inicializa a vari�vel de controle para saber se o recorde est� salvo
int Explode = 0;			// Inicializa a vari�vel de controle para mostrar as particulas
std::chrono::steady_clock::time_point explodeTime;// Inicializa a vari�vel de tempo da explos�o

/*
	Se��o de Classes:
*/

class Sounds {
public:
	void playSoundEffect(int soundIndex);
	void playSoundEffectInThread(int soundIndex);

};

class File {
	public:
		void firstTimeReadFile();
		void saveFile(int score);
};

/*
	Objeto que possui os m�todos de gerenciamento do jogo
*/
class Game {
private:
	void drawTransRect(Mat frame, Scalar color, double alpha, Rect region);
	bool isIntersecting(const Rect& rect1, const Rect& rect2);
	void intersectionPoints(const Rect& r, Rect& objRect, int& yObj, int& xRandObj, int& score, int scoreChange, int soundIndex, int explosion);

public:
	Sounds Som;
	File file;
	void drawMenu(string wName);
	void detectAndDraw(Mat& frame, CascadeClassifier& cascade, double scale, bool tryflip, int elapsedTime);
	void resetGame();
	void loadResources();
	void drawImage(Mat frame, Mat img, int xPos, int yPos);
};

/*
	Fun��o que tenta abrir o arquivo "placar.txt" para leitura.
	Se o arquivo n�o existir ou n�o puder ser aberto, exibe uma mensagem de erro.
	Caso contr�rio, l� o valor do placar e o armazena na vari�vel highScore.
*/
void File::firstTimeReadFile() {
	fstream file;
	file.open("placar.txt", ios::in);
	if (file) {
		file >> highScore;
		file.close();
	}
};

/*
	Fun��o que salva a maior pontua��o (score) no arquivo "placar.txt".
	Primeiro, tenta abrir o arquivo para leitura e verifica se a pontua��o
	registrada (highScore) pode ser lida. Em seguida, abre o arquivo para
	escrita. Se a pontua��o atual for maior que highScore, grava a nova
	pontua��o no arquivo; caso contr�rio, mant�m o recorde atual.
	Ao final, define isSaved como true.
*/
void File::saveFile(int score) {
	fstream file;
	file.open("placar.txt", ios::in);
	if (file) {
		file >> highScore;
		file.close();
	}
	file.open("placar.txt", ios::out);
	if (!file) {
		cout << "Erro ao abrir o arquivo para escrita!" << endl;
	}
	else if (score > highScore) {
		cout << "Escrevi no arquivo!" << endl;
		file << score << endl;
		highScore = score;
	}
	else {
		cout << "N�o passou do recorde!" << endl;
		file << highScore << endl;
	}
	isSaved = true;
	file.close();
}

void Game::drawMenu(string wName) {
	namedWindow(wName, WINDOW_KEEPRATIO);		// Exibe a janela
	Mat fundo = cv::imread("FundoFOCUS.png", IMREAD_UNCHANGED); // Carrega a imagem 

	imshow(wName, fundo); // Exibe a imagem na janela
}

Game jogo;

/*
	Se��o de configura��es de vari�veis essenciais e complexas e mecanismos

	Configura vari�veis essenciais para a l�gica
	Configura Param�tros da exibi��o
	Configura aleatoriedade dos itens
	Define fun��es para o uso de textura e sons
*/


RNG rng(cv::getTickCount());	// Inicializa um gerador de n�meros aleat�rios com o timestamp atual
string cascadeName;				// Nome do arquivo do classificador em cascata.
string wName = "FOCUS";			// Nome da janela de exibi��o do jogo

// Vari�vel global para armazenar o tempo do �ltimo reset
auto startTime = chrono::steady_clock::now();

// Vari�veis globais das coordenadas dos itens que aparecem para o player
int xRandCoal, yCoal, xRandTNT, yTNT, xRandCopper, yCopper, xRandDiamond,
yDiamond, xRandEmerald, yEmerald, xRandGold, yGold, xRandIron, yIron,
xRandLapis, yLapis, xRandRedstone, yRedstone, xParticles, yParticles, score;

vector<sf::SoundBuffer> soundBuffers(2);	// Vector com Buffers para sons.
vector<sf::Sound> sounds(2);				// Vector de sons correspondentes aos buffers.

vector<Mat> images(9);	// Vetor para armazenar imagens para o processamento.

/*
	Fun��o que reproduz um efeito sonoro com base no �ndice fornecido.
	Verifica se o �ndice � v�lido antes de tentar reproduzir o som.
	Se o �ndice for inv�lido, exibe uma mensagem de erro.
*/
void Sounds::playSoundEffect(int soundIndex) {
	if (soundIndex < 0 || soundIndex >= sounds.size()) {
		cout << "Erro: �ndice de som inv�lido!" << endl;
		return;
	}

	sounds[soundIndex].play();
}

/*
	Fun��o que reproduz um efeito sonoro ao chamar a fun��o playSoundEffect em uma nova thread.
*/
void Sounds::playSoundEffectInThread(int soundIndex) {
	// Cria uma nova thread que executa a fun��o playSoundEffect
	// passando o �ndice do som como argumento.
	std::thread t(&Sounds::playSoundEffect, this, soundIndex);
	// Desacopla a thread, permitindo que ela continue a execu��o
	// em segundo plano, independentemente do thread principal.
	t.detach();
}

/*
	Fun��o que reinicia o estado do jogo.
	- Chamada ao apertar r
	- Reinicia as coordenadas dos itens, com x aleat�rio e y = 0
	- Zera o score
	- Marca como false a vari�vel de controle do save
	- Reinicia o temporizador do jogo, armazenando o tempo atual na vari�vel startTime
*/

void Game::resetGame() {
	startTime = chrono::steady_clock::now();
	xRandCoal = rng.uniform(500, 1500);
	yCoal = 0;
	xRandTNT = rng.uniform(500, 1500);
	yTNT = 0;
	xRandCopper = rng.uniform(500, 1500);
	yCopper = 0;
	xRandDiamond = rng.uniform(500, 1500);
	yDiamond = 0;
	xRandEmerald = rng.uniform(500, 1500);
	yEmerald = 0;
	xRandGold = rng.uniform(500, 1500);
	yGold = 0;
	xRandIron = rng.uniform(500, 1500);
	yIron = 0;
	xRandLapis = rng.uniform(500, 1500);
	yLapis = 0;
	xRandRedstone = rng.uniform(500, 1500);
	yRedstone = 0;
	score = 0;
	isSaved = false;
}

/*
	Fun��o de carregar recursos de som e imagem para o jogo
*/
void Game::loadResources() {
	// Carrega o primeiro arquivo de som e verifica se ocorreu um erro.
	if (!soundBuffers[0].loadFromFile("Pop.wav")) {
		cout << "Erro ao carregar o arquivo de som Pop.wav!" << endl;
	}
	// Carrega o segundo arquivo de som e verifica se ocorreu um erro.
	if (!soundBuffers[1].loadFromFile("Explosion.wav")) {
		cout << "Erro ao carregar o arquivo de som Explosion.wav!" << endl;
	}

	// Associa os buffers de som aos objetos de som.
	sounds[0].setBuffer(soundBuffers[0]);
	sounds[1].setBuffer(soundBuffers[1]);

	// Carrega imagens dos recursos do jogo.
	images[0] = imread("Coal.png", IMREAD_UNCHANGED);
	images[1] = imread("TNT.png", IMREAD_UNCHANGED);
	images[2] = imread("Copper.png", IMREAD_UNCHANGED);
	images[3] = imread("Diamond.png", IMREAD_UNCHANGED);
	images[4] = imread("Emerald.png", IMREAD_UNCHANGED);
	images[5] = imread("Gold.png", IMREAD_UNCHANGED);
	images[6] = imread("Iron.png", IMREAD_UNCHANGED);
	images[7] = imread("Lapis.png", IMREAD_UNCHANGED);
	images[8] = imread("Redstone.png", IMREAD_UNCHANGED);

	// Redimensiona imagens maiores que 100x84 pixels.
	for (auto& img : images) {
		if (img.rows > 100 || img.cols > 84) {
			resize(img, img, Size(100, 84));
		}
	}
}

/*
	~Main do programa ~Chamada das fun��es e vari�veis j� implementadas + detectAndDraw
*/
int main(int argc, const char** argv) {
	// Declara��o de vari�veis
	VideoCapture capture;	// Usado para capturar v�deo de um arquivo ou c�mera
	Mat frame;				// Estrutura que armazena os quadros do v�deo
	bool tryflip;			// Vari�vel que indica se o v�deo deve ser invertido horizontalmente
	CascadeClassifier cascade; // Usado para a detec��o em cascata
	double scale;			// Fator de escala para redimensionar a imagem durante a detec��o
	char key = 0;			// Vari�vel para armazenar a tecla digitada

	while (1) {	// Escopo para a exibi��o do menu

		jogo.drawMenu(wName);

		key = (char)waitKey();
		if (key == 27 || key == 'q' || key == 'Q')
			return 0;
		if (key == 'r' || key == 'R') { // Verifica se a tecla 'r' foi pressionada
			break;
		}
	};

	// Inicializando e configurando as vari�veis declaradas
	cascadeName = "haarcascade_frontalface_alt.xml";	// Arquivo utilizado para a detec��o em cascata
	scale = 1; // usar 1, 2, 4.
	if (scale < 1)
		scale = 1;
	tryflip = true;

	// Tenta carregar o classificador em cascata
	if (!cascade.load(cascadeName)) {
		cout << "ERROR: Could not load classifier cascade: " << cascadeName << endl;
		return -1;
	}

	// Tenta abrir o v�deo -> Limpar argumento para tentar abrir a c�mera
	if (!capture.open(0)) {
		cout << "Capture from camera #0 didn't work" << endl;
		return 1;
	}

	// Defini��o da largura e altura do quadro do v�deo
	// Configurar de acordo com webcam / v�deo
	capture.set(CAP_PROP_FRAME_WIDTH, 1280); // Largura desejada
	capture.set(CAP_PROP_FRAME_HEIGHT, 720); // Altura desejada

	// Testa se a abertura ocorreu com �xito e inicia o jogo
	if (capture.isOpened()) {
		cout << "Video capturing has been started ..." << endl;
		namedWindow(wName, WINDOW_KEEPRATIO);	// Abre a janela para a exibi��o

		int fps = static_cast<int>(
			capture.get(CAP_PROP_FPS)); // Obt�m a taxa de quadros do v�deo
		int delay = (fps > 0) ? 1000 / fps
			: 60; // Calcula o tempo de espera em milissegundos,
		// usa 30ms como padr�o se fps for zero

		jogo.loadResources(); // Carrega os recursos (sons e imagens)
		jogo.resetGame();     // Inicializa as vari�veis do jogo

		while (1) {	// While onde ocorre a repeti��o do c�digo

			capture >> frame;	// Captura um novo quadro do v�deo

			if (frame.empty()) {	// Testa se o frame est� vazio
				cout << "ERROR: Frame is empty!" << endl;
				break;
			}

			// Verifica se a tecla pressionada � 0, indicando que � a primeira itera��o do loop e
			// Redimensiona a janela de exibi��o do v�deo para corresponder �s dimens�es do quadro (scale)
			if (key == 0) // just first time
				resizeWindow(wName, static_cast<int>(frame.cols / scale),
					static_cast<int>(frame.rows / scale));

			// Verifica se um segundo se passou desde a �ltima atualiza��o e atualiza o tempo decorrido
			auto currentTime = chrono::steady_clock::now();
			auto elapsedTime =
				chrono::duration_cast<chrono::milliseconds>(currentTime - startTime)
				.count();

			// Chama a fun��o que detecta os rostos e interage com a exibi��o
			jogo.detectAndDraw(frame, cascade, scale, tryflip, elapsedTime);

			key = (char)waitKey(delay); // Usa o tempo de espera calculado
			if (key == 27 || key == 'q' || key == 'Q')
				break;
			if (key == 'r' || key == 'R') { // Verifica se a tecla 'r' foi pressionada
				jogo.resetGame();
			}
			// Se a janela for fechada, o while � encerrado
			if (getWindowProperty(wName, WND_PROP_VISIBLE) == 0)
				break;
		}
	}
	else { // Caso o/a v�deo/c�mera n�o consiga ser aberto(a)
		cout << "ERROR: Could not open video capture!" << endl;
	}

	return 0;
}

/*

	Se��o das configura��es dos componentes da fun��o detectAndDraw

*/

/**
 * @brief Draws a transparent image over a frame Mat.
 *
 * @param frame the frame where the transparent image will be drawn
 * @param transp the Mat image with transparency, read from a PNG image, with
 * the IMREAD_UNCHANGED flag
 * @param xPos x position of the frame image where the image will start.
 * @param yPos y position of the frame image where the image will start.
 */

 /*
	 Fun��o respons�vel por desenhar uma imagem em uma posi��o especifica
	 -> Recebe o frame
	 -> Recebe a imagem a ser desenhada
	 -> Recebe o x e o y da posi��o
 */
void Game::drawImage(Mat frame, Mat img, int xPos, int yPos) {
	// Calcula as dimens�es da regi�o onde a imagem ser� desenhada
	int width = img.cols;
	int height = img.rows;

	// Ajusta a largura e altura se a imagem ultrapassar os limites do frame
	int xStart = max(xPos, 0);
	int yStart = max(yPos, 0);
	int xEnd = min(xPos + width, frame.cols);
	int yEnd = min(yPos + height, frame.rows);

	// Verifica se a regi�o ajustada ainda est� dentro dos limites do frame
	if (xStart >= xEnd || yStart >= yEnd) {
		return;
	}

	// Ajusta a regi�o da imagem se parte dela estiver fora dos limites do frame
	int imgXStart = (xPos < 0) ? -xPos : 0;
	int imgYStart = (yPos < 0) ? -yPos : 0;
	int imgWidth = xEnd - xStart;
	int imgHeight = yEnd - yStart;

	// Verifica se as dimens�es ajustadas s�o v�lidas
	if (imgWidth <= 0 || imgHeight <= 0) {
		return;
	}

	Mat mask;			// M�scara para aplicar transpar�ncia
	vector<Mat> layers;	// Vetor para armazenar os canais da imagem

	split(img, layers);       // separa os canais

	if (layers.size() == 4) { // verifica se a imagem possui canal alfa (transpar�ncia)

		Mat rgb[3] = { layers[0], layers[1], layers[2] };	// Canais RGB

		mask = layers[3];   // Canal alfa do PNG usado como m�scara

		merge(rgb, 3, img); // Junta os canais RGB, agora img n�o � transparente

		// Copia a regi�o da imagem para o frame, aplicando a m�scara de transpar�ncia
		img(Rect(imgXStart, imgYStart, imgWidth, imgHeight))
			.copyTo(frame.rowRange(yStart, yEnd).colRange(xStart, xEnd),
				mask(Rect(imgXStart, imgYStart, imgWidth, imgHeight)));
	}

	else if (layers.size() == 3) { // Se a imagem n�o possui o canal alfa
		// Copia diretamente a regi�o da imagem para o frame
		img(Rect(imgXStart, imgYStart, imgWidth, imgHeight))
			.copyTo(frame.rowRange(yStart, yEnd).colRange(xStart, xEnd));
	}
	else {
		// Se a imagem n�o possui 3 ou 4 canais, imprime um erro
		cout << "ERROR: Unsupported number of channels in image!" << endl;
	}
}

/**
 * @brief Draws a transparent rect over a frame Mat.
 *
 * @param frame the frame where the transparent image will be drawn
 * @param color the color of the rect
 * @param alpha transparence level. 0 is 100% transparent, 1 is opaque.
 * @param region rect region where the should be positioned
 */

 // Fun��o respons�vel por desenhar um ret�ngulo nas faces detectadas
void Game::drawTransRect(Mat frame, Scalar color, double alpha, Rect region) {
	// Verifica se a regi�o est� dentro dos limites da imagem
	if (region.x >= 0 && region.y >= 0 && region.x + region.width <= frame.cols &&
		region.y + region.height <= frame.rows) {

		// Cria uma regi�o de interesse (ROI) a partir do quadro
		Mat roi = frame(region);
		// Cria uma imagem do tamanho da ROI preenchida com a cor especificada
		Mat rectImg(roi.size(), CV_8UC3, color);
		// Adiciona a imagem do ret�ngulo � ROI, aplicando o n�vel de transpar�ncia
		addWeighted(rectImg, alpha, roi, 1.0 - alpha, 0, roi);
	}
	else {
		// Lida com o erro: regi�o fora dos limites
		cout << "ERROR: Region is out of bounds!" << endl;
	}
}

// Fun��o que verifica intersec��o de dois ret�ngulos.
bool Game::isIntersecting(const Rect& rect1, const Rect& rect2) {
	// Verifica a partir da �rea gerada entre os retangulos (intersec��o)
	// Se for maior que 0 retorna true, caso contr�rio retorna false
	return (rect1 & rect2).area() > 0;
}

/*
	Fun��o que verifica a interse��o entre um ret�ngulo 'r' e um objeto 'objRect'.
	Ou seja, essa fun��o � utilizada para verificar se o player interagiu com algum objeto
	Se houver interse��o, atualiza a pontua��o e reposiciona o objeto.
*/
void Game::intersectionPoints(const Rect& r, Rect& objRect, int& yObj, int& xRandObj,
	int& score, int scoreChange, int soundIndex, int explosion) {
	// Verifica se o ret�ngulo 'r' intersecta o 'objRect' ao chamar a fun��o isIntersecting
	// -> Recebe o objeto e a face 
	// -> Recebe as coordenadas do objeto
	// -> Recebe o score atual
	// -> Recebe o scoreChange do objeto
	// -> Recebe o indice do som do objeto
	// -> Recebe o bool(int) para mostrar a explos�o
	if (isIntersecting(r, objRect)) {
		// Atualiza a pontua��o adicionando 'scoreChange', valor que varia de objeto p/ objeto
		score += scoreChange;
		// Se for o caso de explos�o ir� mostrar particulas no local
		if (explosion == 1) {
			Explode = 1;
			xParticles = xRandObj;
			yParticles = yObj;
			explodeTime = chrono::steady_clock::now();
		}
		// Reinicia a coordenada y do objeto
		yObj = 0;
		// Gera uma nova coordenada x aleat�ria para o objeto dentro de um intervalo espec�fico
		xRandObj = rng.uniform(100, 1161);
		// Reproduz um efeito sonoro em uma thread separada ao chamar a fun��o playSoundEffectInThread
		Som.playSoundEffectInThread(soundIndex);

	}
}


/*


	Par�metros:
	 - Mat& frame: refer�ncia para o quadro de imagem atual a ser processado

	 - CascadeClassifier& cascade: refer�ncia para o classificador em cascata

	 - double scale: fator de escala para redimensionar a imagem durante a detec��o.

	 - bool tryflip: indica se deve tentar inverter a imagem p/ detectar os objetos

	 - int elapsedTime: vari�vel que registra o tempo decorrido

*/

void Game::detectAndDraw(Mat& frame, CascadeClassifier& cascade, double scale,
	bool tryflip, int elapsedTime) {
	vector<Rect> faces;	// cria um vetor para armazenar os ret�ngulos que representam as faces detectadas
	Mat grayFrame, smallFrame;	// declara duas matrizes para a imagem em escala de cinza e a imagem redimensionada
	Scalar color = Scalar(255, 0, 0);	// define a cor para desenhar os ret�ngulos (azul).

	double fx = 1 / scale;	// Redimensiona com base no scale
	// Redimensiona a imagem para 'smallFrame' usando o fx.
	resize(frame, smallFrame, Size(), fx, fx, INTER_LINEAR_EXACT);

	// Se 'tryflip' for verdadeiro, inverte a imagem horizontalmente.
	if (tryflip)
		flip(smallFrame, smallFrame, 1);

	// Converte a imagem colorida 'smallFrame' para escala de cinza em 'grayFrame'
	// Aplica equaliza��o de histograma � imagem em escala de cinza 'grayFrame'
	// Essas configura��es melhoram a detec��o das faces
	cvtColor(smallFrame, grayFrame, COLOR_BGR2GRAY);
	equalizeHist(grayFrame, grayFrame);

	// Detecta rostos na imagem 'grayFrame' utilizando o classificador em cascata 'cascade' 
	// e armazena os ret�ngulos resultantes em 'faces'. Os par�metros ajustam a escala, 
	// o n�mero m�nimo de vizinhos e o tamanho da detec��o.
	cascade.detectMultiScale(grayFrame, faces, 1.5, 3,
		0 | CASCADE_FIND_BIGGEST_OBJECT |
		CASCADE_DO_ROUGH_SEARCH | CASCADE_SCALE_IMAGE,
		Size(40, 40));

	// PERCORRE AS FACES ENCONTRADAS
	for (size_t i = 0; i < faces.size(); i++) {

		Rect r = faces[i]; // Para cada rosto detectado vai haver uma itera��o, sendo r a representa��o 

		// Desenha um ret�ngulo ao redor da face na imagem
		rectangle(
			smallFrame, Point(cvRound(r.x), cvRound(r.y)),
			Point(cvRound((r.x + r.width - 1)), cvRound((r.y + r.height - 1))),
			color, 3);

		// Define os ret�ngulos para diferentes elementos do jogo para esse frame em especifico
		Rect coalRect(xRandCoal, yCoal, 100, 84);
		Rect copperRect(xRandCopper, yCopper, 100, 84);
		Rect diamondRect(xRandDiamond, yDiamond, 100, 84);
		Rect emeraldRect(xRandEmerald, yEmerald, 100, 84);
		Rect goldRect(xRandGold, yGold, 100, 84);
		Rect ironRect(xRandIron, yIron, 100, 84);
		Rect lapisRect(xRandLapis, yLapis, 100, 84);
		Rect redstoneRect(xRandRedstone, yRedstone, 100, 84);
		Rect TNTRect(xRandTNT, yTNT, 100, 84);

		// Verifica enquanto o tempo decorrido � menor ou igual a 30 segundos
		// se h� interse��es entre a face e os ret�ngulos dos elementos do jogo
		if (elapsedTime <= 30000) {
			intersectionPoints(r, coalRect, yCoal, xRandCoal, score, 5, 0, 0);
			intersectionPoints(r, TNTRect, yTNT, xRandTNT, score, -100, 1, 1);
			intersectionPoints(r, copperRect, yCopper, xRandCopper, score, 7, 0, 0);
			intersectionPoints(r, diamondRect, yDiamond, xRandDiamond, score, 19, 0, 0);
			intersectionPoints(r, emeraldRect, yEmerald, xRandEmerald, score, 22, 0, 0);
			intersectionPoints(r, goldRect, yGold, xRandGold, score, 17, 0, 0);
			intersectionPoints(r, ironRect, yIron, xRandIron, score, 10, 0, 0);
			intersectionPoints(r, lapisRect, yLapis, xRandLapis, score, 12, 0, 0);
			intersectionPoints(r, redstoneRect, yRedstone, xRandRedstone, score, 14, 0, 0);
		}
	}


	if (elapsedTime <= 30000) {
		// Enquanto o tempo � menor que 30 seg carrega as imagens dos itens do jogo
		Mat img = cv::imread("Coal.png", IMREAD_UNCHANGED),
			img2 = imread("TNT.png", IMREAD_UNCHANGED),
			img3 = imread("Copper.png", IMREAD_UNCHANGED),
			img4 = imread("Diamond.png", IMREAD_UNCHANGED),
			img5 = imread("Emerald.png", IMREAD_UNCHANGED),
			img6 = imread("Gold.png", IMREAD_UNCHANGED),
			img7 = imread("Iron.png", IMREAD_UNCHANGED),
			img8 = imread("Lapis.png", IMREAD_UNCHANGED),
			img9 = imread("Redstone.png", IMREAD_UNCHANGED),
			img10 = imread("Particles.png", IMREAD_UNCHANGED);

		// Verifica se cada imagem � maior que 100x84 pixels e redimensiona, se necess�rio
		if (img.rows > 100 || img.cols > 84)
			resize(img, img, Size(100, 84));
		if (img2.rows > 100 || img2.cols > 84)
			resize(img2, img2, Size(100, 84));
		if (img3.rows > 100 || img3.cols > 84)
			resize(img3, img3, Size(100, 84));
		if (img4.rows > 100 || img4.cols > 84)
			resize(img4, img4, Size(100, 84));
		if (img5.rows > 100 || img5.cols > 84)
			resize(img5, img5, Size(100, 84));
		if (img6.rows > 100 || img6.cols > 84)
			resize(img6, img6, Size(100, 84));
		if (img7.rows > 100 || img7.cols > 84)
			resize(img7, img7, Size(100, 84));
		if (img8.rows > 100 || img8.cols > 84)
			resize(img8, img8, Size(100, 84));
		if (img9.rows > 100 || img9.cols > 84)
			resize(img9, img9, Size(100, 84));

		// Redimensiona o tamanho da particula
		if (img10.rows > 200 || img10.cols > 168)
			resize(img10, img10, Size(300, 252));

		// Desenha as imagens dos itens na posi��o especificada no 'smallFrame'
		drawImage(smallFrame, img, xRandCoal, yCoal);
		drawImage(smallFrame, img2, xRandTNT, yTNT);
		drawImage(smallFrame, img3, xRandCopper, yCopper);
		drawImage(smallFrame, img4, xRandDiamond, yDiamond);
		drawImage(smallFrame, img5, xRandEmerald, yEmerald);
		drawImage(smallFrame, img6, xRandGold, yGold);
		drawImage(smallFrame, img7, xRandIron, yIron);
		drawImage(smallFrame, img8, xRandLapis, yLapis);
		drawImage(smallFrame, img9, xRandRedstone, yRedstone);
		drawImage(smallFrame, img10, xParticles, yParticles);

		// Atualiza a posi��o vertical de cada item, movendo-os para baixo
		yCoal += 20;
		yTNT += 20;
		yCopper += 20;
		yDiamond += 20;
		yEmerald += 20;
		yGold += 20;
		yIron += 20;
		yLapis += 20;
		yRedstone += 20;
		// Verifica se cada item ultrapassou a altura m�xima de 720 pixels
		// Se sim, reinicia a posi��o vertical e gera uma nova posi��o horizontal aleat�ria

		if (yCoal > 720) {
			yCoal = 0;
			xRandCoal = rng.uniform(100, 1161);
		}
		if (yTNT > 720) {
			yTNT = 0;
			xRandTNT = rng.uniform(100, 1161);
		}
		if (yCopper > 720) {
			yCopper = 0;
			xRandCopper = rng.uniform(100, 1161);
		}
		if (yDiamond > 720) {
			yDiamond = 0;
			xRandDiamond = rng.uniform(100, 1161);
		}
		if (yEmerald > 720) {
			yEmerald = 0;
			xRandEmerald = rng.uniform(100, 1161);
		}
		if (yGold > 720) {
			yGold = 0;
			xRandGold = rng.uniform(100, 1161);
		}
		if (yIron > 720) {
			yIron = 0;
			xRandIron = rng.uniform(100, 1161);
		}
		if (yLapis > 720) {
			yLapis = 0;
			xRandLapis = rng.uniform(100, 1161);
		}
		if (yRedstone > 720) {
			yRedstone = 0;
			xRandRedstone = rng.uniform(100, 1161);
		}

		// Escopo para mostrar as particulas de explos�o na tela
		auto now = chrono::steady_clock::now();
		auto explodeDuration = chrono::duration_cast<chrono::milliseconds>(now - explodeTime).count();
		if (!(Explode == 1 && explodeDuration < 500)) {
			Explode = 0;
			xParticles = 10000000;
			yParticles = 10000000;
		}
	}

	// Desenha quadrados com transparencia
	double alpha = 1;
	// drawTransRect(smallFrame, Scalar(255, 0, 0), alpha, Rect(200, 0, 200,
	// 200));

	// Desenha um texto
	color = Scalar(255, 255, 255);

	file.firstTimeReadFile();	// Chamando o m�todo para ler o maior record

	// Inicializa a largura do ret�ngulo para o high score
	int highScoreWidth = 0;

	// Define a largura do ret�ngulo com base no valor do high score
	// Sendo pequeno, m�dio, grande e gigante

	if (highScore < 100 && highScore >-100) {
		highScoreWidth = 375;
	}
	else if (highScore < 1000 && highScore > -1000) {
		highScoreWidth = 425;
	}
	else if (highScore < 10000 && highScore > -10000) {
		highScoreWidth = 475;
	}
	else {
		highScoreWidth = 525;
	}

	if (highScore != INT_MIN) {
		// Desenha um ret�ngulo com uma cor espec�fica na parte inferior da tela para exibir o high score
		drawTransRect(smallFrame, Scalar(242, 101, 88), alpha, Rect(0, 620, highScoreWidth, 100));

		// Adiciona o texto "Recorde:" na tela na posi��o especificada
		putText(smallFrame, "Recorde: ", Point(0, 700), FONT_HERSHEY_DUPLEX, 2, color);

		// Exibe o recorde na parte inferior da tela
		putText(smallFrame, to_string(highScore), Point(280, 700), FONT_HERSHEY_DUPLEX, 2, color);
	}

	// Exibir as informa��es da gameplay
	if (elapsedTime <= 30000) {
		// Desenha um ret�ngulo com transpar�ncia na parte superior da tela
		drawTransRect(smallFrame, Scalar(242, 101, 88), alpha, Rect(0, 0, 825, 110));

		// Exibe o texto "Placar:" na posi��o especificada
		putText(smallFrame, "Placar:", Point(0, 80), FONT_HERSHEY_DUPLEX, 2, color);

		// Exibe a pontua��o atual na posi��o especificada
		putText(smallFrame, to_string(score), Point(230, 80), FONT_HERSHEY_DUPLEX, 2, color);

		// Exibe o texto "Tempo:" na posi��o especificada
		putText(smallFrame, "Tempo:", Point(450, 80), FONT_HERSHEY_DUPLEX, 2, color);

		// Calcula e exibe o tempo restante em segundos
		putText(smallFrame, to_string((30000 - elapsedTime) / 1000), Point(730, 80), FONT_HERSHEY_DUPLEX, 2, color);

	}
	else {
		// Se o jogo j� terminou e a pontua��o n�o foi salva ele ir� salvar
		if (isSaved == false) {
			file.saveFile(score);
		}

		// Desenha um ret�ngulo com transpar�ncia na parte superior da tela para o fim de jogo
		drawTransRect(smallFrame, Scalar(242, 101, 88), alpha, Rect(0, 0, 1280, 110));

		// Exibe o texto "Placar:" na parte superior
		putText(smallFrame, "Placar:", Point(0, 80), FONT_HERSHEY_DUPLEX, 2, color);

		// Exibe a pontua��o final na posi��o especificada
		putText(smallFrame, to_string(score), Point(230, 80), FONT_HERSHEY_DUPLEX, 2, color);

		// Exibe o texto "Fim de Jogo" na posi��o especificada
		putText(smallFrame, "Fim de Jogo", Point(450, 80), FONT_HERSHEY_DUPLEX, 2, color);

		// Exibe o recorde na parte inferior da tela
		putText(smallFrame, to_string(highScore), Point(280, 700), FONT_HERSHEY_DUPLEX, 2, color);

		// Informa o jogador sobre como reiniciar o jogo
		putText(smallFrame, "Pressione 'r'", Point(890, 45), FONT_HERSHEY_DUPLEX, 1.75, color);
		putText(smallFrame, "para reiniciar", Point(890, 90), FONT_HERSHEY_DUPLEX, 1.75, color);

	}
	// Desenha o frame na tela
	imshow(wName, smallFrame);
}
