/**
  This is Network Topologies Generator, a tool with the objective to assist network architects in creating new survivor topologies.
    Copyright (C) 2015  Silvana Trindade

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include <chrono>
#include <ctime>
#include <sstream> // stringstream
#include <iomanip> // put_time
#include <string>

#include <QtCore>
#include "xlsxdocument.h"
#include "xlsxconditionalformatting.h"

#include "Plane.hpp"

using namespace QXlsx;

class FileWriter
{
public:
	FileWriter();
	~FileWriter();

	string getDateTime();
    void setNumberOfTopology(int);

	void openFile();
	string returnCurrentTimeAndDate();
    void createXls();
	void writeCoordinatesTopologies(Graph, Plane);
    void writeTopologies(Graph,int,int);
	void writeMeasures(Graph &,bool,bool,bool,bool);
	void closeFileTopologies();
	void closeFileMeasures();

private:
	string dateTime; 	//armazena data e hora
	ofstream output;	//arquivo onde será armazenado as topologias geradas
	Document xlsx;		//arquivo xls
	Format hFmt;		//formato 
	int row;			//guarda a linha da ultima topologia gravada
	int simulation;		//guarda a simulação anterior
	int lastSimulation; //número da simulação anterior
	int topology;		//armazena número da topologia no momento 
};
