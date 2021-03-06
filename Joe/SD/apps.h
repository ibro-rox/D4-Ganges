#ifndef ANIMATION_H
#define ANIMATION_H

#include <stdio.h>
#include <inttypes.h>
#include "timer1.h"
#include "dirent.h"
#include "file.h"
#include "tty.h"
#include "pwm.h"
#include "dac.h"


namespace apps
{
	//static uint8_t image(const char *path);
	//static uint8_t image(struct dirent *ent);
	//static uint8_t animation(const char *path, const uint_t clr);
	static uint8_t wav(const char *path);
	static uint8_t wtxt(const char *path);
	static uint8_t rtxt(const char *path);
}

#if 0
static uint8_t apps::image(const char *path)
{
	using namespace display;

	fprintf(stderr, TTY_YELLOW "apps::image(): \'%s\'\n", path);
	FILE *fp = op::fopen(path, "r");
	if (fp == NULL) {
		fprintf(stderr, TTY_RED "apps::image(): open file failed: %u\n", errno);
		return 1;
	}
	for (uint_t r = 0; r < BUFF_H; r++)
		for (uint_t c = 0; c < BUFF_W / 8; c++) {
			buff[r][c][BuffRed] = fgetc(fp);
			buff[r][c][BuffGreen] = fgetc(fp);
		}
	op::fclose(fp);
	fputs(TTY_YELLOW "apps::image() finished\n", stderr);
	return 0;
	*/
}

static uint8_t apps::image(struct dirent *ent)
{
	/*
	using namespace display;

	fprintf(stderr, TTY_YELLOW "apps::image(): \'%s\'\n", ent->d_name);
	FILE *fp = op::fopen(ent, "r");
	if (fp == NULL) {
		fprintf(stderr, TTY_RED "apps::image(): open file failed: %u\n", errno);
		return 1;
	}
	for (uint_t r = 0; r < BUFF_H; r++)
		for (uint_t c = 0; c < BUFF_W / 8; c++) {
			buff[r][c][BuffRed] = fgetc(fp);
			buff[r][c][BuffGreen] = fgetc(fp);
		}
	op::fclose(fp);
	fputs(TTY_YELLOW "apps::image() finished\n", stderr);
	return 0;
	*/
}

static uint8_t apps::animation(const char *path, const uint_t clr)
{
	/*
	using namespace display;

	fprintf(stderr, TTY_YELLOW "apps::animation(): \'%s\'\n", path);
	FILE *fp = op::fopen(path, "r");
	if (fp == NULL) {
		fprintf(stderr, TTY_RED "apps::animation(): open file failed: %u\n", errno);
		return 1;
	}
	uint8_t cnt = 0;
	int ch;
	timer1::freq5();
	while ((ch = fgetc(fp)) != -1) {
		if (cnt++ % (30 / 5) == 0) {
			while (!timer1::over());
			timer1::clear();
			cnt = 1;
		}
		for (uint_t r = 0; r < BUFF_H; r++)
			for (uint_t c = 0; c < BUFF_W / 8; c++) {
				if (r != 0 || c != 0)
					ch = fgetc(fp);
				buff[r][c][BuffRed] = 0;
				if (clr & (Red << Foreground))
					buff[r][c][BuffRed] |= ch;
				if (clr & (Red << Background))
					buff[r][c][BuffRed] |= ~ch;
				buff[r][c][BuffGreen] = 0;
				if (clr & (Green << Foreground))
					buff[r][c][BuffGreen] |= ch;
				if (clr & (Green << Background))
					buff[r][c][BuffGreen] |= ~ch;
			}
		_delay_us(2000);
	}
	timer1::stop();
	op::fclose(fp);
	fputs(TTY_YELLOW "apps::animation() finished\n", stderr);
	return 0;
	*/
}
#endif

static uint8_t apps::wav(const char *path)
{
	printf("apps::wav(): \'%s\'\n", path);
	FILE *fp = op::fopen(path, "r");
	if (fp == NULL) {
		printf("apps::wav(): open file failed: %u\n", errno);
		return 1;
	}
	int ch;
	pwm::enable(true);
	timer1::freq8000();
	while ((ch = fgetc(fp)) != -1) {
		while (!timer1::over());
		timer1::clear();
		pwm::set(ch);
		set_dac(ch);
	}
	timer1::stop();
	pwm::enable(false);
	op::fclose(fp);
	fputs("apps::wav() finished\n", stderr);
	return 0;
}
// Joes code

// file creation always fails
// existing files can be opened
// writing data always fails (returns EOF)

static uint8_t apps::wtxt(const char *path)
{
	printf("apps::wtxt(): \'%s'\n", path );
	FILE *fp = op::fopen(path, "w"); // create a new file in write mode at the specified path
	if (fp == NULL){
		printf("file creation failed\n");
		return 1;
	}
	else{
		printf("file open success\n");

		uint8_t temp = fputc('1', fp);
		if (temp ==0){
			printf("File Write Success\n");
		}
		else{
			printf("File write failed\n");
			printf("%d\n",errno );
		}
	//fputs("testing, testing\n", fp);
	
	

	op::fclose(fp);		// close the file
	}

	/* // iostream and fstream don't appear to exist for avr
	std::ofstream myFile;
	myFile.open("example.txt");
	myFile << "your text here\n";
	myFile.close();
	*/
	return 0;
}

static uint8_t apps::rtxt(const char *path)
{
	printf("apps::rtxt(): \'%s'\n", path );
	FILE *fp = op::fopen(path, "r"); // openss file in read mode at the specified path
	if (fp == NULL){
		printf("file open failed\n");
		return 1;
	}

	else{
	printf("file open success\n");
	char ch = fgetc(fp); // produces ASCII code
	printf("data: %d\n",ch );

	}


}
#endif
