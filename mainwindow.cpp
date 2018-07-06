/****************************************************************************
**
** Copyright (C) 2012 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Digia nor the names of its contributors
**     may be used to endorse or promote products derived from this 
**     software without specific prior written permission.
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
** $QT_END_LICENSE$
**
****************************************************************************/

#include <QtGui>
#include "pieview.h"
#include "mainwindow.h"
#include "qcustomplot.h"
#include <algorithm>
MainWindow::MainWindow()
{
    //Setup file menu layout
    setupMenu();
 
    //Setup radio buttons layout
    setupButtons();

    //Setup table and plot layouts
    setupMainLayout();

    //Set Abstract Data Model for table and charts
    setupModel();

    //Action for menu bar to open file
    createMenuActions();

    //Actions for radio buttons
    createButtonActions();
}

//Setup File Menu
void MainWindow::setupMenu()
{
    fileMenu = new QMenu(tr("&File"), this);
    openAction = fileMenu->addAction(tr("&Open..."));
    openAction->setShortcuts(QKeySequence::Open);
    menuBar()->addMenu(fileMenu);
    statusBar();
    resize(700, 420);
}

//Create Menu Actions
void MainWindow::createMenuActions()
{
    connect(openAction, SIGNAL(triggered()), this, SLOT(openFile()));
}

// Setup bottons at top of window, dont assign any actions yet
void MainWindow::setupButtons()
{
    // Initialize splitter, group box, and radio buttons
    mainVsplitter = new QSplitter(this);
    groupBox = new QGroupBox();
    radio1 = new QRadioButton(tr("&Pie Chart"));
    radio2 = new QRadioButton(tr("&Bar Chart"));
    radio3 = new QRadioButton(tr("&Line Chart"));

    // Arrange radio buttons in horizontal layout
    radio1->setChecked(true);
    QHBoxLayout *hbox = new QHBoxLayout;
    hbox->addItem(new QSpacerItem(2, 0, QSizePolicy::Expanding) );
    hbox->addWidget(radio1);
    hbox->addItem(new QSpacerItem(120, 0, QSizePolicy::Expanding) );
    hbox->addWidget(radio2);
    hbox->addItem(new QSpacerItem(120, 0, QSizePolicy::Expanding) );
    hbox->addWidget(radio3);
    hbox->addItem(new QSpacerItem(2, 0, QSizePolicy::Expanding) );
    hbox->addStretch(1);
    
    groupBox-> setLayout(hbox);

    // Add radio button group box to main splitter
    mainVsplitter->setOrientation(Qt::Vertical);
    mainVsplitter-> addWidget(groupBox);
    
    //Makes vertical splitter the main widget item
    // Can access mainVsplitter from other functions
    setCentralWidget(mainVsplitter);
}

//Create Button Actions
void MainWindow::createButtonActions()
{
connect(radio1, SIGNAL(clicked()), this, SLOT(displayPieChart()));
connect(radio2, SIGNAL(clicked()), this, SLOT(displayBarChart()));
connect(radio3, SIGNAL(clicked()), this, SLOT(displayLineChart()));
}

//Display Bar Chart
void MainWindow::displayBarChart()
{
pieChart-> hide();
customLinePlot-> hide();
setupBarPlot();
customBarPlot-> show();
}

//Display Pie Chart
void MainWindow::displayPieChart()
{
customLinePlot-> hide();
customBarPlot-> hide();
pieChart -> show();
}

//Display Line Chart
void MainWindow::displayLineChart()
{
pieChart-> hide();
customBarPlot-> hide();
setupLinePlot();
customLinePlot-> show();
}

//Make Custom Bar Plot
void MainWindow::setupBarPlot()               
{
    // FYI How to access data stored in model directly!
    // model->index(0,0).data().toString();
    // Going to just use stored data though
    QVector<double> xvec(numpeople.size());
    for (int i=0; i<xvec.size(); ++i)
    {
      xvec[i] = i;
    };

    double max = *std::max_element(numpeople.constBegin(), numpeople.constEnd());
    for (int i=0; i<xvec.size(); ++i)
    {
        QCPBars *bars1 = new QCPBars(customBarPlot->xAxis, customBarPlot->yAxis);
        bars1->setWidth(0.8);
        bars1->addData(xvec[i], numpeople[i]);
        bars1->setPen(QColor("black"));
        bars1->setBrush(QColor(color[i]));
        bars1->setName(country[i]);
        customBarPlot->xAxis->setRange(-.5, xvec.size()-.5);
        customBarPlot->yAxis->setRange(-.1, max+1);
        customBarPlot->xAxis->setVisible(false);
    };
    customBarPlot->legend->setVisible(false);
    customBarPlot-> replot();
    //numpeople.clear();
}

//Make Custom Line Plot
void MainWindow::setupLinePlot()
{
    
    
    //numpeople.clear();
    QVector<double> xvec(numpeople);
    for (int i=0; i<xvec.size(); ++i)
    {
      xvec[i] = i;
    };
    double max = *std::max_element(numpeople.constBegin(), numpeople.constEnd());
    // Plot black line
    customLinePlot->addGraph();
    customLinePlot->graph()->setData(xvec, numpeople);
    customLinePlot->graph()->setPen(QColor("black"));
    //Plot colored points
    for (int i=0; i<xvec.size(); ++i)
    {
        customLinePlot->addGraph();
        customLinePlot->graph()->addData(xvec[i], numpeople[i]);
        customLinePlot->graph()->setPen(QColor("black"));
        customLinePlot->graph()->setName(country[i]);
        customLinePlot->xAxis->setRange(-.5, xvec.size()-.5);
        customLinePlot->yAxis->setRange(-.1, max+1);
        customLinePlot->xAxis->setVisible(false);
        QCPScatterStyle myScatter;
        myScatter.setShape(QCPScatterStyle::ssCircle);
        myScatter.setPen(QPen(Qt::black));
        myScatter.setBrush(QColor(color[i]));
        myScatter.setSize(10);
        customLinePlot->graph()->setScatterStyle(myScatter);
    };
    //numpeople.clear();
}
void MainWindow::setupMainLayout()
{
    //Sets up bottom portion with table and plot
    splitter = new QSplitter;
    table = new QTableView;

    //Widgets for Bar, Line, and Pie Charts    
    barWidge = new QWidget;
    lineWidge = new QWidget;
    pieChart = new PieView;

    customBarPlot = new QCustomPlot(barWidge);
    customLinePlot = new QCustomPlot(lineWidge);
    customLinePlot->addGraph();

    //Add widgets to layout splitter
    splitter->addWidget(table);
    splitter->addWidget(pieChart);
    splitter->addWidget(customBarPlot);
    splitter->addWidget(customLinePlot);

    //Set strech scaling of splitter
    splitter->setStretchFactor(0, 0);
    splitter->setStretchFactor(1, 1);
    splitter->setStretchFactor(2, 1);
    splitter->setStretchFactor(3, 1);

    //setCentralWidget(splitter);
    //Add this bottom splitter to one made for buttons
    this-> mainVsplitter->addWidget(splitter);

    //For default display
    customLinePlot-> hide();
    customBarPlot-> hide();
}

// Setup model for table/pie chart
void MainWindow::setupModel()
{   //Generic model for storing custom data (row, column, parent)
    model = new QStandardItemModel(8, 2, this);
    model->setHeaderData(0, Qt::Horizontal, tr("Country"));
    model->setHeaderData(1, Qt::Horizontal, tr("Number of People"));
    table->setModel(model);
    pieChart->setModel(model);
    selectionModel = new QItemSelectionModel(model);
    table->setSelectionModel(selectionModel);
    pieChart->setSelectionModel(selectionModel);    
    QHeaderView *headerView = table->horizontalHeader();
    headerView->setStretchLastSection(true);

}

//Open File from main menu
void MainWindow::openFile(const QString &path)
{
    QString fileName;
    if (path.isNull())
        fileName = QFileDialog::getOpenFileName(this, tr("Choose a data file"),
                                                "", "*.cht");
    else
        fileName = path;

    if (!fileName.isEmpty()) {
        QFile file(fileName);

        if (file.open(QFile::ReadOnly | QFile::Text)) {
            QTextStream stream(&file);
            QString line;
            numpeople.clear();
            country.clear();
            color.clear();
            //delete customBarPlot;
            //delete customLinePlot;
            model->removeRows(0, model->rowCount(QModelIndex()), QModelIndex());

            int row = 0;
            do {
                line = stream.readLine();
                if (!line.isEmpty()) {
                    //Model Data for piechart, table using Model/View 
                    model->insertRows(row, 1, QModelIndex());
                    QStringList pieces = line.split(",", QString::SkipEmptyParts);
                    model->setData(model->index(row, 0, QModelIndex()),
                                   pieces.value(0));
                    model->setData(model->index(row, 1, QModelIndex()),
                                   pieces.value(1));
                    model->setData(model->index(row, 0, QModelIndex()),
                                   QColor(pieces.value(2)), Qt::DecorationRole);
                    //Store Data for custom plots
                    country.push_back(pieces.value(0));
                    numpeople.push_back(pieces.value(1).toFloat());
                    color.push_back(pieces.value(2));

                    row++;
                }
            } while (!line.isEmpty());

            file.close();
            statusBar()->showMessage(tr("Loaded %1").arg(fileName), 2000);
        }
    }
}

