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
#include "mainwindow.h"
#include "svgviewer.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QString>
#include <QDesktopServices>//para abrir help
#include "Plane.hpp"
#include "Measure.hpp"
#include "Suurballe.hpp"
#include "FileWriter.hpp"
#include "DrawGraph.hpp"
#include "graphicsview.h"
#include <QFileDialog>

using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->actionOpen_help, SIGNAL(triggered()), this, SLOT(on_help_clicked()));
    connect(ui->actionAbout, SIGNAL(triggered()), this, SLOT(openNewWindow()));

    GraphicsView *q = new GraphicsView();

    this->graphEditor = new GraphEditor(q);

    q->setGeometry(0, 0, ui->gridLayout->geometry().width(), ui->gridLayout->geometry().height());
    ui->gridLayout->addWidget(q);

    addingNode = false;

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::openNewWindow()
{
    this->aboutWindow = new About(this);
    this->aboutWindow->show();
}

void MainWindow::on_pushButton_clicked()
{
    ui->error->setText(QString::fromUtf8(""));
     ui->error->setText(QString::fromUtf8(""));
     ui->pushButton->setEnabled(false);
     ui->pushButton->setText("Simulating...");
     ui->pushButton->repaint();
     int simulation = 1;
     int limitSimulation = ui->numberOfSimulations->value();
     FileWriter file;
     
     SVGViewer *svg = new SVGViewer();
     svg->setWindowFlags(Qt::Dialog | Qt::Desktop);
     bool survivor = false, ok = false;
     
     while( simulation <= limitSimulation )
     {

        Graph graph; // cria objeto grafo

         graph.setNumberOfNodes( ui->nodes->value() ); //número de nós

         graph.setMinimumDegree(ui->minimumDegree->value());//grau mínimo

         graph.setMaximumDegree(ui->maximumDegree->value());//grau máximo

         graph.setMinimumDistanceOfNode(ui->distance->value());//distância mínima entre dois nós

         graph.setMaximumAverageDegree(ui->max_average_degree->value());


         /**
          * Configurações do plano
          */
         Plane plane;

         plane.setArea(ui->area->value()); //valor da raiz da area

         /**
          * Verifica tipo de configuração para as regiões
          * Caso o usuário inserir somente o número de regiões
          * Então será calculado a largura e o comprimento.
          */
         if (ui->fexibleRegions->isChecked())
         {
            plane.setLength(ui->length->value());//comprimento

            plane.setBreadth(ui->breadth->value());//largura

         }
         else
         {

            plane.setNumberRegions(ui->numberOfRegions->value());

            plane.setRegionsMeasures();
         }

         plane.setWaxmanParameters(ui->alpha->value(),ui->beta->value());

         if(ui->nodesDistribution->currentText() == "Uniform")
         {
             plane.setDistributionType(0);
         }
         else
         {
             plane.setDistributionType(1);
         }

         try
         {
            plane.limitArea(graph.getNumberOfNodes());
         }
         catch(const char *error)
         {
            ui->error->setText(QString::fromUtf8("2N ≤ R ≤ N²\n"));
            ui->pushButton->setEnabled(true);
            ui->pushButton->setText("Begin simulation");
            return;
         }

         try
         {
            graph.limitDegree();
         }
         catch(const char *error)
         {
            ui->error->setText(QString::fromUtf8("2 ≤ Maximum Degree ≤ N-1\n"));
            ui->pushButton->setEnabled(true);
            ui->pushButton->setText("Begin simulation");
            return;
         }

         plane.setNumberOfSimulations(ui->numberOfSimulations->value());

        graph.memsetGraph();
        plane.initialize(graph,simulation);
        
         /**
          * Verifica se o número de ligações foi atingido
          * Se sim verifica se a topologia gerada é sobrevivente
          * Do contrário realiza sorteio randômico de nós até atingir
          * o limit e máximo, verificando-se a sobrevivência
          */

         int notMax = std::numeric_limits<int>::max();
         int topology = 1;

         while( ( graph.getNumberOfEdges() < graph.getMinimumNumberOfEdges() && notMax >= 2) && ( (graph.getAverageDegree() < graph.getMaximumAverageDegree() ) && notMax >= 2) )
         {
            notMax = plane.randomLink(graph);

            if (graph.getAverageDegree() >= graph.getMaximumAverageDegree())
            {
                break;
            }
         }

          for(int w = 0; w < graph.getNumberOfNodes();w++) graph.printAdjacents(w);

        Suurballe s;

        survivor = s.execute(graph);

        if(survivor == true)
        {
            ok = true;
            if (simulation == 1)
            {
                file.openFile();
                file.writeCoordinatesTopologies(graph,plane);
            }

            file.writeTopologies(graph,simulation,topology);

            if( (ui->bc->isChecked() || ui->cc->isChecked() || ui->dc->isChecked() || ui->ec->isChecked() ) && survivor)
            {
                if (simulation)
                {
                    file.createXls();
                }
                Measure measures;
                
                measures.initialize( graph,graph.getNumberOfNodes(),ui->bc->isChecked(),ui->cc->isChecked(),ui->dc->isChecked(),ui->ec->isChecked() ); //obtêm as medidas de centralidade para cada nó da rede

                file.writeMeasures(graph,ui->bc->isChecked(),ui->ec->isChecked(),ui->dc->isChecked(),ui->cc->isChecked());
            }

            /**
             * Adicionando imagem do grafo em formato svg e abrindo caixa de diálago
             */
            DrawGraph draw(graph,plane,file.getDateTime(),topology,simulation);//desenha grafo

            svg->openSVG(draw.getFile(),topology);

            topology++;
        }



        int nEdges = graph.getNumberOfEdges();


        while( (graph.getNumberOfEdges() < graph.getMaximumNumberOfEdges() && notMax >= 2) && ( (graph.getAverageDegree() < graph.getMaximumAverageDegree() ) && notMax >= 2) )
        {
            notMax = plane.randomLink(graph);
          
            if (graph.getNumberOfEdges() > nEdges)
            {
                nEdges = graph.getNumberOfEdges();

                /**
                 * Executa algoritmo se Suurballe somente 
                 * se a rede não for sobrevivente
                 */
                if (survivor == false)
                {
                    Suurballe suurballe;

                    survivor = suurballe.execute(graph);
                }

                if(survivor)
                {
                    if (simulation == 1 && topology == 1)
                    {
                        file.openFile();
                        file.writeCoordinatesTopologies(graph,plane);
                    }

                    file.writeTopologies(graph,simulation,topology);


                    if( (ui->bc->isChecked() || ui->cc->isChecked() || ui->dc->isChecked() || ui->ec->isChecked() ) && survivor )
                    {
                        if (simulation)
                        {
                            file.createXls();
                        }

                         Measure measures;

                         measures.initialize(graph,graph.getNumberOfNodes(),ui->bc->isChecked(),ui->cc->isChecked(),ui->dc->isChecked(),ui->ec->isChecked() ); //obtêm as medidas de centralidade para cada nó da rede

                         file.writeMeasures(graph,ui->bc->isChecked(),ui->ec->isChecked(),ui->dc->isChecked(),ui->cc->isChecked());
                    }

                    /**
                     * Adicionando imagem do grafo em formato svg e abrindo caixa de diálago
                     */
                    DrawGraph draw(graph,plane,file.getDateTime(),topology,simulation);//desenha grafo

                    svg->openSVG(draw.getFile(),topology);

                    topology++;
                }

                if (graph.getAverageDegree() >= graph.getMaximumAverageDegree())
                {
                    break;
                }

                survivor = false;
            }
        }

        simulation++;
     }

     svg->show();

     if (ok == true)
     {
         QString message = "Simulation complete. File located at \"";
         message.append(QDir::homePath());
         message.append("/simulations\"");

         ui->error->setText(message);

         file.closeFileTopologies();
         if (ui->bc->isChecked() || ui->cc->isChecked() || ui->dc->isChecked() || ui->ec->isChecked())
         {
            file.closeFileMeasures();
         }
     }
     
     ui->pushButton->setEnabled(true);
     ui->pushButton->setText("Begin simulation");
}

void MainWindow::on_fixedRegions_clicked()
{
    ui->numberOfRegions->setEnabled(true);

    ui->breadth->setEnabled(false);
    ui->length->setEnabled(false);
}

void MainWindow::on_fexibleRegions_clicked()
{
    ui->numberOfRegions->setEnabled(false);

    ui->breadth->setEnabled(true);
    ui->length->setEnabled(true);
}

void MainWindow::on_measures_clicked()
{
    if(ui->bc->isChecked())
    {
        ui->bc->setEnabled(true);
    }
    else
    {
        ui->bc->setEnabled(false);
    }

    if(ui->ec->isChecked())
    {
        ui->ec->setEnabled(true);
    }
    else
    {
        ui->ec->setEnabled(false);
    }

    if(ui->cc->isChecked())
    {
        ui->cc->setEnabled(true);
    }
    else
    {
        ui->cc->setEnabled(false);
    }

    if(ui->dc->isChecked())
    {

        ui->dc->setEnabled(true);
    }
    else
    {
        ui->dc->setEnabled(false);
    }
}

void MainWindow::on_help_clicked()
{
    QDesktopServices::openUrl(QUrl(QString::fromStdString(this->appPath+"/help/index.html"), QUrl::TolerantMode));
}

void MainWindow::on_pushButton_2_clicked()
{
    QString filename = QFileDialog::getSaveFileName();
    graphEditor->saveAsSVG(filename);
}

void MainWindow::addNode(int x, int y)
{
    graphEditor->addNode(x, y);
}

void MainWindow::on_addnode_clicked()
{
    graphEditor->addingNode = true;
}

void MainWindow::on_new_topology_clicked()
{
    graphEditor->clearGraph();
}

void MainWindow::on_m_simulation_clicked()
{
     ui->error->setText(QString::fromUtf8(""));
     ui->error->setText(QString::fromUtf8(""));
     ui->m_simulation->setEnabled(false);
     ui->m_simulation->setText("Simulating...");
     ui->m_simulation->repaint();
     FileWriter file;

     Graph graph; // cria objeto grafo

    graphEditor->constructGraph(graph);

    for(int u = 0; u < graph.getNumberOfNodes();u++) graph.printAdjacents(u);

     /**
      * Verifica se o número de ligações foi atingido
      * Se sim verifica se a topologia gerada é sobrevivente
      * Do contrário realiza sorteio randômico de nós até atingir
      * o limit e máximo, verificando-se a sobrevivência
      */

    Suurballe s;

    bool survivor = s.execute(graph);

    if(survivor == true)
    {

        if( (ui->m_bc->isChecked() || ui->m_cc->isChecked() || ui->m_dc->isChecked() || ui->m_ec->isChecked() ) && survivor)
        {
            file.createXls();

            Measure measures;
            
            measures.initialize( graph,graph.getNumberOfNodes(),ui->m_bc->isChecked(),ui->cc->isChecked(),ui->m_dc->isChecked(),ui->m_ec->isChecked() ); //obtêm as medidas de centralidade para cada nó da rede

            file.writeMeasures(graph,ui->m_bc->isChecked(),ui->m_ec->isChecked(),ui->m_dc->isChecked(),ui->m_cc->isChecked());
        }


         if (ui->m_bc->isChecked() || ui->m_cc->isChecked() || ui->m_dc->isChecked() || ui->m_ec->isChecked())
         {
             QString message = "File located at \"";
             message.append(QDir::homePath());
             message.append("/simulations\"");

             ui->m_error->setText(message);
             file.closeFileMeasures();
         }

    }

    ui->m_simulation->setEnabled(true);
    ui->m_simulation->setText("Begin simulation");
}
