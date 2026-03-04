#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

// Глобальные настройки для трекбаров
int threshVal = 180;
int minAreaVal = 500;
const int MAX_THRESH = 255;
const int MAX_AREA = 10000;

int main() {
    // Загрузка изображения
    string imagePath = "coins.jpg";
    Mat img = imread(imagePath);

    if (img.empty()) {
        cout << "Error: Image not found! Place 'coins.jpg' near the .exe or check path." << endl;
        return -1;
    }

    // Настройка окон
    namedWindow("Detector", WINDOW_NORMAL);
    namedWindow("Mask (Debug)", WINDOW_NORMAL);

    // Задаем удобный стартовый размер
    resizeWindow("Detector", 800, 600);
    resizeWindow("Mask (Debug)", 400, 300);

    createTrackbar("Threshold", "Detector", &threshVal, MAX_THRESH);
    createTrackbar("Min Area", "Detector", &minAreaVal, MAX_AREA);

    Mat gray, blur, binary, displayImg;

    cvtColor(img, gray, COLOR_BGR2GRAY);
    GaussianBlur(gray, blur, Size(7, 7), 0);

    cout << "Controls: [S] - Save, [ESC] - Exit" << endl;

    while (true) {
        threshold(blur, binary, threshVal, 255, THRESH_BINARY_INV);

        // Поиск контуров
        vector<vector<Point>> contours;
        findContours(binary, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

        displayImg = img.clone();
        int objectsFound = 0;

        for (const auto& contour : contours) {
            double area = contourArea(contour);

            if (area > minAreaVal) {
                // Отрисовка рамки
                Rect rect = boundingRect(contour);
                rectangle(displayImg, rect, Scalar(0, 255, 0), 3);

                objectsFound++;
            }
        }

        // Вывод инфо
        rectangle(displayImg, Point(0, 0), Point(250, 40), Scalar(0, 0, 0), -1);
        string info = "Objects: " + to_string(objectsFound);
        putText(displayImg, info, Point(10, 30), FONT_HERSHEY_SIMPLEX, 0.8, Scalar(0, 255, 0), 2);

        imshow("Detector", displayImg);
        imshow("Mask (Debug)", binary);

        char key = (char)waitKey(30);
        if (key == 27) break;
        if (key == 's' || key == 'S') {
            imwrite("result.jpg", displayImg);
            cout << "Saved to result.jpg" << endl;
        }
    }

    destroyAllWindows();
    return 0;
}