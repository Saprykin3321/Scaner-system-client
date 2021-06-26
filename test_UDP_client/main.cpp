#include <iostream>
#include <string>
#include <thread>
#include <mutex>

#include "CClient.h"

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc_c.h>
#include <opencv2/videoio/videoio_c.h>
#include <opencv2/calib3d.hpp>
#include <opencv2/opencv.hpp>

using namespace std;
#define width 1280
#define height 720
#define width_short 640
#define height_short 480

std::mutex m;
bool stop_thread = false;

string convertToString(char* a, int size)
{
	int i;
	string s = "";
	for (i = 0; i < size; i++) {
		s = s + a[i];
	}
	return s;
}

void receiv_video(Client_UDP client)
{
	cv::Mat frame;
	frame = cv::imread("D:/wtf11.jpg", 1);
	cv::imshow("Received_image", frame);

	char scrin[921600];
	int nSendSize = 921600;
	char buf[1024];

	while (1)
	{
		for (int y1 = 0; y1 < height_short; y1++)
		{
			unsigned char stroka[width_short * 3 + 2];
			int ny = 0;
			m.lock();
			client.RecMsg(reinterpret_cast<char*>(stroka), width_short * 3 + 2);
			m.unlock();
			if (stroka[0] == 1) { ny = 255 + stroka[1]; }
			else { ny = stroka[1]; }
			uchar* ptr = (uchar*)(frame.data + ny * frame.step);
			for (int x = 0; x < frame.cols; x++) {
				ptr[3 * x] = (uchar)stroka[x * 3 + 2];
				ptr[3 * x + 1] = (uchar)stroka[x * 3 + 3];
				ptr[3 * x + 2] = (uchar)stroka[x * 3 + 4];
			}
		}
		cv::imshow("Received_image", frame);
		cv::waitKey(33);
	}
}

int main()
{
	Client_UDP client_UDP;
	Client_TCP client_TCP;

	//thread recProc(&Client_TCP::recvData, &client_TCP);

	while (true)
	{
		//SEND
		std::string msg1;
		std::getline(std::cin, msg1);
		int msg_size1 = msg1.size();
		client_TCP.sendData((char*)&msg_size1, sizeof(int));
		client_TCP.sendData(msg1.c_str(), msg_size1);

		//RECEIV
		int msg_size;
		client_TCP.recvData((char*)&msg_size, sizeof(int));
		char *msg = new char[msg_size + 1];
		msg[msg_size] = '\0';
		client_TCP.recvData(msg, msg_size);
		std::cout << "Message from server: " << msg
				  << " (size: " << msg_size << ")"
				  << std::endl;
		string command = convertToString(msg, msg_size);
		if (command == "Start send video")
		{
			std::thread video(receiv_video, client_UDP);
			video.detach();
		}
		delete[] msg;
	}

	system("pause");
	return 0;

}