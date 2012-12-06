/*
 * segment_utils.c
 *
 *  Created on: Oct 16, 2012
 *      Author: chris
 */

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>


#include "segment_utils.h"
#include "input_handle.h"
#include "output_handle.h"
#include "chris_global.h"
#include "chris_error.h"
#include "segment_yy.h"

int parse_options_chris(int argc, char *argv[]) {

	int ret = 0;
	/*parse options*/
	int next_option;
	//short char option
	const char * const short_option = "vhm:d:t:p:n:r:w:e:v:a:s:c:0";

	//long char option struction array
	const struct option long_option[] = {
			{ "version", 0, NULL, 'v' },
			{ "help",	0, NULL, 'h' },
			{ "mode", 1, NULL, 'm' }, //execute mode
			{ "dir", 1, NULL, 'd' }, //ts storage directory
			{ "segment_time", 1, NULL, 't' }, //segment duration
			{ "prefix_ts", 1, NULL, 'p' }, //the prefix in the m3u8 file
			{ "m3u8_name", 1, NULL, 'n' }, //m3u8 name
			{ "frame_rate", 1, NULL, 'r' }, //frame rate
			{ "width", 1, NULL, 'w' }, //video width
			{ "height", 1, NULL, 'e' }, //video height
			{ "vb", 1, NULL, 'o' }, //video bitrate
			{ "ab", 1, NULL, 'a' }, //audio bitrate
			{ "sample", 1, NULL, 's' }, //audio sample
			{ "channel", 1, NULL, 'c' }, //audio channels
			{ "decollator", 0, NULL, '0' },
			{ NULL, 0, NULL, 0 }
	};

	do {

		next_option = getopt_long(argc, argv, short_option, long_option, NULL);

		switch (next_option) {

		case '0':
			ret ++;
			break;
		default:		//there is no options
			break;

		}
	} while (next_option != -1);

	return ret;
}

int parse_option_argument(Segment_U ** seg_union_ptr ,int argc, char *argv[]) {

	Segment_U * seg_union = NULL;
	int i = 0;
	/*parse options*/
	int next_option;
	//short char option
	const char * const short_option = "vhm:d:t:p:n:r:w:e:v:a:s:c:02:3:";

	//long char option struction array
	const struct option long_option[] = {
			{ "version", 0, NULL, 'v' },
			{ "help",	0, NULL, 'h' },
			{ "mode", 1, NULL, 'm' }, //execute mode
			{ "dir", 1, NULL, 'd' }, //ts storage directory
			{ "segment_time", 1, NULL, 't' }, //segment duration
			{ "prefix_ts", 1, NULL, 'p' }, //the prefix in the m3u8 file
			{ "m3u8_name", 1, NULL, 'n' }, //m3u8 name
			{ "frame_rate", 1, NULL, 'r' }, //frame rate
			{ "width", 1, NULL, 'w' }, //video width
			{ "height", 1, NULL, 'e' }, //video height
			{ "vb", 1, NULL, 'o' }, //video bitrate
			{ "ab", 1, NULL, 'a' }, //audio bitrate
			{ "sample", 1, NULL, 's' }, //audio sample
			{ "channel", 1, NULL, 'c' }, //audio channels
			{ "decollator", 0, NULL, '0' },		//indicator a new bitrate
			{ "num_in_dir", 1, NULL, '2' },
			{ "num_in_m3u8", 1, NULL, '3' },
			{ NULL, 0, NULL, 0 }
	};

	do {

		next_option = getopt_long(argc, argv, short_option, long_option, NULL);

		switch (next_option) {

		case 'v': 	//version
			printf("**********************************\n");
			printf("  segment version:%s \n\n", SEG_VERSION);
			exit(0);
		case 'h': //help
			fprintf(stderr,
					"\n**********************************\n"
					"--version			view the software version,with no argument\n"
					"--input			assign the input file ,at least one input file,at most 20 input files\n"
					"--mode 			you must assign the program work mode ,you can use 0:transcode ,1:vod ,2:live\n"
					"--dir				the directory where ts and m3u8 file storage\n"
					"--segment_time		the segment duration time in second\n"
					"--prefix_ts		the prefix of the ts file in the m3u8 file \n"
					"--m3u8_name		name of the m3u8 file\n"
					"--frame_rate		video frames per second\n"
					"--width			width of the video \n"
					"--height		    height of the video \n"
					"--vb				the bitrate of the video \n"
					"--ab				the bitrate of the audio\n"
					"--sample			the samples of the audio\n"
					"--channel			audio channel number\n"
					"--decollator		indicator a new program\n"
					"--num_in_dir		completed ts-file number in the directory\n"
					"--num_in_m3u8		ts record saved in the m3u8 list\n"
					"\n");
			exit(0);
//			break;
		case '0':
			if(i != 0){
				if( seg_union->mode_type == -1	||
						seg_union->m3u8_name == NULL ||
						seg_union->segment_duration == 0 ||
						seg_union->storage_dir == NULL ||
						seg_union->ts_prfix_name == NULL ||
						seg_union->frame_rate == 0 	||
						seg_union->width == 0 ||
						seg_union->height == 0	||
						seg_union->video_rate == 0 ||
						seg_union->audio_rate == 0 ||
						seg_union->sample  == 0 ||
						seg_union->channel == 0	||
						seg_union->num_in_dir == 0 ||
						seg_union->num_in_m3u8 == 0){

					printf("  Segment Invalid argument   ,please use"
																" '%s  --help '  to find some information\n", argv[0]);
					exit(SEG_INVALID_ARGUMENT);
				}

			}
			seg_union_ptr[i] = (Segment_U *)malloc(sizeof(Segment_U));
			if(seg_union_ptr[i] == NULL){
				printf("seg_union malloc failed .\n");
				exit(1);
			}
			seg_union = seg_union_ptr[i++];
			break;

		case '2':  //num_in_dir
			seg_union->num_in_dir = atoi(optarg);
			break;
		case '3':  //num_in_m3u8
			seg_union->num_in_m3u8 = atoi(optarg);
			break;
		case 'm': //the program work mode
			seg_union->mode_type = atoi(optarg);
			break;
		case 'd': //ts storage directory
			seg_union->storage_dir = optarg;
			break;
		case 't': //segment duration
			seg_union->segment_duration = atof(optarg);
			break;
		case 'p': //the ts file in the m3u8 file
			seg_union->ts_prfix_name = optarg;
			break;
		case 'n': //the m3u8 file
			seg_union->m3u8_name = optarg;
			break;
		case 'r': //frame rate
			seg_union->frame_rate = atoi(optarg);
			break;
		case 'w': //video width
			seg_union->width = atoi(optarg);
			break;
		case 'e': //video height
			seg_union->height = atoi(optarg);
			break;
		case 'o': //video bitrate 蛋疼的短选项

			if(strchr(optarg ,'k') == NULL)
				seg_union->video_rate = atoi(optarg);
			else
				seg_union->video_rate = atoi(optarg) * 1000;

			break;
		case 'a': //audio bitrate
			if(strchr(optarg ,'k') == NULL)
				seg_union->audio_rate = atoi(optarg);
			else
				seg_union->audio_rate = atoi(optarg) * 1000;

			break;
		case 's': //audio sample
			seg_union->sample = atoi(optarg);
			break;
		case 'c': //audio channels
			seg_union->channel = atoi(optarg);
			break;
		case '?':		//invalid options
			exit(0);
//			break;
		default:		//there is no options
			break;

		}
	} while (next_option != -1);


	/*	check the input argument valid or not*/
	if( seg_union->mode_type == -1	||
			seg_union->m3u8_name == NULL ||
			seg_union->segment_duration == 0 ||
			seg_union->storage_dir == NULL ||
			seg_union->ts_prfix_name == NULL ||
			seg_union->frame_rate == 0 	||
			seg_union->width == 0 ||
			seg_union->height == 0	||
			seg_union->video_rate == 0 ||
			seg_union->audio_rate == 0 ||
			seg_union->sample  == 0 ||
			seg_union->channel == 0	||
			seg_union->num_in_dir == 0 ||
			seg_union->num_in_m3u8 == 0){

		printf("  Segment Invalid argument   ,please use"
													" '%s  --help '  to find some information\n", argv[0]);
		exit(SEG_INVALID_ARGUMENT);
	}

	return i;
}

void create_directory(char *storage_dir) {

	DIR *dir = opendir(storage_dir);
	if (dir == NULL) {

		int tmp_ret = mkdir(storage_dir, 0777);
		if (tmp_ret != 0) {
			printf("mkdir  directory failed\n");
			exit(CREAT_M3U8_DIR_FAIL);
		}
	} else {
		closedir(dir);
	}

}

void create_m3u8_name(Segment_U * seg_union ){
	//add storage dir in the ts_name
	if( seg_union->storage_dir[strlen(seg_union->storage_dir) -1 ] == '/'){ //storage end with '/'

		sprintf(seg_union->full_m3u8_name ,"%s" ,seg_union->storage_dir);
	}else{
		sprintf(seg_union->full_m3u8_name ,"%s/" ,seg_union->storage_dir);
	}

	sprintf(&(seg_union->full_m3u8_name[strlen(seg_union->full_m3u8_name)]) ,"%s" ,seg_union->m3u8_name);

}

void create_first_ts_name(Segment_U * seg_union ,int mode_type){

	//add storage dir in the ts_name
	if( seg_union->storage_dir[strlen(seg_union->storage_dir) -1 ] == '/'){ //storage end with '/'

		sprintf(seg_union->ts_name ,"%s" ,seg_union->storage_dir);
	}else{
		sprintf(seg_union->ts_name ,"%s/" ,seg_union->storage_dir);
	}

	seg_union->dir_name_len = strlen(seg_union->ts_name);
	//structure  the first ts name
	if(mode_type == YY_TRANSCODE){

		sprintf(&(seg_union->ts_name[strlen(seg_union->ts_name)]) ,"%s.ts" ,seg_union->ts_prfix_name);
	}else if(mode_type == YY_VOD){

		sprintf(&(seg_union->ts_name[strlen(seg_union->ts_name)]) ,"%s-%d.ts" ,seg_union->ts_prfix_name ,++seg_union->segment_no);
	}else if (mode_type == YY_LIVE){

		sprintf(&(seg_union->ts_name[strlen(seg_union->ts_name)]) ,"%s-%d.ts" ,seg_union->ts_prfix_name ,++seg_union->segment_no);
	}


}


void record_segment_time(Output_Context *ptr_output_ctx){

	if(ptr_output_ctx->start_time_mark == 0){
		ptr_output_ctx->start_time_mark = 1;
//		printf("混蛋。。。、\n");
		ptr_output_ctx->prev_segment_time = av_q2d(ptr_output_ctx->video_stream->time_base) *
													(ptr_output_ctx->pkt.pts )
													- (double)ptr_output_ctx->ptr_format_ctx->start_time / AV_TIME_BASE;

		printf("ptr_output_ctx->prev_segment_time = %f \n" ,ptr_output_ctx->prev_segment_time);

	}

	ptr_output_ctx->curr_segment_time =
						av_q2d(ptr_output_ctx->video_stream->time_base) *
										(ptr_output_ctx->pkt.pts )
										- (double)ptr_output_ctx->ptr_format_ctx->start_time / AV_TIME_BASE;

//	ptr_output_ctx->curr_segment_time =
//						av_q2d(ptr_output_ctx->video_stream->time_base) *
//						ptr_output_ctx->video_stream->pts.val
//										- (double)ptr_output_ctx->ptr_format_ctx->start_time / AV_TIME_BASE;
//	printf("ptr_output_ctx->prev_segment_time = %f \n" ,ptr_output_ctx->curr_segment_time);

//	//time meet
	if(ptr_output_ctx->curr_segment_time - ptr_output_ctx->prev_segment_time >= ptr_output_ctx->segment_duration){
		printf("...meet time .. ,duration = %f ,start_time = %f .\n" ,ptr_output_ctx->curr_segment_time - ptr_output_ctx->prev_segment_time ,
				(double)ptr_output_ctx->ptr_format_ctx->start_time / AV_TIME_BASE);
		avio_flush(ptr_output_ctx->ptr_format_ctx->pb);
		avio_close(ptr_output_ctx->ptr_format_ctx->pb);

		printf("complete the %d.ts ,and write the m3u8 file..\n" ,ptr_output_ctx->segment_no);
		write_m3u8_body( ptr_output_ctx ,ptr_output_ctx->curr_segment_time - ptr_output_ctx->prev_segment_time);




		//concat ts file name
		sprintf(&(ptr_output_ctx->ts_name[ptr_output_ctx->dir_name_len]) ,"%s-%d.ts" ,ptr_output_ctx->ts_prfix_name ,++ptr_output_ctx->segment_no);
		if (avio_open(&(ptr_output_ctx->ptr_format_ctx->pb), ptr_output_ctx->ts_name, AVIO_FLAG_WRITE) < 0) {
			fprintf(stderr, "Could not open '%s'\n", ptr_output_ctx->ts_name);
			exit(OPEN_MUX_FILE_FAIL);
		}

		ptr_output_ctx->prev_segment_time = ptr_output_ctx->curr_segment_time;   //place here
	}

}


void write_m3u8_header(Output_Context *ptr_output_ctx){
	//vod
	if(ptr_output_ctx->mode_type  == YY_VOD){
		ptr_output_ctx->fp_m3u8 = fopen(ptr_output_ctx->full_m3u8_name, "wb+");

		if(ptr_output_ctx->fp_m3u8 == NULL){
			fprintf(stderr ,"Could not open m3u8 file %s...\n" ,ptr_output_ctx->full_m3u8_name);
			exit(OPEN_M3U8_FAIL);
		}

		//write header
		fprintf(ptr_output_ctx->fp_m3u8 ,"#EXTM3U\n#EXT-X-TARGETDURATION:%.02f\n#EXT-X-MEDIA-SEQUENCE:1\n" ,ptr_output_ctx->segment_duration);

		fclose(ptr_output_ctx->fp_m3u8);
	}
}


/*	round function	*/
static int segmenter_duration_round(double src_num){

	int tmp_num = (int)src_num;  //acquire the integer of the double data

	double src_num1 = src_num + 0.5;
	int target_num = (int)src_num1;
	if(target_num > tmp_num){
		return target_num;
	}else{
		return tmp_num;
	}

}

void write_m3u8_body(Output_Context *ptr_output_ctx ,double segment_duration){

	printf("=====segment_duration = %f \n" ,segment_duration);
//	while(1);

	if (ptr_output_ctx->mode_type == YY_LIVE) { //live

		ptr_output_ctx->fp_m3u8 = fopen(ptr_output_ctx->full_m3u8_name, "wb+");

		if (ptr_output_ctx->fp_m3u8 == NULL) {
			fprintf(stderr, "Could not open m3u8 file %s...\n",
					ptr_output_ctx->full_m3u8_name);
			exit(OPEN_M3U8_FAIL);
		}

		int live_buf_index;

		/*	header in m3u8	*/
		if (ptr_output_ctx->segment_no <= ptr_output_ctx->num_in_m3u8) { //amite-1.ts

			snprintf(ptr_output_ctx->live_write_buf, 1024,
					"#EXTM3U\n#EXT-X-VERSION:3\n#EXT-X-ALLOW-CACHE:NO\n#EXT-X-TARGETDURATION:%d\n#EXT-X-MEDIA-SEQUENCE:1\n",
					segmenter_duration_round(ptr_output_ctx->segment_duration));

		} else {		// the first ts file have been removed
			snprintf(ptr_output_ctx->live_write_buf, 1024,
					"#EXTM3U\n#EXT-X-VERSION:3\n#EXT-X-ALLOW-CACHE:NO\n#EXT-X-TARGETDURATION:%d\n#EXT-X-MEDIA-SEQUENCE:%d\n",
					segmenter_duration_round((int) ptr_output_ctx->segment_duration),
					ptr_output_ctx->segment_no - ptr_output_ctx->num_in_m3u8
							+ 1);
		}
		live_buf_index = strlen(ptr_output_ctx->live_write_buf);

		int j;
		/*	context in m3u8	*/
		//situation one
		if (ptr_output_ctx->segment_no <= ptr_output_ctx->num_in_m3u8) {

			ptr_output_ctx->seg_duration_arr[ptr_output_ctx->segment_no] =
					segment_duration;

			for (j = 1; j <= ptr_output_ctx->segment_no; j++) {

				if (bool_interger)
					snprintf(ptr_output_ctx->live_write_buf + live_buf_index,
							1024, "#EXTINF:%d,\n%s-%u.ts\n",
							segmenter_duration_round(
									ptr_output_ctx->seg_duration_arr[j]), ptr_output_ctx->ts_prfix_name,
							j);
				else
					snprintf(ptr_output_ctx->live_write_buf + live_buf_index,
							1024, "#EXTINF:%.02f,\n%s-%u.ts\n",
							ptr_output_ctx->seg_duration_arr[j], ptr_output_ctx->ts_prfix_name,
							j);

				//update live_buf_index
				live_buf_index = strlen(ptr_output_ctx->live_write_buf);
			}

			if (fwrite(ptr_output_ctx->live_write_buf,
					strlen(ptr_output_ctx->live_write_buf), 1,
					ptr_output_ctx->fp_m3u8) != 1) {
				fprintf(stderr,
						"Could not write to m3u8 index file, will not continue writing to index file\n");
				fclose(ptr_output_ctx->fp_m3u8);
				exit(WRITE_M3U8_FAIL);
			}
			printf("#chirs :success write the m3u8 file\n");
		} else { ////situation second
			/*先对数组中保存的ts的时间长度进行更新*/

//===========================
			for (j = 1; j < ptr_output_ctx->num_in_m3u8; j++) {

				ptr_output_ctx->seg_duration_arr[j] =
						ptr_output_ctx->seg_duration_arr[j + 1]; //元素前移

			}
			ptr_output_ctx->seg_duration_arr[ptr_output_ctx->num_in_m3u8] =
					segment_duration; //处理数组最后一个成员

			for (j = ptr_output_ctx->segment_no - ptr_output_ctx->num_in_m3u8
					+ 1; j <= ptr_output_ctx->segment_no; j++) { //last_segment - sf_segmenter_handle_union->m3u8_ts_num + 1 从这开始 没错

				if (bool_interger)
					snprintf(ptr_output_ctx->live_write_buf + live_buf_index,
							1024, "#EXTINF:%d,\n%s-%u.ts\n",
							segmenter_duration_round(
									ptr_output_ctx->seg_duration_arr[ptr_output_ctx->num_in_m3u8
											- (ptr_output_ctx->segment_no - j)]), ptr_output_ctx->ts_prfix_name,
							j);
				else
					snprintf(ptr_output_ctx->live_write_buf + live_buf_index,
							1024, "#EXTINF:%.02f,\n%s-%u.ts\n",
							ptr_output_ctx->seg_duration_arr[ptr_output_ctx->num_in_m3u8
									- (ptr_output_ctx->segment_no - j)], ptr_output_ctx->ts_prfix_name, j);

				//拼接
				//update live_buf_index
				live_buf_index = strlen(ptr_output_ctx->live_write_buf);
			}
///===============================
			if (fwrite(ptr_output_ctx->live_write_buf,
					strlen(ptr_output_ctx->live_write_buf), 1,
					ptr_output_ctx->fp_m3u8) != 1) {
				fprintf(stderr,
						"Could not write to m3u8 index file, will not continue writing to index file\n");
				fclose(ptr_output_ctx->fp_m3u8);
				exit(WRITE_M3U8_FAIL);
			}
			printf("#chirs :success write the m3u8 file\n");
		}

		fclose(ptr_output_ctx->fp_m3u8);

	} else if (ptr_output_ctx->mode_type == YY_VOD) { //vod

		ptr_output_ctx->fp_m3u8 = fopen(ptr_output_ctx->full_m3u8_name, "ab+");

		if (ptr_output_ctx->fp_m3u8 == NULL) {
			fprintf(stderr, "Could not open m3u8 file %s...\n",
					ptr_output_ctx->full_m3u8_name);
			exit(OPEN_M3U8_FAIL);
		}
		fprintf(ptr_output_ctx->fp_m3u8, "#EXTINF:%.02f,\n%s-%u.ts\n",
				segment_duration, ptr_output_ctx->ts_prfix_name,
				ptr_output_ctx->segment_no);

		fclose(ptr_output_ctx->fp_m3u8);
	}

}

void write_m3u8_tailer(Output_Context *ptr_output_ctx){

	//vod
	if(ptr_output_ctx->mode_type  == YY_VOD){

		double tailer_duration = ptr_output_ctx->sync_ipts - ptr_output_ctx->prev_segment_time;
		ptr_output_ctx->fp_m3u8 = fopen(ptr_output_ctx->full_m3u8_name, "ab+");

		if(ptr_output_ctx->fp_m3u8 == NULL){
			fprintf(stderr ,"Could not open m3u8 file %s...\n" ,ptr_output_ctx->full_m3u8_name);
			exit(OPEN_M3U8_FAIL);
		}
		fprintf(ptr_output_ctx->fp_m3u8 ,"#EXTINF:%.02f,\n%s-%u.ts\n#EXT-X-ENDLIST\n" ,tailer_duration ,ptr_output_ctx->ts_prfix_name ,
									ptr_output_ctx->segment_no);

		fclose(ptr_output_ctx->fp_m3u8);
	}
}
