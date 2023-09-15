#ifndef FILTERS_H
#define FILTERS_H

#include <iostream>
#include <string>
#include <stdlib.h>
#include <vector>
#include "ppm.h"
#include <atomic>

// FILTROS SINGLE-THREAD

void blackWhite(ppm &img);
void contrast(ppm &img, float contrast);
void brightness(ppm &img, float b, int start, int end);
void shades(ppm &img, unsigned char shades);
void merge(ppm &img1, ppm &img2, float alpha);
void boxBlur(ppm &img);
void sharpen(ppm &img);
void edgeDetection(ppm &img); // ppm &img_target, sacamos este argumento por ahora
void plain(ppm &img, unsigned char c);
void gradiente(ppm &img, int dimension);
void vintage(ppm &img, float brillo, int start, int end);
void kaleidoscope(ppm &img);
void emboss(ppm &img, int start, int end);
void canvas(ppm &img); //a
// FILTROS MULTI-THREAD

void multiPlain(ppm &img, unsigned char c, unsigned int n);
void multiBlackWhite(ppm &img, unsigned int n);
void multiContrast(ppm &img, float contrast, unsigned int n);
void multiShades(ppm &img, unsigned char shades, unsigned int n);
void multiBrightness(ppm &img, float b, int start, int end, unsigned int n);
void multiSharpen(ppm &img, unsigned int n);
void multiBoxBlur(ppm &img, unsigned int n);
void multiEdgeDetection(ppm &img, unsigned int n);
void multiMerge(ppm &img1, ppm &img2, float alpha, unsigned int n);
void multiVintage(ppm &img, unsigned int n, float brillo);
void multiKaleidoscope(ppm &img, unsigned int n);
void multiEmboss(ppm &img, unsigned int n);
void canvas(ppm &img, unsigned int n);
// Completar :)

#endif
