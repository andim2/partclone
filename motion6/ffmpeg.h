#ifndef _INCLUDE_FFMPEG_H_
#define _INCLUDE_FFMPEG_H_

#ifdef HAVE_FFMPEG
#include "avformat.h"
#endif

/* Define a codec name/identifier for timelapse videos, so that we can
 * differentiate between normal mpeg1 videos and timelapse videos.
 */
#define TIMELAPSE_CODEC "mpeg1_tl"

struct ffmpeg {
#ifdef HAVE_FFMPEG
	AVFormatContext *oc;
	AVStream *video_st;
	AVCodecContext *c;

	AVFrame *picture;       /* contains default imaga pointers */
	uint8_t *video_outbuf;
	int video_outbuf_size;

	void *udata;            /* U & V planes for greyscale images */
	int vbr;                /* variable bitrate setting */
	char codec[20];         /* codec name */
#else
	int dummy;
#endif
};

/* Initialize FFmpeg stuff. Needs to be called before ffmpeg_open. */
void ffmpeg_init(void);

/* Open an mpeg file. This is a generic interface for opening either an mpeg1 or
 * an mpeg4 video. If non-standard mpeg1 isn't supported (FFmpeg build > 4680), 
 * calling this function with "mpeg1" as codec results in an error. To create a
 * timelapse video, use TIMELAPSE_CODEC as codec name.
 */
struct ffmpeg *ffmpeg_open(
	struct context *cnt,
	char *ffmpeg_video_codec, 
	char *filename, 
	unsigned char *y,    /* YUV420 Y plane */
	unsigned char *u,    /* YUV420 U plane */
	unsigned char *v,    /* YUV420 V plane */
	int width,
	int height, 
	int rate,            /* framerate, fps */
	int bps,             /* bitrate; bits per second */
	int vbr              /* variable bitrate */
	);

/* Puts the image pointed to by the picture member of struct ffmpeg. */
void ffmpeg_put_image(struct context *cnt, struct ffmpeg *);

/* Puts the image defined by u, y and v (YUV420 format). */
void ffmpeg_put_other_image(
	struct context *cnt,
	struct ffmpeg *ffmpeg, 
	unsigned char *y, 
	unsigned char *u, 
	unsigned char *v
	);

/* Closes the mpeg file. */
void ffmpeg_close(struct ffmpeg *);

#endif /* _INCLUDE_FFMPEG_H_ */