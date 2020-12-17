/*
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "kcftracker.hpp"

#include <dirent.h>

using namespace std;
using namespace cv;

int main(int argc, char* argv[]){

	if (argc > 5) return -1;

	bool HOG = true;						//�Ƿ�ʹ��hog����
	bool FIXEDWINDOW = false;	//�Ƿ�ʹ����������
	bool MULTISCALE = true;		//�Ƿ�ʹ�ö�߶�
	bool SILENT = true;					//�Ƿ�����ʾ
	bool LAB = false;						//�Ƿ�ʹ��LAB��ɫ

	for(int i = 0; i < argc; i++){
		if ( strcmp (argv[i], "hog") == 0 )
			HOG = true;
		if ( strcmp (argv[i], "fixed_window") == 0 )
			FIXEDWINDOW = true;
		if ( strcmp (argv[i], "singlescale") == 0 )
			MULTISCALE = false;
		if ( strcmp (argv[i], "show") == 0 )
			SILENT = false;
		if ( strcmp (argv[i], "lab") == 0 ){
			LAB = true;
			HOG = true;
		}
		if ( strcmp (argv[i], "gray") == 0 )
			HOG = false;
	}
	
	// Create KCFTracker object
	KCFTracker tracker(HOG, FIXEDWINDOW, MULTISCALE, LAB);

	// Frame readed
	Mat frame;

	// Tracker results
	Rect result;

	// Path to list.txt
	ifstream listFile;
	string fileName = "images.txt";
  	listFile.open(fileName);			//���ļ�

  	// Read groundtruth for the 1st frame
  	ifstream groundtruthFile;
	string groundtruth = "region.txt";
  	groundtruthFile.open(groundtruth);
  	string firstLine;
  	getline(groundtruthFile, firstLine);		//����groundtruthFile �� firstLine
	groundtruthFile.close();
  	
  	istringstream ss(firstLine);				//#include<sstream>        //istringstream ����������ͷ�ļ�     istringstream������ִ��C++���Ĵ������������

  	// Read groundtruth like a dumb					���ϡ����ϡ����¡����µ�X ,Y     �������һ���еĶ�����洢���˶��float
  	float x1, y1, x2, y2, x3, y3, x4, y4;
  	char ch;
	ss >> x1;
	ss >> ch;												//�м��  ,  
	ss >> y1;
	ss >> ch;
	ss >> x2;
	ss >> ch;
	ss >> y2;
	ss >> ch;
	ss >> x3;
	ss >> ch;
	ss >> y3;
	ss >> ch;
	ss >> x4;
	ss >> ch;
	ss >> y4; 

	// Using min and max of X and Y for groundtruth rectangle											�����������Ϳ��
	float xMin =  min(x1, min(x2, min(x3, x4)));
	float yMin =  min(y1, min(y2, min(y3, y4)));
	float width = max(x1, max(x2, max(x3, x4))) - xMin;
	float height = max(y1, max(y2, max(y3, y4))) - yMin;

	
	// Read Images																	
	ifstream listFramesFile;
	string listFrames = "images.txt";
	listFramesFile.open(listFrames);
	string frameName;


	// Write Results
	ofstream resultsFile;
	string resultsPath = "output.txt";
	resultsFile.open(resultsPath);

	// Frame counter
	int nFrames = 0;


	while ( getline(listFramesFile, frameName) ){						//��ȡlistFramesFile�е�ÿһ��  frameName
		frameName = frameName;

		// Read each frame from the list
		frame = imread(frameName, CV_LOAD_IMAGE_COLOR);				//����ͼƬ

		// First frame, give the groundtruth to the tracker								��һ֡��ʼ��  KCFTracker  ��	�����Σ�ͼƬ��
		if (nFrames == 0) {
			tracker.init( Rect(xMin, yMin, width, height), frame );
			rectangle( frame, Point( xMin, yMin ), Point( xMin+width, yMin+height), Scalar( 0, 255, 255 ), 1, 8 );			//��ʼ������
			resultsFile << xMin << "," << yMin << "," << width << "," << height << endl;											
		}
		// Update
		else{
			result = tracker.update(frame);									//���¿�  ������X,����Y,���ߣ�
			
			rectangle( frame, Point( result.x, result.y ), Point( result.x+result.width, result.y+result.height), Scalar( 0, 255, 255 ), 1, 8 );
			resultsFile << result.x << "," << result.y << "," << result.width << "," << result.height << endl;
		}

		nFrames++;

		if (!SILENT){
			imshow("Image", frame);
			waitKey(1);
		}
	}
	resultsFile.close();

	listFile.close();

}



*/