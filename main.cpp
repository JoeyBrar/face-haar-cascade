#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp> 
#include <opencv2/objdetect.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/core/types.hpp>
#include <opencv2/core.hpp>
#include <iostream>

int main() { 
	cv::Mat img; 
	cv::VideoCapture cap(0); 
	cv::CascadeClassifier face, smile;
	std::vector<cv::Rect> faces, smiles;
	int inBox = 0;

	face.load("haarcascade_frontalface_default.xml");
	smile.load("haarcascade_smile.xml");

	cv::namedWindow("win", cv::WINDOW_NORMAL);
	cv::setWindowProperty("win", cv::WND_PROP_FULLSCREEN, cv::WINDOW_FULLSCREEN); //FULLSCREEN

	if (!cap.isOpened()) { std::cout << "ERROR"; return -1; }
	while (true) {
		cap >> img;

		face.detectMultiScale(img, faces, 1.6, 2);
		//smile.detectMultiScale(img, smiles, 1.4, 1);

		for (int i = 0; i < faces.size(); i++) {
			for (int j = 0; j < smiles.size(); j++) {
				if (smiles[j].x > faces[i].x && smiles[j].y > faces[i].y && smiles[j].x + smiles[j].width < faces[i].x + faces[i].width && smiles[j].y + smiles[j].height < faces[i].y + faces[i].height) {
					inBox = 1;
				}
			}
		}

		
		cv::rectangle(img, cv::Point(0, 0), cv::Point(5000, 50), cv::Scalar(0, 121, 225), cv::FILLED);
		if (faces.size() == 1 && smiles.size()!=0&&inBox==1) cv::putText(img, std::to_string(faces.size()) + " PERSON DETECTED: HAPPY", cv::Point(15, 36), cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(255, 255, 255), 1.4);
		else if (faces.size() == 1) cv::putText(img, std::to_string(faces.size()) + " PERSON DETECTED", cv::Point(15, 36), cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(255, 255, 255), 1.4);
		else cv::putText(img, std::to_string(faces.size()) + " PEOPLE DETECTED", cv::Point(15, 36), cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(255, 255, 255), 1.4);

		for (int i = 0; i < faces.size(); i++) {
			for (int j = 0; j < smiles.size(); j++) {
				if (smiles[j].x > faces[i].x && smiles[j].y > faces[i].y && smiles[j].x + smiles[j].width < faces[i].x + faces[i].width && smiles[j].y + smiles[j].height < faces[i].y + faces[i].height) {
					std::cout << "SMILE: (" << smiles[j].x << ", " << smiles[j].y << ") -> (" << smiles[j].x + smiles[j].width << ", " << smiles[j].y + smiles[j].height << ")" << std::endl;
					cv::rectangle(img, cv::Point(smiles[j].x, smiles[j].y), cv::Point(smiles[j].x + smiles[j].width, smiles[j].y + smiles[j].height), cv::Scalar(0, 225, 225), 5);
				}
			}

			std::cout << "FACE: (" << faces[i].x<<", "<<faces[i].y << ") -> (" << faces[i].x + faces[i].width << ", " << faces[i].y + faces[i].height << ")" << std::endl;
			cv::rectangle(img, cv::Point(faces[i].x, faces[i].y), cv::Point(faces[i].x + faces[i].width, faces[i].y + faces[i].height), cv::Scalar(0, 121, 225), 5);
		}

		cv::imshow("win", img);
		inBox = 0;
		int c = cv::waitKey(1);
		if (c==32) break;
	}
	
	cap.release();
	return 0;
}
