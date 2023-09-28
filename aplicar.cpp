#include <iostream>
#include <string>
#include <stdlib.h>
#include <vector>
#include "filters.h"
#include "aplicar.h"
#include <thread>
#include <unistd.h>
#include <fstream>
#include "ppm.h"
#define ONE_OVER_BILLION 1E-9
using namespace std;

int aplicar(string filter, float p1, string imgString, int n, string out, ppm img2)
{
	ppm img(imgString);
	
	if (filter == "plain") {
		if (p1 < 0 && p1 > 255) {
			cout << "El parametro debe ser menor o igual a 255 y mayor o igual a 0" << endl;
			return 0;
		}
		if (n >= 2) multiPlain(std::ref(img), (unsigned char)p1, n);
		else plain(std::ref(img), (unsigned char)p1);
	}
	else if (filter == "blackWhite") {
		if (n >= 2) multiBlackWhite(std::ref(img), n);
		else blackWhite(std::ref(img));
	}
	else if (filter == "shades") {
		if (p1 < 1) {
			cout << "El parametro debe ser un numero entero mayor o igual a 1" << endl;
			return 0;
		}
		if (n >= 2) multiShades(std::ref(img), (unsigned char)p1, n);
		else shades(std::ref(img), (unsigned char)p1);
	}
	else if (filter == "contrast") {
		if (p1 < 0 && p1 > 1) {
			cout << "El parametro debe ser un numero decimal 0 / 1" << endl;
			return 0;
		}
		if (n >= 2) multiContrast(std::ref(img), p1, n);
		else contrast(std::ref(img), p1);
	}
	else if (filter == "brightness") {
		if (p1 -1 && p1 > 1) {
			cout << "El parametro debe ser un decimal entre -1 / 1" << endl;
			return 0;
		}
		if (n >= 2) multiBrightness(std::ref(img), p1, -1, 1, n);
		else brightness(std::ref(img), p1, -1, 1);
	}
	else if (filter == "boxBlur") {
		if (n >= 2) multiBoxBlur(std::ref(img), n);
		else boxBlur(std::ref(img));
	}
	else if (filter == "sharpen") {
		if (n >= 2) multiSharpen(std::ref(img), n);
		else sharpen(std::ref(img));
	}
	else if (filter == "edgeDetection") {
		if (n >= 2) multiEdgeDetection(std::ref(img), n);
		else edgeDetection(std::ref(img));
	}
	else if (filter == "merge") {
		if (p1 < 0 && p1 > 1) {
			cout << "El parametro debe estar entre 0 / 1";
			return 0;
		}
		if (img.height != img2.height || img.width != img2.width) {
			cout << "Las imagenes deben tener las mismas dimensiones";
			return 0;
		}
		if (n >= 2) multiMerge(std::ref(img), img2, p1, n);
		else merge(std::ref(img), img2, p1);
	}
	else if (filter == "gradiente") {
		if(p1 != 0 && p1 != 1) {
			cout << "El parametro debe ser 0 / 1";
			return 0;
		}
		gradiente(std::ref(img), p1);
	}
	else if (filter=="vintage"){
		if (p1 -1 && p1 > 1) {
			cout << "El parametro debe ser un decimal entre -1 / 1" << endl;
			return 0;
		}
		if (n >= 2) multiVintage(std::ref(img), p1, n);
		else vintage(std::ref(img), p1, -1,1);
	}
	else if (filter=="emboss"){
		if (n >= 2) multiEmboss(std::ref(img), n);
		else emboss(std::ref(img), -1, 1);
	}
	else if (filter=="kaleidoscope"){
		if (n >= 2) multiKaleidoscope(std::ref(img), n);
		else kaleidoscope(std::ref(img));
	}
	else if (filter=="canvas"){
		if (n >= 2) multiCanvas(std::ref(img), n, -1,1);
		else canvas(std::ref(img));
	}
	else {
		cout << "Filtro no existe..." << endl;
		return 0;
	}

	cout << "Escribiendo imagen" << endl;
	
	img.write(out);

	cout << "Listo" << endl;
	return 0;
}