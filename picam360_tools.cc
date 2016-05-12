/**
 * @file omxcv-test.cpp
 * @brief Simple testing application for omxcv.
 */
#include "picam360_tools.h"
#include "omxcv.h"
#include "gl_transform.h"
#include <opencv2/opencv.hpp>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <thread>

//#define TEXURE_WIDTH  1296//2048
//#define TEXURE_HEIGHT 1944//2048
//#define TEXURE_WIDTH  1312
//#define TEXURE_HEIGHT 1952
//#define TEXURE_WIDTH  960
//#define TEXURE_HEIGHT 1440
//#define TEXURE_WIDTH  1376
//#define TEXURE_HEIGHT 1024
//#define TEXURE_WIDTH  1664
//#define TEXURE_HEIGHT 1232
#define TEXURE_WIDTH  1248
#define TEXURE_HEIGHT 1232
//#define EQUIRECTANGULAR_WIDTH  1024
//#define EQUIRECTANGULAR_HEIGHT 512
#define EQUIRECTANGULAR_WIDTH  1440
#define EQUIRECTANGULAR_HEIGHT 720
//#define EQUIRECTANGULAR_WIDTH  2048
//#define EQUIRECTANGULAR_HEIGHT 1024
#define MMAP_COUNT    2
#define PICTURE_NUM   10

#define TIMEDIFF(start) (duration_cast<microseconds>(steady_clock::now() - start).count())

using omxcv::OmxCv;
using omxcv::OmxCvJpeg;
using std::this_thread::sleep_for;
using std::chrono::microseconds;
using std::chrono::milliseconds;
using std::chrono::steady_clock;
using std::chrono::duration_cast;

using namespace openblw;

//pre procedure difinition

//structure difinition

//global variables

OmxCvJpeg encoder = OmxCvJpeg(EQUIRECTANGULAR_WIDTH, EQUIRECTANGULAR_HEIGHT);
GLTransform transformer(EQUIRECTANGULAR_WIDTH, EQUIRECTANGULAR_HEIGHT,
TEXURE_WIDTH, TEXURE_HEIGHT);
OmxCv *recorder = NULL;

int StartRecord(const char *filename, int bitrate_kbps) {
	recorder = new OmxCv(filename, EQUIRECTANGULAR_WIDTH,
			EQUIRECTANGULAR_HEIGHT, bitrate_kbps);
	return 0;
}

int StopRecord() {
	if (recorder == NULL)
		return -1;
	delete recorder;
	recorder = NULL;
	return 0;
}

int SetRotation(float x_deg, float y_deg, float z_deg) {
	transformer.SetRotation(x_deg, y_deg, z_deg);
}

int AddFrame(int width, int height, int stride,
		const unsigned char *imagedata1, const unsigned char *imagedata2) {
	if (recorder == NULL)
		return -1;

	cv::Mat raw_image(TEXURE_HEIGHT, TEXURE_WIDTH, CV_8UC(stride / width));
	cv::Mat vr_image(EQUIRECTANGULAR_HEIGHT, EQUIRECTANGULAR_WIDTH, CV_8UC(3));

	memcpy(raw_image.data, imagedata1, stride * height);
	//if(imagedata2 != NULL) {
	//	memcpy(raw_image.data + stride * height, imagedata2, stride * height);
	//}

	transformer.Transform(raw_image, vr_image);
	recorder->Encode(vr_image);
}

int SaveJpegAsEquirectangular(int width, int height, int stride,
		const unsigned char *imagedata1, const unsigned char *imagedata2,
		const char *out_filename) {

	cv::Mat raw_image(TEXURE_HEIGHT, TEXURE_WIDTH, CV_8UC(stride / width));
	cv::Mat vr_image(EQUIRECTANGULAR_HEIGHT, EQUIRECTANGULAR_WIDTH, CV_8UC(3));

	if (TEXURE_WIDTH == width && TEXURE_HEIGHT == height) {
		memcpy(raw_image.data, imagedata1, stride * height);
		//if(imagedata2 != NULL) {
		//	memcpy(raw_image.data + stride * height, imagedata2, stride * height);
		//}
	} else {
//		int xoffset = (TEXURE_WIDTH - width) / 2;
//		int yoffset = (TEXURE_HEIGHT - height) / 2;
//		unsigned char *src_cur;
//		unsigned char *des_cur;
//		{
//			src_cur = (unsigned char *) imagedata1;
//			des_cur = (unsigned char *) (raw_image.data
//					+ yoffset * raw_image.step + xoffset * raw_image.elemSize());
//			for (int y = 0; y < height; y++) {
//				memcpy(des_cur, src_cur, stride);
//				src_cur += stride;
//				des_cur += raw_image.step;
//			}
//		}
//		{
//			src_cur = (unsigned char *) imagedata2;
//			des_cur = (unsigned char *) (raw_image.data
//					+ stride * height + imagedata2 + yoffset * raw_image.step
//					+ xoffset * raw_image.elemSize());
//			for (int y = 0; y < height; y++) {
//				memcpy(des_cur, src_cur, stride);
//				src_cur += stride;
//				des_cur += raw_image.step;
//			}
//		}
	}

	if (out_filename != NULL) {

		transformer.Transform(raw_image, vr_image);

		if (encoder.Encode(out_filename, vr_image)) {
		} else {
			perror("error on jpeg encode");
			return -1;
		}
	}

	return 0;
}
