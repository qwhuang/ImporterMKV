#pragma once

#include "lavf.h"

class Decoder
{
public:
	Decoder();
	~Decoder();
	int open(const char *filename);
	void close();
	AVStream* findStream(AVMediaType type);
	AVFrame* getFrameByTimestamp(int streamIdx, int64_t timestamp);
	int64_t getDuration();
	bool isOpen = false;

private:
	const char *filename;
	AVFormatContext *fmt_ctx;
	AVCodecContext *codecContext;
	AVPacket *packet;
	AVFrame *frame;
};
