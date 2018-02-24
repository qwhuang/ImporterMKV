#include "Decoder.h"

Decoder::Decoder()
{
	packet = av_packet_alloc();
	frame = av_frame_alloc();
}

Decoder::~Decoder()
{
	av_frame_free(&frame);
	av_packet_free(&packet);
}

int Decoder::open(const char *filename)
{
	int ret;

	// Open file
	fmt_ctx = avformat_alloc_context();
	fmt_ctx->flags = AVFMT_FLAG_GENPTS;
	if ((ret = avformat_open_input(&fmt_ctx, filename, NULL, NULL)) < 0)
	{
		return ret;
	}

	if (avformat_find_stream_info(fmt_ctx, NULL) < 0) {
		exit(1);
	}

	// Open all streams
	//for (int i = 0; i < fmt_ctx->nb_streams; i++)
	for (int i = 0; i < 1; i++)
	{
		AVStream *stream = fmt_ctx->streams[i];
		AVCodec *codec = avcodec_find_decoder(stream->codecpar->codec_id);
		codecContext = avcodec_alloc_context3(codec);

		ret = avcodec_parameters_to_context(codecContext, stream->codecpar);

		/*
		if (codec->type == AVMEDIA_TYPE_VIDEO)
		{
			codecContext->width = stream->codecpar->width;
			codecContext->height = stream->codecpar->height;
			codecContext->time_base = stream->r_frame_rate;
			codecContext->pix_fmt = codec->pix_fmts ? codec->pix_fmts[0] : AV_PIX_FMT_YUV420P;
		}
		*/
		if ((ret = avcodec_open2(codecContext, NULL, NULL)) < 0)
		{
			break;
		}
	}
	av_dump_format(fmt_ctx, 0, filename, 0);

	isOpen = true;

	return ret;
}

void Decoder::close()
{
	avcodec_free_context(&codecContext);
	avformat_close_input(&fmt_ctx);

	isOpen = false;

}

int64_t Decoder::getDuration()
{
	return fmt_ctx->duration;
}

AVStream* Decoder::findStream(AVMediaType type)
{
	AVStream *stream = NULL;
	int idx = av_find_best_stream(fmt_ctx, type, -1, -1, NULL, 0);
	if (idx >= 0)
	{
		stream = fmt_ctx->streams[idx];
	}

	return stream;
}

AVFrame* Decoder::getFrameByTimestamp(int streamIdx, int64_t timestamp)
{
	int ret;

	// Seek the desired frame
	if ((ret = avformat_seek_file(fmt_ctx, streamIdx, timestamp, timestamp, timestamp, 0)) < 0)
	{
		return NULL;
	}

	// Read and the decode the frame
	AVPacket *packet = av_packet_alloc();
	while (av_read_frame(fmt_ctx, packet) >= 0)
	{
		if (packet->stream_index == 0)
		{
			// Request one frame and flush the encoder directly
			ret = avcodec_send_packet(codecContext, packet);
			ret = avcodec_send_packet(codecContext, NULL);

			// Receive the frame
			ret = avcodec_receive_frame(codecContext, frame);
			if (ret == 0)
			{
				return frame;
			}
		}
		
	}

	return NULL;
}