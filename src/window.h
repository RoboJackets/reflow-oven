#pragma once

#include "button.h"

void mainWindow();

void start();
void run();

void profiles();
void addProfile();
void editProfile();

void custom();

void settings();

void graphProfile(int x, int y, int w, int h, Profile profile, bool detailed);
void viewTemp(int x, int y);
void keyboard(int x, int y, int w, int h, int r, int c, char (*KB)[11]);
char keyboardB(int x, int y, int w, int h,int r, int c, char (*KB)[11], TSPoint p);
