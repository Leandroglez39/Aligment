// Algoritmos.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <chrono>
#include <iostream>
#include <iomanip>
#include "Input.h"
#include "Tools.h"
#include <omp.h>



using namespace std;





int main()  
{
	omp_set_num_threads(4);


#pragma omp parallel
	{
		cout << "Hello\n";
	}// Inicio del tiempo.
	clock_t startTime = clock();

	/*
		const auto max = 10000;

		omp_set_nested(1); // Permite ciclos anidados.

		auto start = chrono::system_clock::now();
	*/


	// Testing levenshtein
	/*double time1 = 0;
	double time2 = 0;
	//double time3 = 0;
	*/


	//const auto text1 = input::read_file("data.txt"); // 1300 Lines
	//const auto text2 = input::read_file("data1.txt"); //1200 Lines
	//const auto text3 = input::read_file("data3.txt"); //21 Lines
	//const auto text4 = input::read_file("data4.txt"); //65 Lines
	/*

	const auto cant = Tools::levenshtein_parallel(text2, text1, time1);

	//const auto d = Tools::editDistanceOMP(text3, text4, time3);

	const auto can2 = Tools::levenshtein_secuencial(text2, text1, time2);

	cout << "Parallel Algorithm Classic:" << "\n" << "    ";
	cout << "Distance -> " << cant << " -> Time = ";
	cout << std::setprecision(7) << time1 << "\n\n";


	cout << "Secuencial Algorithm:" << "\n" << "    ";
	cout << "Distance -> " << can2 << " -> Time = ";
	cout << std::setprecision(7) << time2 << "\n\n";

	*/

	/// Show Custom Algorithm	
	/*
	//cout << "Parallel Algorithm Custom:" << "\n" << "    ";
	//cout << "Distance -> " << d << " -> Time = ";
	//cout << std::setprecision(4) << time3 << "\n\n";
	*/

	//Testing Crear Matriz

	//Crear instancia de la clase
	auto instance = Tools();

	//Juego de datos .....
	//string trazas[6286];



	auto trazas = input::read_file2("C:\\Users\\Leo\\Desktop\\TraceAligment CSV Data\\cleandataB2", 10618);

	cout << "Fichero cargado en: ";
	cout << double(clock() - startTime) / (double)CLOCKS_PER_SEC << " seconds." << endl;

	instance.check_parallel();
	/*
	trazas[0] = "c";
	trazas[1] = "cc";
	trazas[2] = "ccc";
	trazas[3] = "cccc";
	trazas[4] = "ccccc";
	trazas[5] = "cccccc";
	trazas[6] = "ccccccc";
	trazas[7] = "cccccccc";
	*/
	// Actualizar el juego de datos en la clase.
	//instance.inicio(trazas, 10618);  // NOLINT(readability-static-accessed-through-instance)

	//cout << "Trazas ordenadas por Distance of Levenshtein en: ";
	//cout << double(clock() - startTime) / (double)CLOCKS_PER_SEC << " seconds." << endl;

	//const auto start = chrono::system_clock::now();

	//Calcular la matriz de distancias.
	//auto trazas_ordenadas = instance.trazas_ordenadas;

	//const auto end = std::chrono::system_clock::now();


	//const auto time = (end - start).count() / 1000;

	//cout << std::setprecision(7) << time << "\n\n";

	/*for (int i = 0; i < 163; i++)
		cout << trazas_ordenadas[i] << "\n";
		*/











}


