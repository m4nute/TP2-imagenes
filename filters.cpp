#include <iostream>
#include <string>
#include <stdlib.h>
#include <math.h>
#include <vector>
#include "filters.h"
#include <thread>
#include <atomic>
#include "ppm.h"
#include <chrono>
#include <cmath>
#include <algorithm>
#include <semaphore.h>

#define BLACK 0

using namespace std;

// Filtro plano como ejemplo
void plain(ppm &img, unsigned char c)
{
	std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();

	for (int i = 0; i < img.height - 1; i++)
		for (int j = 0; j < img.width - 1; j++)
			img.setPixel(i, j, pixel(c, c, c));
	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now(); // Fin del timer
	std::chrono::duration<double> duration = end - start;						  // Duración en segundos

	std::cout << "Duración: " << duration.count() << " segundos" << std::endl;
}

void brightness(ppm &img, float b, int start, int end)
{
	std::chrono::steady_clock::time_point startTime = std::chrono::steady_clock::now();
	for (int i = 0; i < img.height; i++)
	{
		for (int j = 0; j < img.width; j++)
		{
			pixel p = img.getPixel(i, j);
			int brillo = static_cast<int>(p.r * b);
			int r = min(p.r + brillo, 255);
			int g = min(p.g + brillo, 255);
			int b = min(p.b + brillo, 255);

			img.setPixel(i, j, pixel(r, g, b));
		}
	}
	std::chrono::steady_clock::time_point endTime = std::chrono::steady_clock::now(); // Fin del timer
	std::chrono::duration<double> duration = endTime - startTime;					  // Duración en segundos

	std::cout << "Duración: " << duration.count() << " segundos" << std::endl;
}

void contrast(ppm &img, float contrast)
{
	std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();

	for (int i = 0; i < img.height; i++)
		for (int j = 0; j < img.width; j++)
		{
			pixel p = img.getPixel(i, j);

			int r_prima = p.r - 128;
			int g_prima = p.g - 128;
			int b_prima = p.b - 128;

			float f = (259 * (contrast + 255)) / (255 * (259 - contrast));

			float nuevo_r = f * r_prima + 128;
			float nuevo_g = f * g_prima + 128;
			float nuevo_b = f * b_prima + 128;

			if (nuevo_r < 0)
				nuevo_r = 0;
			else if (nuevo_r > 255)
				nuevo_r = 255;

			if (nuevo_g < 0)
				nuevo_g = 0;
			else if (nuevo_g > 255)
				nuevo_g = 255;

			if (nuevo_b < 0)
				nuevo_b = 0;
			else if (nuevo_b > 255)
				nuevo_b = 255;

			img.setPixel(i, j, pixel(nuevo_r, nuevo_g, nuevo_b));
		}

	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now(); // Fin del timer
	std::chrono::duration<double> duration = end - start;						  // Duración en segundos

	std::cout << "Duración: " << duration.count() << " segundos" << std::endl;
}

void blackWhite(ppm &img)
{
	std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
	for (int i = 0; i < img.height; i++)
	{
		for (int j = 0; j < img.width; j++)
		{
			pixel p = img.getPixel(i, j);
			int promedio = (p.r + p.g + p.b) / 3;
			img.setPixel(i, j, pixel(promedio, promedio, promedio));
		}
	}
	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now(); // Fin del timer
	std::chrono::duration<double> duration = end - start;						  // Duración en segundos

	std::cout << "Duración: " << duration.count() << " segundos" << std::endl;
}

void shades(ppm &img, unsigned char shades)
{
	std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();

	for (int i = 0; i < img.height; i++)
	{
		for (int j = 0; j < img.width; j++)
		{
			pixel p = img.getPixel(i, j);
			int rango = 255 / (shades - 1);
			int gprima = (p.r + p.g + p.b) / 3;
			int g = (gprima / rango) * rango;
			img.setPixel(i, j, pixel(g, g, g));
		}
	}
	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now(); // Fin del timer
	std::chrono::duration<double> duration = end - start;						  // Duración en segundos

	std::cout << "Duración: " << duration.count() << " segundos" << std::endl;
}

void boxBlur(ppm &img)
{
	std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
	ppm tempImg = img; // Crear una imagen temporal para almacenar los resultados
	for (int i = 1; i < img.height - 1; i++)
		for (int j = 1; j < img.width - 1; j++)
		{
			int sumR = 0, sumG = 0, sumB = 0;

			// Sumar los valores de los canales de los 9 píxeles vecinos
			for (int k = -1; k <= 1; k++)
			{
				for (int l = -1; l <= 1; l++)
				{
					pixel p = img.getPixel(i + k, j + l);
					sumR += p.r;
					sumG += p.r;
					sumB += p.b;
				}
			}
			tempImg.setPixel(i, j, pixel(sumR / 9, sumG / 9, sumB / 9));
		}
	img = tempImg;																  // Reemplazar la imagen original con la imagen filtrada
	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now(); // Fin del timer
	std::chrono::duration<double> duration = end - start;						  // Duración en segundos
	std::cout << "Duración: " << duration.count() << " segundos" << std::endl;
}

void sharpen(ppm &img)
{
	std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();

	ppm tempImg = img; // Crear una imagen temporal para almacenar los resultados

	// Iterar sobre cada píxel de la imagen
	for (int i = 1; i < img.height - 1; i++)
	{
		for (int j = 1; j < img.width - 1; j++)
		{
			int sumR = 0, sumG = 0, sumB = 0;

			// Aplicar el efecto del kernel a los píxeles vecinos
			for (int k = -1; k <= 1; k++)
			{
				for (int l = -1; l <= 1; l++)
				{
					pixel p = img.getPixel(i + k, j + l);
					int coefficient = 0;

					// Asignar el valor del coeficiente según la posición en el kernel
					if ((k == 0 && l == 0) || (k == 1 && l == 1) || (k == 2 && l == 0))
						coefficient = -1;
					else if ((k == 1 && l == 0) || (k == 1 && l == 2))
						coefficient = 5;

					sumR += coefficient * p.r;
					sumG += coefficient * p.g;
					sumB += coefficient * p.b;
				}
			}

			// Asegurarse de que los valores de los canales estén en el rango válido
			int newR = std::min(std::max(sumR, 0), 255);
			int newG = std::min(std::max(sumG, 0), 255);
			int newB = std::min(std::max(sumB, 0), 255);

			// Establecer el nuevo valor del píxel en la imagen temporal
			tempImg.setPixel(i, j, pixel(newR, newG, newB));
		}
	}

	img = tempImg; // Reemplazar la imagen original con la imagen filtrada

	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now(); // Fin del timer
	std::chrono::duration<double> duration = end - start;						  // Duración en segundos

	std::cout << "Duración: " << duration.count() << " segundos" << std::endl;
}

void edgeDetection(ppm &img)
{
	std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();

	for (int i = 0; i < img.height; i++)
	{
		for (int j = 0; j < img.width; j++)
		{
			pixel p = img.getPixel(i, j);
			int promedio = (p.r + p.g + p.b) / 3;
			img.setPixel(i, j, pixel(promedio, promedio, promedio));
		}
	}
	ppm tempImg = img; // Crear una imagen temporal para almacenar los resultados

	// Iterar sobre cada píxel de la imagen
	for (int i = 1; i < img.height - 1; i++)
	{
		for (int j = 1; j < img.width - 1; j++)
		{
			int sumR = 0, sumG = 0, sumB = 0;

			// Sumar los valores de los canales de los 9 píxeles vecinos
			for (int k = -1; k <= 1; k++)
			{
				for (int l = -1; l <= 1; l++)
				{
					pixel p = img.getPixel(i + k, j + l);
					sumR += p.r;
					sumG += p.r;
					sumB += p.b;
				}
			}

			tempImg.setPixel(i, j, pixel(sumR / 9, sumG / 9, sumB / 9));
		}
	}

	img = tempImg; // Reemplazar la imagen original con la imagen filtrada

	// Crear una nueva imagen en escala de grises para almacenar los bordes acentuados
	ppm result(img.width, img.height);

	// Kernel Sobel para detectar bordes verticales
	int sobelVertical[3][3] = {
		{1, 0, -1},
		{2, 0, -2},
		{1, 0, -1}};

	// Convolución de la imagen con el kernel Sobel
	for (int i = 1; i < img.height - 1; i++)
	{
		for (int j = 1; j < img.width - 1; j++)
		{
			int gx = 0;
			int gy = 0;

			// Calcular la convolución para el píxel actual
			for (int k = -1; k <= 1; k++)
			{
				for (int l = -1; l <= 1; l++)
				{
					// Obtener el valor del píxel en la posición correspondiente al kernel
					pixel p = img.getPixel(i + k, j + l);

					// Calcular la contribución del píxel al valor de convolución
					int value = sobelVertical[k + 1][l + 1];
					gx += value * p.r;
					gy += value * p.g;
					gy += value * p.b;
				}
			}

			// Calcular la magnitud del gradiente
			int magnitude = sqrt(gx * gx + gy * gy);

			// Establecer el valor de intensidad en la imagen de resultado
			result.setPixel(i, j, pixel(magnitude, magnitude, magnitude));
		}
	}

	// Reemplazar la imagen original con la imagen de resultado
	img = result;

	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now(); // Fin del timer
	std::chrono::duration<double> duration = end - start;						  // Duración en segundos

	std::cout << "Duración: " << duration.count() << " segundos" << std::endl;
}

void merge(ppm &img1, ppm &img2, float p1)
{
	std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
	float p2 = 1 - p1;
	ppm img_final = img1;

	for (int i = 0; i < img1.height - 1; i++)
	{
		for (int j = 0; j < img1.width - 1; j++)
		{ // Ambas img tienen la misma resolución
			pixel pixel1 = img1.getPixel(i, j);
			pixel pixel2 = img2.getPixel(i, j);

			float red = pixel1.r * p1 + p2 * pixel2.r;
			float green = pixel1.g * p1 + p2 * pixel2.g;
			float blue = pixel1.b * p1 + pixel2.b * p2;
			img_final.setPixel(i, j, pixel(round(red), round(green), round(blue)));
		}
	}
	img1 = img_final;
	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now(); // Fin del timer
	std::chrono::duration<double> duration = end - start;						  // Duración en segundos

	std::cout << "Duración: " << duration.count() << " segundos" << std::endl;
}

void multiMerge(ppm &img1, ppm &img2, float p1, unsigned int n)
{
	std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();

	float p2 = 1 - p1;
	ppm img_final = img1;

	// Function to be executed by each thread
	auto mergePixels = [&](int start_i, int end_i)
	{
		for (int i = start_i; i < end_i; i++)
		{
			for (int j = 0; j < img1.width - 1; j++)
			{ // Both images have the same resolution
				pixel pixel1 = img1.getPixel(i, j);
				pixel pixel2 = img2.getPixel(i, j);

				float red = pixel1.r * p1 + p2 * pixel2.r;
				float green = pixel1.g * p1 + p2 * pixel2.g;
				float blue = pixel1.b * p1 + pixel2.b * p2;
				img_final.setPixel(i, j, pixel(round(red), round(green), round(blue)));
			}
		}
	};

	// Split the loop iterations evenly among the threads
	int rowsPerThread = img1.height / n;
	std::vector<std::thread> threads;

	// Launch threads
	for (int t = 0; t < n; t++)
	{
		int startRow = t * rowsPerThread;
		int endRow = (t == n - 1) ? img1.height - 1 : startRow + rowsPerThread;
		threads.emplace_back(mergePixels, startRow, endRow);
	}

	// Wait for all threads to finish
	for (auto &thread : threads)
	{
		thread.join();
	}

	img1 = img_final;
	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now(); // Fin del timer
	std::chrono::duration<double> duration = end - start;						  // Duración en segundos

	std::cout << "Duración: " << duration.count() << " segundos" << std::endl;
}


// MULTI

void plainThreads(int iteration, int pixeles, ppm &img, unsigned char c, int resto)
{
	for (int i = iteration * pixeles; i < pixeles * (iteration + 1)+resto; i++)
		for (int j = 0; j < img.width; j++)
			img.setPixel(i, j, pixel(c, c, c));
}

void multiPlain(ppm &img, unsigned char c, unsigned int n)
{
	std::vector<std::thread> threads;
	std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
	int pixeles = img.height / n;
	int resto = img.height % n;

	for (int i = 0; i < n; ++i)
	{
		if (i == threads.size() - 1)
		{
			threads.emplace_back(plainThreads, i, pixeles, std::ref(img), c,resto);
		}
		else
			threads.emplace_back(plainThreads, i, pixeles, std::ref(img), c,0);
	}

	for (std::thread &thread : threads)
	{
		thread.join();
	}


	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now(); // Fin del timer
	std::chrono::duration<double> duration = end - start;						  // Duración en segundos

	std::cout << "Duración: " << duration.count() << " segundos" << std::endl;
}

void blackWhiteThreads(ppm &img, int startRow, int endRow)
{
	for (int i = startRow; i < endRow; i++)
	{
		for (int j = 0; j < img.width; j++)
		{
			pixel p = img.getPixel(i, j);
			int promedio = (p.r + p.g + p.b) / 3;
			img.setPixel(i, j, pixel(promedio, promedio, promedio));
		}
	}
}

void multiBlackWhite(ppm &img, unsigned int n)
{
	std::vector<std::thread> threads;
	std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
	int rowsPerThread = img.height / n;
	int remainingRows = img.height % n;
	int startRow = 0;
	int endRow = startRow + rowsPerThread;

	for (int t = 0; t < n; t++)
	{
		if (t == n - 1)
		{
			// Assign remaining rows to the last thread
			endRow += remainingRows - 1;
		}

		threads.emplace_back(blackWhiteThreads, std::ref(img), startRow, endRow);

		startRow = endRow;
		endRow = startRow + rowsPerThread;
	}

	for (auto &thread : threads)
	{
		thread.join();
	}

	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now(); // Fin del timer
	std::chrono::duration<double> duration = end - start;						  // Duración en segundos

	std::cout << "Duración: " << duration.count() << " segundos" << std::endl;
}

void contrastThreads(int iteration, int pixeles, ppm &img, float contrast,int resto)
{
	for (int i = iteration * pixeles; i < pixeles * (iteration + 1)+resto; i++)
		for (int j = 0; j < img.width; j++)
		{
			pixel p = img.getPixel(i, j);

			float f = (259 * (contrast + 255)) / (255 * (259 - contrast));

			int r_prima = p.r - 128;
			int g_prima = p.g - 128;
			int b_prima = p.b - 128;

			float nuevo_r = f * r_prima + 128;
			float nuevo_g = f * g_prima + 128;
			float nuevo_b = f * b_prima + 128;

			if (nuevo_r < 0)
			{
				nuevo_r = 0;
			}
			else if (nuevo_r > 255)
			{
				nuevo_r = 255;
			}

			if (nuevo_g < 0)
			{
				nuevo_g = 0;
			}
			else if (nuevo_g > 255)
			{
				nuevo_g = 255;
			}

			if (nuevo_b < 0)
			{
				nuevo_b = 0;
			}
			else if (nuevo_b > 255)
			{
				nuevo_b = 255;
			}

			img.setPixel(i, j, pixel(nuevo_r, nuevo_g, nuevo_b));
		}
}

void multiContrast(ppm &img, float contrast, unsigned int n)
{
	std::vector<std::thread> threads;
	std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
	int pixeles = img.height / n;
	int resto = img.height % n;

	for (int i = 0; i < n; ++i)
	{
		if (i == threads.size() - 1)
		{
			threads.emplace_back(contrastThreads, i, pixeles, std::ref(img), contrast, resto);
		}
		else
			threads.emplace_back(contrastThreads, i, pixeles, std::ref(img), contrast, 0);
	}

	for (std::thread &thread : threads)
	{
		thread.join();
	}

	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now(); // Fin del timer
	std::chrono::duration<double> duration = end - start;						  // Duración en segundos

	std::cout << "Duración: " << duration.count() << " segundos" << std::endl;
}

void shadesThreads(int iteration, int pixeles, ppm &img, unsigned char shades,int resto)
{
	for (int i = iteration * pixeles; i < pixeles * (iteration + 1); i++)
	{
		for (int j = 0; j < img.width; j++)
		{
			pixel p = img.getPixel(i, j);
			int rango = 255 / (shades - 1);
			int gprima = (p.r + p.g + p.b) / 3;
			int g = (gprima / rango) * rango;
			img.setPixel(i, j, pixel(g, g, g));
		}
	}
}

void multiShades(ppm &img, unsigned char shades, unsigned int n)
{
	std::vector<std::thread> threads;
	std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
	int pixeles = img.height / n;
	int resto = img.height % n;

	for (int i = 0; i < n; ++i)
	{
		if (i == threads.size() - 1)
		{
			threads.emplace_back(shadesThreads, i, pixeles, std::ref(img), shades, resto);
		}
		else
			threads.emplace_back(shadesThreads, i, pixeles, std::ref(img), shades, 0);
	}

	for (std::thread &thread : threads)
	{
		thread.join();
	}

	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now(); // Fin del timer
	std::chrono::duration<double> duration = end - start;						  // Duración en segundos

	std::cout << "Duración: " << duration.count() << " segundos" << std::endl;
}

void brightnessThreads(int iteration, int pixeles, ppm &img, float b, int resto)
{
	for (int i = iteration * pixeles; i < pixeles * (iteration + 1)+resto; i++)
		for (int j = 0; j < img.width; j++)
		{
			pixel p = img.getPixel(i, j);
			int brillo = static_cast<int>(p.r * b);
			int r = min(p.r + brillo, 255);
			int g = min(p.g + brillo, 255);
			int b = min(p.b + brillo, 255);

			img.setPixel(i, j, pixel(r, g, b));
		}
}

void multiBrightness(ppm &img, float b, int start, int end, unsigned int n)
{
	std::vector<std::thread> threads;
	std::chrono::steady_clock::time_point startTime = std::chrono::steady_clock::now();
	int pixeles = img.height / n;
	int resto = img.height % n;

	for (int i = 0; i < n; ++i)
	{
		if (i == threads.size() - 1)
		{
			threads.emplace_back(brightnessThreads, i, pixeles, std::ref(img), b, resto);
		}
		else
			threads.emplace_back(brightnessThreads, i, pixeles, std::ref(img), b, 0);
	}

	for (std::thread &thread : threads)
	{
		thread.join();
	}

	std::chrono::steady_clock::time_point endTime = std::chrono::steady_clock::now(); // Fin del timer
	std::chrono::duration<double> duration = endTime - startTime;					  // Duración en segundos

	std::cout << "Duración: " << duration.count() << " segundos" << std::endl;
}

void sharpenThread(ppm &tempImg, int startRow, int endRow)
{
	// Iterar sobre las filas asignadas al hilo
	for (int i = startRow; i < endRow; i++)
	{
		for (int j = 1; j < tempImg.width - 1; j++)
		{
			int sumR = 0, sumG = 0, sumB = 0;

			// Aplicar el efecto del kernel a los píxeles vecinos
			for (int k = -1; k <= 1; k++)
			{
				for (int l = -1; l <= 1; l++)
				{
					pixel p = tempImg.getPixel(i + k, j + l);
					int coefficient = 0;

					// Asignar el valor del coeficiente según la posición en el kernel
					if ((k == 0 && l == 0) || (k == 1 && l == 1) || (k == 2 && l == 0))
						coefficient = -1;
					else if ((k == 1 && l == 0) || (k == 1 && l == 2))
						coefficient = 5;

					sumR += coefficient * p.r;
					sumG += coefficient * p.g;
					sumB += coefficient * p.b;
				}
			}

			// Asegurarse de que los valores de los canales estén en el rango válido
			int newR = std::min(std::max(sumR, 0), 255);
			int newG = std::min(std::max(sumG, 0), 255);
			int newB = std::min(std::max(sumB, 0), 255);

			// Establecer el nuevo valor del píxel en la imagen temporal
			tempImg.setPixel(i, j, pixel(newR, newG, newB));
		}
	}
}

// Función principal para el procesamiento multi-hilo
void multiSharpen(ppm &img, unsigned int n)
{
	std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();

	ppm tempImg = img; // Crear una imagen temporal para almacenar los resultados

	std::vector<std::thread> threads;

	int rowsPerThread = img.height / n;
	int startRow = 1;
	int endRow = startRow + rowsPerThread;

	// Crear los hilos y asignar las filas correspondientes a cada hilo
	for (int i = 0; i < n - 1; i++)
	{
		threads.emplace_back(sharpenThread, std::ref(tempImg), startRow, endRow);
		startRow = endRow;
		endRow += rowsPerThread;
	}
	threads.emplace_back(sharpenThread, std::ref(tempImg), startRow, img.height - 1);

	// Esperar a que todos los hilos terminen su ejecución
	for (std::thread &thread : threads)
	{
		thread.join();
	}

	img = tempImg; // Reemplazar la imagen original con la imagen filtrada

	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now(); // Fin del timer
	std::chrono::duration<double> duration = end - start;						  // Duración en segundos

	std::cout << "Duración: " << duration.count() << " segundos" << std::endl;
}

int sobelVertical[3][3] = {
	{1, 0, -1},
	{2, 0, -2},
	{1, 0, -1}};

void boxBlurThreads(ppm img, ppm &tempImg, int startRow, int endRow)
{
	for (int i = startRow; i < endRow; i++)
	{
		for (int j = 1; j < img.width - 1; j++)
		{
			int sumR = 0, sumG = 0, sumB = 0;
			int count = 0;

			// Sumar los valores de los canales de los 9 píxeles vecinos
			for (int k = -1; k <= 1; k++)
			{
				for (int l = -1; l <= 1; l++)
				{
					int row = i + k;
					int col = j + l;

					if (row >= 0 && row < img.height && col >= 0 && col < img.width)
					{
						pixel p = img.getPixel(row, col);
						sumR += p.r;
						sumG += p.g;
						sumB += p.b;
						count++;
					}
				}
			}

			int avgR = sumR / count;
			int avgG = sumG / count;
			int avgB = sumB / count;
			tempImg.setPixel(i, j, pixel(avgR, avgG, avgB));
		}
	}
}

void multiBoxBlur(ppm &img, unsigned int n)
{
	std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
	ppm tempImg = img; // Crear una imagen temporal para almacenar los resultados

	std::vector<std::thread> threads;

	int rowsPerThread = img.height / n;
	int remainingRows = img.height % n;

	int startRow = 1;
	int endRow = startRow + rowsPerThread;

	for (int t = 0; t < n; t++)
	{
		if (t == n - 1)
		{
			// Assign remaining rows to the last thread
			endRow += remainingRows - 1;
		}

		threads.emplace_back(boxBlurThreads, img, std::ref(tempImg), startRow, endRow);

		startRow = endRow;
		endRow = startRow + rowsPerThread;
	}

	for (auto &thread : threads)
	{
		thread.join();
	}

	img = tempImg; // Reemplazar la imagen original con la imagen filtrada

	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now(); // Fin del timer
	std::chrono::duration<double> duration = end - start;						  // Duración en segundos
	std::cout << "Duración: " << duration.count() << " segundos" << std::endl;
}

void edgeDetectionThreads(ppm img, ppm &result, int startRow, int endRow)
{
	for (int i = startRow; i < endRow; i++)
		for (int j = 1; j < img.width - 1; j++)
		{
			int gx = 0;
			int gy = 0;
			for (int k = -1; k <= 1; k++)
			{
				for (int l = -1; l <= 1; l++)
				{
					int row = i + k;
					int col = j + l;

					if (row >= 0 && row < img.height && col >= 0 && col < img.width)
					{
						pixel p = img.getPixel(i + k, j + l);
						int value = sobelVertical[k + 1][l + 1];
						gx += value * p.r;
						gy += value * p.g;
						gy += value * p.b;
					}
				}
			}
			int magnitude = sqrt(gx * gx + gy * gy);
			result.setPixel(i, j, pixel(magnitude, magnitude, magnitude));
		}
}

void multiEdgeDetection(ppm &img, unsigned int n)
{
	std::vector<std::thread> threads;
	std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
	int rowsPerThread = img.height / n;
	int remainingRows = img.height % n;
	int startRow = 0;
	int endRow = startRow + rowsPerThread;

	for (int t = 0; t < n; t++)
	{
		if (t == n - 1)
		{
			endRow += remainingRows;
		}
		threads.emplace_back(blackWhiteThreads, std::ref(img), startRow, endRow);

		startRow = endRow;
		endRow = startRow + rowsPerThread;
	}

	for (std::thread &thread : threads)
	{
		thread.join();
	}

	startRow = 0;
	endRow = startRow + rowsPerThread;
	threads.clear();
	ppm tempImg = img; // Crear una imagen temporal para almacenar los resultados

	for (int t = 0; t < n; t++)
	{
		if (t == n - 1)
		{
			// Assign remaining rows to the last thread
			endRow += remainingRows - 1;
		}

		threads.emplace_back(boxBlurThreads, img, std::ref(tempImg), startRow, endRow);

		startRow = endRow;
		endRow = startRow + rowsPerThread;
	}

	for (auto &thread : threads)
	{
		thread.join();
	}

	img = tempImg; // Reemplazar la imagen original con la imagen filtrada
	ppm result(img.width, img.height);

	startRow = 0;
	endRow = startRow + rowsPerThread;

	threads.clear();

	for (int t = 0; t < n; t++)
	{
		if (t == n - 1)
		{
			endRow += remainingRows - 1;
		}
		threads.emplace_back(edgeDetectionThreads, img, std::ref(result), startRow, endRow);

		startRow = endRow;
		endRow = startRow + rowsPerThread;
	}

	for (std::thread &thread : threads)
	{
		thread.join();
	}
	img = result;
	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now(); // Fin del timer
	std::chrono::duration<double> duration = end - start;						  // Duración en segundos
	std::cout << "Duración: " << duration.count() << " segundos" << std::endl;



	
}

void gradiente(ppm &img, int dimension)
{
	std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
	int incremento;
	int valorRojo = 0;
	int equilibrio = 0;
	if (dimension == 1)
	{
		incremento = (img.height > 255) ? img.height / 255 : 1;
		if (img.height >= 255)
			incremento = img.height / 255;
		else
		{
			incremento = 1;
			equilibrio = (255 - img.height) / 2;
		}
		for (int i = 0; i < img.height; i++)
		{
			for (int j = 0; j < img.width; j++)
			{
				pixel p = img.getPixel(i, j);
				img.setPixel(i, j, pixel(valorRojo + equilibrio, p.g, p.b));
			}
			if (i % incremento == 0)
				valorRojo += 1;
			if (valorRojo > 255)
				valorRojo = 255;
		}
	}
	else
	{
		if (img.width >= 255)
			incremento = img.width / 255;
		else
		{
			incremento = 1;
			equilibrio = (255 - img.width) / 2;
		}
		for (int j = 0; j < img.width; j++)
		{
			for (int i = 0; i < img.height; i++)
			{
				pixel p = img.getPixel(i, j);
				img.setPixel(i, j, pixel(valorRojo + equilibrio, p.g, p.b));
			}
			if (j % incremento == 0)
				valorRojo += 1;
			if (valorRojo > 255)
				valorRojo = 255;
		}
	}
	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now(); // Fin del timer
	std::chrono::duration<double> duration = end - start;						  // Duración en segundos

	std::cout << "Duración: " << duration.count() << " segundos" << std::endl;
}