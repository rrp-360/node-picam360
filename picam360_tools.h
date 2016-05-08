#ifndef PICAM360_TOOLS_H
#define PICAM360_TOOLS_H


#ifdef __cplusplus
extern "C" {
#endif

int StartRecord(const char *filename, int bitrate_kbps);
int StopRecord();
int AddFrame(int width, int height, int stride,
		const unsigned char *imagedata1, const unsigned char *imagedata2);
int SaveJpegAsEquirectangular(int width, int height, int stride,
		const unsigned char *imagedata1, const unsigned char *imagedata2, const char *out_filename);
int SetRotation(float x_deg, float y_deg, float z_deg);

#ifdef __cplusplus
}
#endif

#endif
