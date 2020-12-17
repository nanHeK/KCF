#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include "kcftracker.hpp"

#include <dirent.h>
using namespace std;
using namespace cv;

int main(int argc, char* argv[])
{
	if (argc > 5) return -1;

	bool HOG = true;						//�Ƿ�ʹ��hog����
	bool FIXEDWINDOW = false;	//�Ƿ�ʹ����������
	bool MULTISCALE = true;		//�Ƿ�ʹ�ö�߶�
	bool SILENT = true;					//�Ƿ�����ʾ
	bool LAB = false;						//�Ƿ�ʹ��LAB��ɫ

	for (int i = 0; i < argc; i++) {
		if (strcmp(argv[i], "hog") == 0)
			HOG = true;
		if (strcmp(argv[i], "fixed_window") == 0)
			FIXEDWINDOW = true;
		if (strcmp(argv[i], "singlescale") == 0)
			MULTISCALE = false;
		if (strcmp(argv[i], "show") == 0)
			SILENT = false;
		if (strcmp(argv[i], "lab") == 0) {
			LAB = true;
			HOG = true;
		}
		if (strcmp(argv[i], "gray") == 0)
			HOG = false;
	}

	// Create KCFTracker object
	KCFTracker tracker(HOG, FIXEDWINDOW, MULTISCALE, LAB);

	float xMin = 900;
	float yMin = 210;
	float width = 50;
	float height = 120;
	Rect result;
	//����Ƶ�ļ�����ʵ���ǽ���һ��VideoCapture�ṹ			E:/0A_hk/Datasets/00000_cam2_new.MP4
	VideoCapture capture("F:/DukeMTMC/hcutvideo/1/11tv.mp4");
	//����Ƿ�������:�ɹ���ʱ��isOpened����ture
	if (!capture.isOpened())
		cout << "fail toopen!" << endl;

	//��ȡ����֡��
	long totalFrameNumber = capture.get(CV_CAP_PROP_FRAME_COUNT);
	cout << "������Ƶ��" << totalFrameNumber << "֡" << endl;


	//���ÿ�ʼ֡()
	long frameToStart = 1;
	capture.set(CV_CAP_PROP_POS_FRAMES, frameToStart);
	cout << "�ӵ�" << frameToStart << "֡��ʼ��" << endl;

	//���ý���֡
	int frameToStop = 300;

	if (frameToStop < frameToStart)
	{
		cout << "����֡С�ڿ�ʼ֡��������󣬼����˳���" << endl;
		return -1;
	}
	else
	{
		cout << "����֡Ϊ����" << frameToStop << "֡" << endl;
	}

	//��ȡ֡��
	double rate = capture.get(CV_CAP_PROP_FPS);
	cout << "֡��Ϊ:" << rate << endl;


	//����һ���������ƶ�ȡ��Ƶѭ�������ı���
	bool stop = false;

	//����ÿһ֡��ͼ��
	Mat frame;

	//��ʾÿһ֡�Ĵ���
	//namedWindow( "Extractedframe" );

	//��֡��ļ��ʱ��:
	//int delay = 1000/rate;
	double delay = 1000 / rate;


	//����whileѭ����ȡ֡
	//currentFrame����ѭ�����п��ƶ�ȡ��ָ����֡��ѭ�������ı���
	long currentFrame = frameToStart;
	bool isInit = true;

	while (!stop)
	{
		//��ȡ��һ֡
		if (!capture.read(frame))
		{
			cout << "��ȡ��Ƶʧ��" << endl;
			return -1;
		}


		//cout << "���ڶ�ȡ��" << currentFrame << "֡" << endl;
		//imshow( "Extractedframe", frame );

		//�˴�Ϊ��֡����
		if (currentFrame % 1 == 0) //�˴�Ϊ֡��������޸�����Ϳ�����
		{
			cout << "����д��" << currentFrame << "֡" << endl;
			stringstream str;
			str << "E:/0A_hk/Datasets/readVideo_Frame/" << currentFrame << ".png";        /*ͼƬ�洢λ��*/

			cout << str.str() << endl;
			if (isInit) {
				tracker.init(Rect(xMin, yMin, width, height), frame);
				rectangle(frame, Point(xMin, yMin), Point(xMin + width, yMin + height), Scalar(0, 255, 255), 1, 8);			//��ʼ������
				isInit = false;
			}
			else {
				result = tracker.update(frame);									//���¿�  ������X,����Y,���ߣ�
				rectangle(frame, Point(result.x, result.y), Point(result.x + result.width, result.y + result.height), Scalar(0, 255, 255), 1, 8);
			}
			//imwrite(str.str(), frame);															����ͼƬ
		}

		//waitKey(intdelay=0)��delay�� 0ʱ����Զ�ȴ�����delay>0ʱ��ȴ�delay����
		//��ʱ�����ǰû�а�������ʱ������ֵΪ-1�����򷵻ذ���
		int c = waitKey(delay);
		//����ESC���ߵ���ָ���Ľ���֡���˳���ȡ��Ƶ
		if ((char)c == 27 || currentFrame > frameToStop)
		{
			stop = true;
		}
		//���°������ͣ���ڵ�ǰ֡���ȴ���һ�ΰ���
		if (c >= 0)
		{
			waitKey(0);
		}
		currentFrame++;
		if (!SILENT) {
			imshow("Image", frame);
			waitKey(1);
		}

	}

	//�ر���Ƶ�ļ�
	capture.release();
	waitKey(0);
	return 0;
}