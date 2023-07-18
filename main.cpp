
#include <stdio.h>
#include <string.h>

#include <stdint.h>
#include "util/utf8_to_utf32.h"
#include "schrift.h"

#include<cmath>
#include<Eigen/Core>
#include<Eigen/Dense>
#include<iostream>

#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>

using namespace cv;
using namespace std;

#define PI 3.1415926f
#define DEG2RAD (PI/180.0f)

#define END(m) do { fprintf(stderr, "%s\n", m); return 1; } while (0)

char pixels[800*800*4];

static int glyph_to_img(SFT_Image *img, SFT *sft, unsigned long cp)
{
#define ABORT(cp, m) do { fprintf(stderr, "codepoint 0x%04lX %s\n", cp, m); return -1; } while (0)

	SFT_Glyph gid;  //  unsigned long gid;

    // printf("sft: %d, %d\n", sft->xOffset, sft->yOffset);
    // printf("gid: %d\n", gid);
	if (sft_lookup(sft, cp, &gid) < 0) {
        printf("codepoint 0x%04lX %s\n", cp, "missing");
		ABORT(cp, "missing");
    }
    // gid = 2158;
    printf("gid: %d\n", gid);

	SFT_GMetrics mtx;
	if (sft_gmetrics(sft, gid, &mtx) < 0) {
        printf("codepoint 0x%04lX %s\n", cp, "bad glyph metrics");
		ABORT(cp, "bad glyph metrics");
    }

    img->width = (mtx.minWidth + 3) & ~3;
    img->height = mtx.minHeight;

	// char pixels[img->width * img->height];
	// char *pixels = malloc(img.width * img.height);
	// img->pixels = pixels;
	if (sft_render(sft, gid, *img) < 0) {
        printf("codepoint 0x%04lX %s\n", cp, "not rendered");
		ABORT(cp, "not rendered");
    }

	// printf("img. width[%d], height[%d]\n", img.width, img.height);
	// printf("pixels:\n");
	// for (int p=0; p<img.width * img.height; p++) {
	// 	printf("%d, ", pixels[p]);
	// }
	// printf("\n");

    return 0;
}


int main()
{
	printf("start:\n");
	int s = 10;
	// int s = 4;
    cv::Mat image;
	
	SFT sft = {
		.xScale = 16*s,
		.yScale = 16*s,
		// .xOffset = 2000,
		// .yOffset = 2000,
		.flags  = SFT_DOWNWARD_Y,
	};
	// sft.font = sft_loadfile("resources/FiraGO-Regular_extended_with_NotoSansEgyptianHieroglyphs-Regular.ttf");
	// sft.font = sft_loadfile("resources/Ubuntu-Regular.ttf");
	// sft.font = sft_loadfile("D:/ISP/vector_font/libschrift_opencv/resources/方正书宋简体.ttf");
	// sft.font = sft_loadfile("D:/ISP/vector_font/libschrift_opencv/resources/fz.ttf");
	// sft.font = sft_loadfile("D:/ISP/vector_font/libschrift_opencv/resources/Alibaba-PuHuiTi-Light.ttf");
	// sft.font = sft_loadfile("D:/ISP/vector_font/libschrift_opencv/resources/Alibaba-PuHuiTi-Light2.ttf");
	// sft.font = sft_loadfile("D:/ISP/vector_font/libschrift_opencv/resources/SourceHanSansSC-Regular.ttf");
	// sft.font = sft_loadfile("D:/ISP/vector_font/libschrift_opencv/resources/systr.ttf");
	// sft.font = sft_loadfile("D:/ISP/vector_font/libschrift_opencv/resources/FZSSJW--GB1-0.ttf");
	// sft.font = sft_loadfile("D:/ISP/vector_font/libschrift_opencv/resources/R.ttf");
	sft.font = sft_loadfile("D:/ISP/vector_font/libschrift_opencv/resources/test2.ttf");
	// sft.font = sft_loadfile("D:/ISP/vector_font/libschrift_opencv/resources/R1.ttf");
	// sft.font = sft_loadfile("resources/FZSSJW--GB1-0.ttf");
	if (sft.font == NULL)
		END("TTF load failed");

	// FILE *file = fopen("D:/ISP/vector_font/libschrift_opencv/resources/glass.utf8", "r");
	FILE *file = fopen("D:/ISP/vector_font/libschrift_opencv/resources/chinese.utf8", "r");
	if (file == NULL)
		END("Cannot open input text");

	// SFT_LMetrics lmtx;
	// sft_lmetrics(&sft, &lmtx);
	// int y = 20 + lmtx.ascender + lmtx.lineGap;

	static int count = 0;
	char text[20][256];
	char text2[256];
    int line_max = 0;
	while (fgets(text[line_max], sizeof(text[line_max]), file)) {
        line_max++;
    }
		// printf("line_max: %d, %d\n", line_max, line_max);

    int i = 0;
    int j = 0;
    int key = 0;
    // namedWindow("image", 0);
    while(key != 27)
    {
        memcpy(text2, text[i], sizeof(text2));
		int n = strlen(text2) - 1;
		text2[n] = 0;  // '\n' => len>0
		// printf("[i:%d, j:%d]: %d, %d\n", i, j, sizeof(text2), n);

		unsigned codepoints[sizeof(text2)];
		// n = utf8_to_utf32((unsigned char *) text2, codepoints, sizeof(text2));  // (const uint8_t *)
        // text2[0] = 0x31;
        // text2[0] = 0xe6;
        // text2[0] = 0xe6;
		n = utf8_to_utf32((unsigned char *) text2, codepoints, sizeof(text2));  // (const uint8_t *)
		// printf("[line:%d, n:%d]\n", count, n);
        count++;

        memset(pixels, 0, sizeof(pixels));
        // Mat image_black = Mat(800, 800, CV_8UC1, pixels, 0);
        // cv::imshow("image", image_black);

        SFT_Image ucImg;
        ucImg.pixels = pixels;
        glyph_to_img(&ucImg, &sft, codepoints[j]);
        // glyph_to_img(&ucImg, &sft, 0x6211);
        // glyph_to_img(&ucImg, &sft, 0x01);
		printf("[i:%d, j:%d]: %d, %d\n", i, j, n, codepoints[j]);

        if (ucImg.width != 0 && ucImg.height != 0) {
            // printf("ucImg. width[%d], height[%d]\n", ucImg.width, ucImg.height);
            // printf("pixels:\n");
            // char *pixels = (char*)ucImg.pixels;
            // for (int p=0; p<ucImg.width * ucImg.height; p++) {
            //     printf("%d, ", pixels[p]);
            // }
            // printf("\n");
            image = Mat(ucImg.height, ucImg.width, CV_8UC1, ucImg.pixels, 0);
            if(image.data == nullptr) {
                cerr << "image is null!" << endl;
            } else {
                Mat img_resize;
                // resize(image, img_resize, Size(image.cols*5, image.rows*5), 0, 0, INTER_LINEAR);
                // resize(image, img_resize, Size(image.cols*1, image.rows*1), 0, 0, INTER_LINEAR);
                cv::imshow("image", image);
                // cv::resizeWindow("image", img_resize.cols, img_resize.rows);
            }
        } else {
            image = Mat(200, 200, CV_8UC1, pixels, 0);
            cv::imshow("image", image);
        }
        // cv::waitKey(0);
        // key = cv::waitKeyEx(100);
        key = cv::waitKeyEx(1000);
        if (key == 'w') {
            // printf("w\n");
            i = (i<=0) ? 0 : (i-1);
        } else if (key == 's') {
            // printf("s\n");
            i = ((i+1)>=line_max) ? (line_max-1) : (i+1);
        } else if (key == 'a') {
            // printf("a\n");
            j = (j<=0) ? 0 : (j-1);
        } else if (key == 'd') {
            // printf("d\n");
            j = ((j+1)>=n) ? (n-1) : (j+1);
        }
        // printf("key:%d\n", key);

		// y += 2 * (lmtx.ascender + lmtx.descender + lmtx.lineGap);
	}

    if(image.data == nullptr) {
        cerr << "image is null!" << endl;
	    fclose(file);
        return 0;
    }
    cv::imshow("image", image);
    cv::waitKey(0);

	fclose(file);

	sft_freefont(sft.font);
	printf("end!\n");

    return 0;
}