#pragma once


#include "Mona/PacketReader.h"
#include "Mona/Startable.h"

namespace Mona {
	class FlashStream;
}

extern "C"
{
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libavutil/avutil.h"
#include "libavutil/opt.h"
#include "libavutil/pixdesc.h"
};



namespace  Mona
{

	class Transcode :public Startable, public virtual Object
	{
	public:
		Transcode();

		//�ص����� ���յ�����buffer������buf��
		friend int read_buffer(void *opaque, uint8_t *buf, int buf_size);
		friend int write_buffer(void *opaque, uint8_t *buf, int buf_size);

		//int decode(int size,const uint8_t *buf);
		Buffer * decode(PacketReader &videoPacket);     //����

		static void build_flv_message(char *tagHeader, char *tagEnd, int size, UInt32 &timeStamp);

		int flush_encoder(AVFormatContext *fmt_ctx, unsigned int stream_index);

		void run(Exception& ex);

	private:
		AVFormatContext* ifmt_ctx;		//AVFormatContext:ͳ��ȫ�ֵĻ����ṹ�塣��Ҫ���ڴ�����װ��ʽ��FLV/MK/RMVB��
		unsigned char* inbuffer;       //���뻺������
		unsigned char* outbuffer;
		AVIOContext *avio_in;          //�����Ӧ�Ľṹ�壬��������
		AVIOContext *avio_out;
		AVInputFormat *piFmt;

		AVFormatContext* ofmt_ctx;
		AVPacket packet, enc_pkt;          //�洢ѹ�����ݣ���Ƶ��ӦH.264���������ݣ���Ƶ��ӦPCM�������ݣ�
		AVFrame *frame;            //AVPacket�洢��ѹ�������ݣ���Ƶ��ӦRGB/YUV�������ݣ���Ƶ��ӦPCM�������ݣ�
		enum AVMediaType type;				//ָ�������ͣ�����Ƶ����Ƶ��������Ļ

		AVStream *out_stream;             //AVStream��AVCodecContext������Ƶ����Ӧ�Ľṹ�壬��������Ƶ����롣
		AVStream *in_stream;
		AVCodecContext *dec_ctx, *enc_ctx;
		AVCodec *encoder;                 //AVCodec�Ǵ洢���������Ϣ�Ľṹ�壬enconder�洢������Ϣ�Ľṹ��
		Buffer outVideoBuffer;
	};

	
} //namespace FFMPEG