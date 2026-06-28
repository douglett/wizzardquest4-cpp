#pragma once
#include "raylib.h"
#include <iostream>
using namespace std;

struct Screen {
	int width = 200, height = 200;
	string winname = "mygame";
	float zoom = 3;
	Color bgcolor = BLACK;
	Camera2D camera = {0};

	int init() {
		SetTraceLogLevel(LOG_WARNING);
		InitWindow(width*zoom, height*zoom, winname.c_str());
		SetTargetFPS(60);
		camera.zoom = zoom;
		// ok
		printf("Screen initialized: %d %d\n", width, height);
		begin();
		return 0;
	}

	void destroy() {
		CloseWindow();  // Close window and OpenGL context
	}

	// bool shouldQuit() {
	// 	return WindowShouldClose();
	// }

	// begin drawing mode (automatic)
	void begin() {
		BeginDrawing();
		BeginMode2D(camera);
		ClearBackground(bgcolor);
	}

	// flip backbuffer to screen
	void flip() {
		// show framerate
		auto fps = to_string(GetFPS());
		DrawText(fps.c_str(), 1, 1, 10, GREEN);
		// Qbfont.Text(fps, scr.Width - len(fps)*4 - 2, 2, ray.Green)
		// flip
		EndMode2D();
		EndDrawing();
		// begin drawing mode for next frame
		begin();
	}
};
