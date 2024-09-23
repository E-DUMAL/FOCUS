#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/objdetect.hpp"
#include "opencv2/videoio.hpp"
#include <chrono>
#include <iostream>

using namespace std;
using namespace cv;

void detectAndDraw(Mat &frame, CascadeClassifier &cascade, double scale,
                   bool tryflip, int elapsedTime);
RNG rng(cv::getTickCount());
string cascadeName;
string wName = "Game";

// Variável global para armazenar o tempo do último reset
auto startTime = chrono::steady_clock::now();

int xRandCoal, yCoal, xRandTNT, yTNT, xRandCopper, yCopper, xRandDiamond,
    yDiamond, xRandEmerald, yEmerald, xRandGold, yGold, xRandIron, yIron,
    xRandLapis, yLapis, xRandRedstone, yRedstone, score;

void resetGame() {
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
}

int main(int argc, const char **argv) {
  VideoCapture capture;
  Mat frame;
  bool tryflip;
  CascadeClassifier cascade;
  double scale;
  char key = 0;

  cascadeName = "haarcascade_frontalface_alt.xml";
  scale = 1; // usar 1, 2, 4.
  if (scale < 1)
    scale = 1;
  tryflip = false;

  if (!cascade.load(cascadeName)) {
    cout << "ERROR: Could not load classifier cascade: " << cascadeName << endl;
    return -1;
  }

  if (!capture.open("video.mp4")) {
    cout << "Capture from camera #0 didn't work" << endl;
    return 1;
  }

  capture.set(CAP_PROP_FRAME_WIDTH, 1920);  // Largura desejada
  capture.set(CAP_PROP_FRAME_HEIGHT, 1080); // Altura desejada

  if (capture.isOpened()) {
    cout << "Video capturing has been started ..." << endl;
    namedWindow(wName, WINDOW_KEEPRATIO);

    int fps = static_cast<int>(
        capture.get(CAP_PROP_FPS)); // Obtém a taxa de quadros do vídeo
    int delay = (fps > 0) ? 1000 / fps
                          : 60; // Calcula o tempo de espera em milissegundos,
                                // usa 30ms como padrão se fps for zero

    resetGame(); // Inicializa as variáveis do jogo

    while (1) {
      capture >> frame;
      capture >> frame;
      if (frame.empty()) {
        cout << "ERROR: Frame is empty!" << endl;
        break;
      }
      if (key == 0) // just first time
        resizeWindow(wName, static_cast<int>(frame.cols / scale),
                     static_cast<int>(frame.rows / scale));

      // Verifica se um segundo se passou desde a última atualização
      auto currentTime = chrono::steady_clock::now();
      auto elapsedTime =
          chrono::duration_cast<chrono::seconds>(currentTime - startTime)
              .count();

      detectAndDraw(frame, cascade, scale, tryflip, elapsedTime);

      key = (char)waitKey(delay); // Usa o tempo de espera calculado
      if (key == 27 || key == 'q' || key == 'Q')
        break;
      if (key == 'r' || key == 'R') { // Verifica se a tecla 'r' foi pressionada
        resetGame();
      }
      if (getWindowProperty(wName, WND_PROP_VISIBLE) == 0)
        break;
    }
  } else {
    cout << "ERROR: Could not open video capture!" << endl;
  }

  return 0;
}

/**
 * @brief Draws a transparent image over a frame Mat.
 *
 * @param frame the frame where the transparent image will be drawn
 * @param transp the Mat image with transparency, read from a PNG image, with
 * the IMREAD_UNCHANGED flag
 * @param xPos x position of the frame image where the image will start.
 * @param yPos y position of the frame image where the image will start.
 */
void drawImage(Mat frame, Mat img, int xPos, int yPos) {
  // Calcula as dimensões da região onde a imagem será desenhada
  int width = img.cols;
  int height = img.rows;

  // Ajusta a largura e altura se a imagem ultrapassar os limites do frame
  int xStart = max(xPos, 0);
  int yStart = max(yPos, 0);
  int xEnd = min(xPos + width, frame.cols);
  int yEnd = min(yPos + height, frame.rows);

  // Verifica se a região ajustada ainda está dentro dos limites do frame
  if (xStart >= xEnd || yStart >= yEnd) {
    return;
  }

  // Ajusta a região da imagem se parte dela estiver fora dos limites do frame
  int imgXStart = (xPos < 0) ? -xPos : 0;
  int imgYStart = (yPos < 0) ? -yPos : 0;
  int imgWidth = xEnd - xStart;
  int imgHeight = yEnd - yStart;

  // Verifica se as dimensões ajustadas são válidas
  if (imgWidth <= 0 || imgHeight <= 0) {
    return;
  }

  Mat mask;
  vector<Mat> layers;

  split(img, layers);       // separa os canais
  if (layers.size() == 4) { // img com transparência
    Mat rgb[3] = {layers[0], layers[1], layers[2]};
    mask = layers[3];   // canal alfa do PNG usado como máscara
    merge(rgb, 3, img); // junta os canais RGB, agora img não é transparente
    img(Rect(imgXStart, imgYStart, imgWidth, imgHeight))
        .copyTo(frame.rowRange(yStart, yEnd).colRange(xStart, xEnd),
                mask(Rect(imgXStart, imgYStart, imgWidth, imgHeight)));
  } else if (layers.size() == 3) { // img sem transparência
    img(Rect(imgXStart, imgYStart, imgWidth, imgHeight))
        .copyTo(frame.rowRange(yStart, yEnd).colRange(xStart, xEnd));
  } else {
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
void drawTransRect(Mat frame, Scalar color, double alpha, Rect region) {
  // Verifica se a região está dentro dos limites da imagem
  if (region.x >= 0 && region.y >= 0 && region.x + region.width <= frame.cols &&
      region.y + region.height <= frame.rows) {

    Mat roi = frame(region);
    Mat rectImg(roi.size(), CV_8UC3, color);
    addWeighted(rectImg, alpha, roi, 1.0 - alpha, 0, roi);
  } else {
    // Lida com o erro: região fora dos limites
    cout << "ERROR: Region is out of bounds!" << endl;
  }
}

bool isIntersecting(const Rect &rect1, const Rect &rect2) {
  return (rect1 & rect2).area() > 0;
}

void intersectionPoints(const Rect &r, Rect &objRect, int &yObj, int &xRandObj,
                        int &score, int scoreChange) {
  if (isIntersecting(r, objRect)) {
    score += scoreChange;
    yObj = 0;
    xRandObj = rng.uniform(100, 1821);
  }
}

void detectAndDraw(Mat &frame, CascadeClassifier &cascade, double scale,
                   bool tryflip, int elapsedTime) {
  vector<Rect> faces;
  Mat grayFrame, smallFrame;
  Scalar color = Scalar(255, 0, 0);

  double fx = 1 / scale;
  resize(frame, smallFrame, Size(), fx, fx, INTER_LINEAR_EXACT);
  if (tryflip)
    flip(smallFrame, smallFrame, 1);
  cvtColor(smallFrame, grayFrame, COLOR_BGR2GRAY);
  equalizeHist(grayFrame, grayFrame);

  printf("smallFrame::width: %d, height=%d\n", smallFrame.cols,
         smallFrame.rows);

  cascade.detectMultiScale(grayFrame, faces, 1.6, 2,
                           0 | CASCADE_FIND_BIGGEST_OBJECT |
                               CASCADE_DO_ROUGH_SEARCH | CASCADE_SCALE_IMAGE,
                           Size(40, 40));

  // PERCORRE AS FACES ENCONTRADAS
  for (size_t i = 0; i < faces.size(); i++) {
    Rect r = faces[i];
    rectangle(
        smallFrame, Point(cvRound(r.x), cvRound(r.y)),
        Point(cvRound((r.x + r.width - 1)), cvRound((r.y + r.height - 1))),
        color, 3);

    Rect coalRect(xRandCoal, yCoal, 100, 84);
    Rect copperRect(xRandCopper, yCopper, 100, 84);
    Rect diamondRect(xRandDiamond, yDiamond, 100, 84);
    Rect emeraldRect(xRandEmerald, yEmerald, 100, 84);
    Rect goldRect(xRandGold, yGold, 100, 84);
    Rect ironRect(xRandIron, yIron, 100, 84);
    Rect lapisRect(xRandLapis, yLapis, 100, 84);
    Rect redstoneRect(xRandRedstone, yRedstone, 100, 84);
    Rect TNTRect(xRandTNT, yTNT, 100, 84);
    if (elapsedTime <= 30) {
      intersectionPoints(r, coalRect, yCoal, xRandCoal, score, 5);
      intersectionPoints(r, TNTRect, yTNT, xRandTNT, score, -100);
      intersectionPoints(r, copperRect, yCopper, xRandCopper, score, 7);
      intersectionPoints(r, diamondRect, yDiamond, xRandDiamond, score, 19);
      intersectionPoints(r, emeraldRect, yEmerald, xRandEmerald, score, 22);
      intersectionPoints(r, goldRect, yGold, xRandGold, score, 17);
      intersectionPoints(r, ironRect, yIron, xRandIron, score, 10);
      intersectionPoints(r, lapisRect, yLapis, xRandLapis, score, 12);
      intersectionPoints(r, redstoneRect, yRedstone, xRandRedstone, score, 14);
    }
  }

  if (elapsedTime <= 30) {
    // Desenha uma imagem
    Mat img = cv::imread("Coal.png", IMREAD_UNCHANGED),
        img2 = imread("TNT.png", IMREAD_UNCHANGED),
        img3 = imread("Copper.png", IMREAD_UNCHANGED),
        img4 = imread("Diamond.png", IMREAD_UNCHANGED),
        img5 = imread("Emerald.png", IMREAD_UNCHANGED),
        img6 = imread("Gold.png", IMREAD_UNCHANGED),
        img7 = imread("Iron.png", IMREAD_UNCHANGED),
        img8 = imread("Lapis.png", IMREAD_UNCHANGED),
        img9 = imread("Redstone.png", IMREAD_UNCHANGED);

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

    drawImage(smallFrame, img, xRandCoal, yCoal);
    drawImage(smallFrame, img2, xRandTNT, yTNT);
    drawImage(smallFrame, img3, xRandCopper, yCopper);
    drawImage(smallFrame, img4, xRandDiamond, yDiamond);
    drawImage(smallFrame, img5, xRandEmerald, yEmerald);
    drawImage(smallFrame, img6, xRandGold, yGold);
    drawImage(smallFrame, img7, xRandIron, yIron);
    drawImage(smallFrame, img8, xRandLapis, yLapis);
    drawImage(smallFrame, img9, xRandRedstone, yRedstone);

    yCoal += 20;
    yTNT += 20;
    yCopper += 20;
    yDiamond += 20;
    yEmerald += 20;
    yGold += 20;
    yIron += 20;
    yLapis += 20;
    yRedstone += 20;
    if (yCoal > 1080) {
      yCoal = 0;
      xRandCoal = rng.uniform(100, 1821);
    }
    if (yTNT > 1080) {
      yTNT = 0;
      xRandTNT = rng.uniform(100, 1821);
    }
    if (yCopper > 1080) {
      yCopper = 0;
      xRandCopper = rng.uniform(100, 1821);
    }
    if (yDiamond > 1080) {
      yDiamond = 0;
      xRandDiamond = rng.uniform(100, 1821);
    }
    if (yEmerald > 1080) {
      yEmerald = 0;
      xRandEmerald = rng.uniform(100, 1821);
    }
    if (yGold > 1080) {
      yGold = 0;
      xRandGold = rng.uniform(100, 1821);
    }
    if (yIron > 1080) {
      yIron = 0;
      xRandIron = rng.uniform(100, 1821);
    }
    if (yLapis > 1080) {
      yLapis = 0;
      xRandLapis = rng.uniform(100, 1821);
    }
    if (yRedstone > 1080) {
      yRedstone = 0;
      xRandRedstone = rng.uniform(100, 1821);
    }
  }
  // Desenha quadrados com transparencia
  double alpha = 1;
  // drawTransRect(smallFrame, Scalar(255, 0, 0), alpha, Rect(200, 0, 200,
  // 200));

  // Desenha um texto
  color = Scalar(255, 255, 255);
  if (elapsedTime <= 30) {
    drawTransRect(smallFrame, Scalar(242, 101, 88), alpha,
                  Rect(0, 0, 1270, 110));
    putText(smallFrame, "Placar:", Point(0, 80), FONT_HERSHEY_DUPLEX, 3, color);
    putText(smallFrame, to_string(score), Point(330, 80), FONT_HERSHEY_DUPLEX,
            3, color);
    putText(smallFrame, "Tempo:", Point(800, 80), FONT_HERSHEY_DUPLEX, 3,
            color);
    putText(smallFrame, to_string(30 - elapsedTime), Point(1140, 80),
            FONT_HERSHEY_DUPLEX, 3, color);
  } else {
    drawTransRect(smallFrame, Scalar(242, 101, 88), alpha,
                  Rect(0, 0, 1420, 110));
    drawTransRect(smallFrame, Scalar(242, 101, 88), alpha,
                  Rect(500, 960, 1330, 110));
    putText(smallFrame, "Placar:", Point(0, 80), FONT_HERSHEY_DUPLEX, 3, color);
    putText(smallFrame, to_string(score), Point(330, 80), FONT_HERSHEY_DUPLEX,
            3, color);
    putText(smallFrame, "Fim de Jogo", Point(800, 80), FONT_HERSHEY_DUPLEX, 3,
            color);
    putText(smallFrame, "Pressione 'r' para reiniciar", Point(500, 1040),
            FONT_HERSHEY_DUPLEX, 3, color);
  }
  // Desenha o frame na tela
  imshow(wName, smallFrame);
}
