#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
using namespace std;



void MainWindow::updateMorphImage()                                             //the MorphologyEx Algorithm, also updates image
{
/*
 * instantiates the img_threshold
 * creates img_morphex if checked
 * updates the image
*/

    if (ui->cb_sobel->isChecked()){
        threshold(img_sobel, img_threshold, 0, 255,CV_THRESH_BINARY); }
    else{
        threshold(src_gray, img_threshold, 0, 255,CV_THRESH_BINARY);}
    Mat element = getStructuringElement( kernalType, Size( morph_width, morph_height )/*, Point( morph_size, morph_size )*/ );
    if (ui->cb_morphex->isChecked()){
        morphologyEx( img_threshold, img_morphex,  morph_operator + 2, element );   //output is  (Src is always RGB)
        imUpdate(  ui->nl , &img_morphex );                                         //updates the label with image
    }
    else{img_morphex=img_threshold.clone();}
//    findMyContours();
}

//================================= G U I ======================================



//____________________MorphologyEx SLIDERS______________________________________MorphologyEx SLIDERS

void MainWindow::on_morphOperatorSlider_valueChanged(int value)                 //sliders update the variables and inititate a rerun of process
{
    ui->MorphologyOperatorDisplay->setValue(value);
    morph_operator=value;
//    int operation = morph_operator + 2;
    updateMorphImage();
    updateMorphologyTypeLabel();
}

void MainWindow::on_kernal_Type_slider_valueChanged(int value)
{
    ui->kernalTypeSpin->setValue(value);
    kernalType = value;
    updateMorphImage();
    updateKernalTypeLabel();



}

void MainWindow::on_kernal_width_slider_valueChanged(int value)
{
//    value=value;
    ui->kernalWidthSpin->setValue(value);
    morph_width=value;
    updateMorphImage();
    updateSizeLabel();

}
void MainWindow::on_kernal_height_slider_valueChanged(int value)
{
    ui->kernalHeightSpin->setValue(value);
    morph_height=value;
    updateMorphImage();
    updateSizeLabel();
}

//____________________MorphologyEx SPINBOXes____________________________________MorphologyEx SPINBOXes

void MainWindow::on_MorphologyOperatorDisplay_valueChanged(int arg1)
{
    ui->morphOperatorSlider->setValue(arg1);
    updateMorphologyTypeLabel();
}

void MainWindow::on_kernalTypeSpin_valueChanged(int arg1)
{
    ui->kernal_Type_slider->setValue(arg1);                                     //should update kernal Type as well

    updateKernalTypeLabel();

}

void MainWindow::on_kernalWidthSpin_valueChanged(int arg1)
{
    ui->kernal_width_slider->setValue(arg1);
    updateSizeLabel();

}
void MainWindow::on_kernalHeightSpin_valueChanged(int arg1)
{
    ui->kernal_height_slider->setValue(arg1);
    updateSizeLabel();
}





//_____________________________________UPdating_________________________________Updating Functions

void MainWindow::updateSizeLabel(){
    QString q = QString::number(morph_width) + " x " + QString::number(morph_height);
    ui->KernalSizeLabel->setText(q);
}

void MainWindow::updateKernalTypeLabel()
{
    switch (kernalType){
    case (0):ui->kernalTypeLabel->setText("Rectangular");break;
    case (1):ui->kernalTypeLabel->setText("Cross");break;
    case (2):ui->kernalTypeLabel->setText("Ellipse");break;
    default : break;
    }
}

void MainWindow::updateMorphologyTypeLabel()
{
    switch (morph_operator){
    case (0):ui->morphologyOperatorLabel->setText("Opening");break;
    case (1):ui->morphologyOperatorLabel->setText("Closing");break;
    case (2):ui->morphologyOperatorLabel->setText("Morphological Gradient");break;
    case (3):ui->morphologyOperatorLabel->setText("Top Hat");break;
    case (4):ui->morphologyOperatorLabel->setText("Black Hat");break;//

//Opening: MORPH_OPEN : 2
//Closing: MORPH_CLOSE: 3
//Gradient: MORPH_GRADIENT: 4
//Top Hat: MORPH_TOPHAT: 5
//Black Hat: MORPH_BLACKHAT: 6

    default : break;
    }
}




