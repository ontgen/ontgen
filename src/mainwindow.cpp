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
#include "pngviewer.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QString>
#include <QDesktopServices>//para abrir help
#include "Plane.hpp"
#include "Measure.hpp"
#include "Suurballe.hpp"
#include "FileWriter.hpp"
#include <QFileDialog>

using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->actionOpen_help, SIGNAL(triggered()), this, SLOT(on_help_clicked()));
    connect(ui->actionAbout, SIGNAL(triggered()), this, SLOT(openNewWindow()));

    q = new GraphicsView();

    this->graphEditor = new GraphEditor(q);
    this->draw = new DrawGraph(q);

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

/**
 * Simulação no modo automático
 */
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
     PNGViewer *png = new PNGViewer();
     svg->setWindowFlags(Qt::Dialog | Qt::Desktop);
     bool survivor = false, ok = false;
     int total = 0; //recebe o número total de topologias para limitar abertura da janela de visualização
     
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
             //verificando limite para o número de regiões
             //onde não deve ser maior que a raiz do plano
             if(ui->area->value() < ui->numberOfRegions->value())
             {
                ui->error->setText(QString::fromUtf8("1 ≤ R ≤ sqrt(A)\n"));
                ui->pushButton->setEnabled(true);
                ui->pushButton->setText("Begin simulation");
                return;
             }

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
        plane.initialize(graph);

         /**
          * Verifica se o número de ligações foi atingido
          * Se sim verifica se a topologia gerada é sobrevivente
          * Do contrário realiza sorteio randômico de nós até atingir
          * o limit e máximo, verificando-se a sobrevivência
          */

         int notMax = std::numeric_limits<int>::max();
         int topology = 1;

         while( ( graph.getNumberOfEdges() < graph.getMinimumNumberOfEdges() ) && ( (graph.getAverageDegree() < graph.getMaximumAverageDegree() ) && ( notMax >= 2 ) ) )
         {
            notMax = plane.randomLink(graph);

            if (graph.getAverageDegree() >= graph.getMaximumAverageDegree())
            {
                break;
            }
         }

//        for(int w = 0; w < graph.getNumberOfNodes();w++) graph.printAdjacents(w);

        Suurballe s;
        survivor = false;
        survivor = s.execute(graph);
//        cout<<"SURVIVOR"<<survivor<<endl;

        if(survivor == true)
        {

            ok = true;

            if (simulation == 1)
            {
                file.openFile();
                file.writeCoordinatesTopologies(graph,plane);
            }

            file.writeTopologies(graph,simulation,topology);

            if( (ui->bc->isChecked() || ui->cc->isChecked() || ui->dc->isChecked() || ui->ec->isChecked() ) && survivor == true)
            {
                if (simulation == 1)
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
//            DrawGraph draw(graph,plane,file.getDateTime(),topology,simulation);//desenha grafo
//            svg->openSVG(draw.getFile(),topology);
            DrawGraph *d = new DrawGraph(q);

            d->setOGDFGraph(graph,plane,file.getDateTime(),topology,simulation);
            d->rerender();
            d->saveAsPNG(QString::fromStdString(d->getFile()));
            png->openPNG(d->getFile(),topology);

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
//                cout<<"SURVIVOR"<<survivor<<endl;
                if(survivor == true)
                {
                    ok = true;

                    if (simulation == 1 && topology == 1)
                    {
                        file.openFile();
                        file.writeCoordinatesTopologies(graph,plane);
                    }

                    file.writeTopologies(graph,simulation,topology);


                    if( (ui->bc->isChecked() || ui->cc->isChecked() || ui->dc->isChecked() || ui->ec->isChecked() ))
                    {
                        if (simulation == 1)
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
//                    DrawGraph draw(graph,plane,file.getDateTime(),topology,simulation);//desenha grafo

//                    svg->openSVG(draw.getFile(),topology);
                    DrawGraph *d = new DrawGraph(q);
                    /**
                     * Adicionando imagem do grafo em formato png e abrindo caixa de diálago
                     */

                    d->setOGDFGraph(graph,plane,file.getDateTime(),topology,simulation);
                    d->rerender();
                    d->saveAsPNG(QString::fromStdString(d->getFile()));
                    png->openPNG(d->getFile(),topology);

                    topology++;
                }
            }

            //atinge limites em relação ao grau
            if (graph.getAverageDegree() >= graph.getMaximumAverageDegree())
            {
                break;
            }
        }
        total = topology + total;

        simulation++;
     }

     if (ok == true)
     {
//       svg->show();//exibe janela de visualização
         if(total <= 30) png->show();//exibe janela de visualização
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


/**
 * Verificação das medidas de centralidade
 */
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

/**
 * Ajuda ao usuário, onde abre arquivo html no navegador
 */
void MainWindow::on_help_clicked()
{
    QDesktopServices::openUrl(QUrl(QString::fromStdString(this->appPath+"/help/index.html"), QUrl::TolerantMode));
}

/**
 * Salva imagem, no local que o usuário escolher
 */
void MainWindow::on_pushButton_2_clicked()
{
    QString filename = QFileDialog::getSaveFileName();
    graphEditor->saveAsPNG(filename);
}

/**
 * Adiciona um novo nó na topologia gerada automática
 */
void MainWindow::addNode(int x, int y)
{
    graphEditor->addNode(x, y);//adiciona um novo nó na topologia manual
}

void MainWindow::on_addnode_clicked()
{
    graphEditor->addingNode = true;
}

/**
 * Limpa, para a geração de uma nova topologia
 */
void MainWindow::on_new_topology_clicked()
{
    graphEditor->clearGraph();

     ui->m_error->setText("");
}

/**
 * Simula uma nova topologia
 */
void MainWindow::on_m_simulation_clicked()
{
    ui->m_error->setText("");
     ui->error->setText(QString::fromUtf8(""));
     ui->error->setText(QString::fromUtf8(""));
     ui->m_simulation->setEnabled(false);
     ui->m_simulation->setText("Simulating...");
     ui->m_simulation->repaint();
     FileWriter file;
     file.setNumberOfTopology(1);//indice da topologia

     Graph graph; // cria objeto grafo

     graphEditor->constructGraph(graph);

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

        if( (ui->m_bc->isChecked() || ui->m_cc->isChecked() || ui->m_dc->isChecked() || ui->m_ec->isChecked() ))
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
    else
    {
        ui->m_error->setText("Topology isn't survivor");
    }
    
    ui->m_simulation->setEnabled(true);
    ui->m_simulation->setText("Begin simulation");
}

/**
 * Insere imagem de plano de fundo
 */
void MainWindow::on_background_image_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this,QString("Open image file"),QDir::home().path());

    if(filename.isEmpty())
    {
        return;
    }

    graphEditor->loadBackgroundImage(filename);
}

/**
 * Abre um arquivo com uma topologia já existente
 * pelo modo manual
 */
void MainWindow::on_open_topology_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this,QString("Open topology file"),QDir::home().path(), tr("DOT Files (*.dot)"));

    if(filename.isEmpty())
    {
        return;
    }

    graphEditor->loadTopology(filename);
}

/**
 * Salva a topologia gerada no modo manual
 */
void MainWindow::on_save_topology_clicked()
{

    QString filename = QFileDialog::getSaveFileName(this, QString("Save topology"), QDir::home().path(), tr("DOT Files (*.dot)"));

    QFileInfo file(filename);
    if(file.suffix().isEmpty()) filename += ".dot";
    graphEditor->saveAsGML(filename);
}
