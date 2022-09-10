

#include "loadFrame.h"
bool load_frame(const char* filename, int* width1, int* height1, unsigned char** data1){
    AVFormatContext* av_format_ctx = avformat_alloc_context();
    if(!av_format_ctx){
        printf("1");
        return false;
    }
    if(avformat_open_input(&av_format_ctx,filename,NULL,NULL)!=0){
        printf("2");
        return false;
    }

    int video_stream_index = -1;
    //int audio_stream_index = -1;
    AVCodecParameters* av_codec_params=nullptr;
    AVCodec* av_codec=nullptr;
    for(int i =0;i<av_format_ctx->nb_streams;++i){
        auto stream = av_format_ctx->streams[i];
        av_codec_params = av_format_ctx->streams[i]->codecpar;
        av_codec = avcodec_find_decoder(av_codec_params->codec_id);
        if(!av_codec){
            continue;
        }
        if(av_codec_params->codec_type == AVMEDIA_TYPE_VIDEO){
            video_stream_index = i;
            //stream
            break;
        }


    }
    if(video_stream_index == -1){
        printf("3");
        return false;
    }
    AVCodecContext* av_codec_ctx = avcodec_alloc_context3(av_codec);
    if(!av_codec_ctx){
        printf("4");
        return false;
    }
    if(avcodec_parameters_to_context(av_codec_ctx,av_codec_params)<0){
        printf("5\n");
        return false;
    }
    if(avcodec_open2(av_codec_ctx,av_codec,NULL)<0){
        printf("6");
        return false;
    }

    AVFrame* av_frame = av_frame_alloc();
    if(!av_frame){
        printf("7");
        return false;
    }
    AVPacket* av_packet = av_packet_alloc();
    if(!av_packet){
        printf("8");
        return false;
    }
    int response;
    while(av_read_frame(av_format_ctx,av_packet)>=0){
        if(av_packet->stream_index != video_stream_index){
            continue;
        }
        response = avcodec_send_packet(av_codec_ctx, av_packet);
        if(response <0){
            printf("9");
            return false;
        }
        response = avcodec_receive_frame(av_codec_ctx,av_frame);
        if(response == AVERROR(EAGAIN) || response == AVERROR_EOF){
            continue;
        } else if(response < 0 ){
            printf("10");
            return false;
        }

        av_packet_unref(av_packet);
        break;

    }
//    unsigned char* data = new unsigned char[av_frame->width*av_frame->height*3];
//    for(int x=0;x<av_frame->width;++x){
//        for(int y=0;y<av_frame->height;++y){
//            data[y*av_frame->width*3+x*3]=  av_frame->data[0][y*av_frame->linesize[0]+x];
//            data[y*av_frame->width*3+x*3+1]=av_frame->data[0][y*av_frame->linesize[0]+x];
//            data[y*av_frame->width*3+x*3+2]=av_frame->data[0][y*av_frame->linesize[0]+x];
//        }
//    }
//    *width1 = av_frame->width;
//    *height1 = av_frame->height;
//    *data1 = data;

    uint8_t* data = new uint8_t[av_frame->width*av_frame->height*4];
    SwsContext* sws_scaler_ctx = sws_getContext(
                av_frame->width,
                av_frame->height,
                av_codec_ctx->pix_fmt,
                av_frame->width,
                av_frame->height,
                AV_PIX_FMT_RGB0,
                SWS_BILINEAR,
                NULL,
                NULL,
                NULL);

    if(!sws_scaler_ctx){
        return false;
    }
    uint8_t* dest[4]={data,NULL,NULL,NULL};
    int dest_linesize[4]={av_frame->width*4,0,0,0};
    sws_scale(sws_scaler_ctx,av_frame->data,av_frame->linesize,
              0,
              av_frame->height,
              dest,
              dest_linesize);
    sws_freeContext(sws_scaler_ctx);
        *width1 = av_frame->width;
        *height1 = av_frame->height;
        *data1 = data;
    avformat_close_input(&av_format_ctx);
    avformat_free_context(av_format_ctx);
    av_frame_free(&av_frame);
    av_packet_free(&av_packet);
    avcodec_free_context(&av_codec_ctx);
    return true;
}

bool video_open(VideoState* state, const char *filename)
{
//    auto& width = state->width;
//    auto& height = state->height;
//    auto& time_base = state->time_base;
//    auto& av_format_ctx = state->av_format_ctx;
//    auto& av_codec_ctx = state->av_codec_ctx;
//    auto& video_stream_index = state->video_stream_index;
//    auto& av_frame = state->av_frame;
//    auto& av_packet = state->av_packet;
    state->av_format_ctx = avformat_alloc_context();
    if(!state->av_format_ctx){
        printf("1\n");
        return false;
    }
    if(avformat_open_input(&state->av_format_ctx,filename,NULL,NULL)!=0){
        printf("2\n");
        return false;
    }

    state->video_stream_index = -1;
    state->audio_stream_index = -1;
    AVCodecParameters* av_codec_params;
    AVCodec* av_codec;
    for(int i =0;i<state->av_format_ctx->nb_streams;++i){
        //auto stream = state->av_format_ctx->streams[i];
        av_codec_params = state->av_format_ctx->streams[i]->codecpar;
        av_codec = avcodec_find_decoder(av_codec_params->codec_id);
        if(!av_codec){
            continue;
        }
        if(av_codec_params->codec_type == AVMEDIA_TYPE_VIDEO){
            state->video_stream_index = i;
            state->width = av_codec_params->width;
            state->height = av_codec_params->height;
            state->time_base = state->av_format_ctx->streams[i]->time_base;
            //stream
            break;
        }
        if(av_codec_params->codec_type == AVMEDIA_TYPE_AUDIO){
            state->audio_stream_index = i;
//            state->width = av_codec_params->width;
//            state->height = av_codec_params->height;
//            state->time_base = state->av_format_ctx->streams[i]->time_base;
            //stream
            break;
        }

    }
    if(state->video_stream_index == -1){
        printf("3\n");
        return false;
    }
    state->av_codec_ctx = avcodec_alloc_context3(av_codec);
    if(!state->av_codec_ctx){
        printf("4\n");
        return false;
    }
    if(avcodec_parameters_to_context(state->av_codec_ctx,av_codec_params)<0){
        printf("5\n");
        return false;
    }
    if(avcodec_open2(state->av_codec_ctx,av_codec,NULL)<0){
        printf("6\n");
        return false;
    }

   state->av_frame = av_frame_alloc();

    if(!state->av_frame){
        printf("7\n");
        return false;
    }

    state->av_packet = av_packet_alloc();
    if(!state->av_packet){
        printf("8\n");
        return false;
    }
//    state->width=state->av_frame->width;
//    state->height=state->av_frame->height;
    return true;
}

bool video_readframe(VideoState* state, uint8_t *framebuffer, int64_t* pts)
{
//    state->width=state->av_frame->width;
//    state->height=state->av_frame->height;
    //int video_stream_index = -1;
    int response;
    while(av_read_frame(state->av_format_ctx,state->av_packet)>=0){
        if(state->av_packet->stream_index != state->video_stream_index){
            av_packet_unref(state->av_packet);
            continue;
        }
        response = avcodec_send_packet(state->av_codec_ctx, state->av_packet);
        if(response <0){
            printf("9\n");
            return false;
        }
        response = avcodec_receive_frame(state->av_codec_ctx,state->av_frame);
        if(response == AVERROR(EAGAIN) || response == AVERROR_EOF){
            continue;
        } else if(response < 0 ){
            printf("10\n");
            return false;
        }

        av_packet_unref(state->av_packet);
        break;

    }
    *pts = state->av_frame->pts;
//    unsigned char* data = new unsigned char[av_frame->width*av_frame->height*3];
//    for(int x=0;x<av_frame->width;++x){
//        for(int y=0;y<av_frame->height;++y){
//            data[y*av_frame->width*3+x*3]=  av_frame->data[0][y*av_frame->linesize[0]+x];
//            data[y*av_frame->width*3+x*3+1]=av_frame->data[0][y*av_frame->linesize[0]+x];
//            data[y*av_frame->width*3+x*3+2]=av_frame->data[0][y*av_frame->linesize[0]+x];
//        }
//    }
//    *width1 = av_frame->width;
//    *height1 = av_frame->height;
//    *data1 = data;

    //uint8_t* data = new uint8_t[state->av_frame->width*state->av_frame->height*4];
    //framebuffer = data;

    if(!state->sws_scaler_ctx){
    state->sws_scaler_ctx = sws_getContext(
                state->av_frame->width,
                state->av_frame->height,
                state->av_codec_ctx->pix_fmt,
                state->av_frame->width,
                state->av_frame->height,
                AV_PIX_FMT_RGB0,
                SWS_BILINEAR,
                NULL,
                NULL,
                NULL);
    }

    if(!state->sws_scaler_ctx){
        printf("13\n");
        return false;
    }

    uint8_t* dest[4]={framebuffer,NULL,NULL,NULL};
    int dest_linesize[4]={state->av_frame->width*4,0,0,0};
    sws_scale(state->sws_scaler_ctx,
              state->av_frame->data,
              state->av_frame->linesize,
              0,
              state->av_frame->height,
              dest,
              dest_linesize);

    return true;
}

void video_close(VideoState* state)
{
    sws_freeContext(state->sws_scaler_ctx);

    avformat_close_input(&state->av_format_ctx);
    avformat_free_context(state->av_format_ctx);
    av_frame_free(&state->av_frame);
    av_packet_free(&state->av_packet);
    avcodec_free_context(&state->av_codec_ctx);
}
