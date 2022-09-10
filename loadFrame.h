#ifndef LOADFRAME_H
#define LOADFRAME_H

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <inttypes.h>
}
struct VideoState {
    int width,height,video_stream_index,audio_stream_index;
    AVFormatContext* av_format_ctx;
    //AVCodecParameters* av_codec_params;
    //AVCodec* av_codec;
    AVCodecContext* av_codec_ctx;
    AVFrame* av_frame;
    AVPacket* av_packet;
    SwsContext* sws_scaler_ctx;
    AVRational time_base;
};

bool load_frame(const char* filename, int* width, int* height, unsigned char** data);

bool video_open(VideoState* state, const char* filename);
bool video_readframe(VideoState* state, uint8_t *framebuffer, int64_t *pts);
void video_close(VideoState* state);

#endif // LOADFRAME_H
