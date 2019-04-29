#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    Timer = new QTimer(this);
    PathImage="imagenB.bmp";
    img = cv::imread(PathImage, 1);
    cvtColor(img, img, CV_BGR2RGB);
    cout<<"--"<<endl;
    connect(Timer,SIGNAL(timeout()),this,SLOT(Display_Image()));
    Timer->start();
    tam_imagen = img.size();
    gray=(img.rows, img.cols, CV_8UC1);
    histograma();
}

void MainWindow::Display_Image(){
    QImage imdisplay((uchar*)img.data,img.cols,img.rows,img.step,QImage::Format_RGB888);
    ui->label->setPixmap(QPixmap::fromImage(imdisplay));

    QImage HistogramaDisplay((uchar*)Histograma.data,Histograma.cols,Histograma.rows,Histograma.step,QImage::Format_RGB888);
    //QImage HistogramaDisplay((uchar*)Histograma.data,Histograma.cols,Histograma.rows,Histograma.step,QImage::Format_RGB888);
    ui->Histograma->setPixmap(QPixmap::fromImage(HistogramaDisplay));

}

void MainWindow::on_horizontalSlider_valueChanged(int value)
{
    int resultado=0;
    if(value>temporal){
        resultado=value-temporal;
        operacionesBasicas(resultado,'+');
        //cout<<"Value = "<<value<<" Temporl = "<<temporal<<" Resultado = "<<resultado<<endl;
        temporal=value;
    }
    else if (value<temporal) {
        resultado=temporal-value;
        operacionesBasicas(resultado,'-');
        //cout<<"Value = "<<value<<" Temporl = "<<temporal<<" Resultado = "<<resultado<<endl;
        temporal=value;
    }
    histograma();

}

void MainWindow::histograma(){
    // Inicialzar el array
    vector<Mat> bgr_planes;
    split(img, bgr_planes);
    for (int i = 0; i < 1; ++i)
    {
      //  cout<<bgr_planes[i]<<endl;
    }
    int histSize = 256;
    float range[] = {0, 256};
    const float* histRange = {range};

    bool uniform = true, accumulate = false;
    Mat b_hist, g_hist, r_hist;

    // Calcular el histograma
    calcHist(&bgr_planes[0], 1, 0, Mat(), b_hist, 1, &histSize, &histRange, uniform, accumulate);
    calcHist(&bgr_planes[1], 1, 0, Mat(), g_hist, 1, &histSize, &histRange, uniform, accumulate);
    calcHist(&bgr_planes[2], 1, 0, Mat(), r_hist, 1, &histSize, &histRange, uniform, accumulate);

    int hist_w =tam_imagen.width;
    int hist_h =tam_imagen.height;

    Mat histImg(hist_h, hist_w, CV_8UC3, Scalar(255, 255, 255));

    normalize(b_hist, b_hist, 0, histImg.rows, NORM_MINMAX, -1, Mat());
    normalize(g_hist, g_hist, 0, histImg.rows, NORM_MINMAX, -1, Mat());
    normalize(r_hist, r_hist, 0, histImg.rows, NORM_MINMAX, -1, Mat());

    int step = cvRound((double)hist_w / histSize);

    for (int i = 1; i < histSize; i++)
    {
        line(histImg, Point(step * (i -1), hist_h - cvRound(b_hist.at<float>(i - 1))),
                    Point(step * i, hist_h - cvRound(b_hist.at<float>(i))),
                    Scalar(255, 0, 0), 2, 8, 0);
        line(histImg, Point(step * (i -1), hist_h - cvRound(g_hist.at<float>(i - 1))),
                    Point(step * i, hist_h - cvRound(g_hist.at<float>(i))),
                    Scalar(0, 255, 0), 2, 8, 0);
        line(histImg, Point(step * (i -1), hist_h - cvRound(r_hist.at<float>(i - 1))),
                    Point(step * i, hist_h - cvRound(r_hist.at<float>(i))),
                    Scalar(0, 0, 255), 2, 8, 0);
    }

    Histograma=histImg;
   }

void MainWindow:: operacionesBasicas(int val,  char ope){


    Vec3f intensity;

    for(int i=0;i<tam_imagen.height;i++){
        for(int j=0;j<tam_imagen.width;j++){
            intensity = img.at<cv::Vec3b>(i, j);
            if(ope =='+'){
                intensity.val[0]+=val;//r
                intensity.val[1]+=val;//g
                intensity.val[2]+=val;//b
            }
            if(ope =='-'){
                intensity.val[0]-=val;//r
                intensity.val[1]-=val;//g
                intensity.val[2]-=val;//b
            }
            if(ope =='*'){
                float sumador=(val/500.0)+1;
                intensity.val[0]*=sumador;//r
                intensity.val[1]*=sumador;//g
                intensity.val[2]*=sumador;//b

            }
            if(ope =='/'){
                float sumador=(val/500.0)+1;
                intensity.val[0]/=sumador;//r
                intensity.val[1]/=sumador;//g
                intensity.val[2]/=sumador;//b
            }
            img.at<cv::Vec3b>(i, j) = intensity;
        }
    }

}
void MainWindow::on_Contraste_valueChanged(int value)
{

    int resultado=0;
    if(value>temporal){
        resultado=value-temporal;
        operacionesBasicas(value,'*');
        temporal=value;
    }
    else if (value<temporal) {
        resultado=temporal-value;
        operacionesBasicas(value,'/');
        temporal=value;
    }
    histograma();
}

void MainWindow::on_Divicion_valueChanged(int value)
{
    operacionesBasicas(value,'/');
/*Abrir ruta y seleccionar tipo de image*/

void MainWindow::on_Ecualizar_clicked()
{
    histograma();
    Mat dst;
    cvtColor(img,dst,COLOR_BGR2YCrCb);

    vector<Mat> vec_chanales; //histogramas array
    split(dst,vec_chanales);
    equalizeHist(vec_chanales[0],vec_chanales[0]);
    merge(vec_chanales,dst);
    cvtColor(dst,dst,COLOR_YCrCb2BGR);
    img=dst;
    histograma();

}

void MainWindow::on_Luminacion_clicked()
{
    Vec3f intensity;
    float luminacion=0.75;
    //Y = 0.299 R + 0.587 G + 0.114 B

    for(int i=0;i<tam_imagen.height;i++){
        for(int j=0;j<tam_imagen.width;j++){
            intensity = img.at<cv::Vec3b>(i, j);
            //cout<<intensity<<" -- "<<endl;
            intensity.val[0]= 255*pow((intensity.val[0]/255),1.0/luminacion);//r
            intensity.val[1]= 255*pow((intensity.val[1]/255),1.0/luminacion);//g
            intensity.val[2]= 255*pow((intensity.val[2]/255),1.0/luminacion);//b
            //cout<<R<<" "<<G<<" "<<B<<endl;
            img.at<cv::Vec3b>(i, j) = intensity;
        }
    }
    histograma();

}

void MainWindow::on_Grises_clicked()
{
    Vec3f intensity;
    //Y = 0.299 R + 0.587 G + 0.114 B

    for(int i=0;i<tam_imagen.height;i++){
        for(int j=0;j<tam_imagen.width;j++){
            intensity = img.at<cv::Vec3b>(i, j);
            float R= intensity.val[0];//r
            float G= intensity.val[1];//g
            float B= intensity.val[2];//b
            float Y = 0.299*R + 0.587*G + 0.114*B;

            intensity.val[0]=Y;//r
            intensity.val[1]=Y;//g
            intensity.val[2]=Y;//b

            img.at<cv::Vec3b>(i, j) = intensity;
        }
    }
    histograma();
}
void MainWindow::on_AbrirImage_clicked()
{

    filename = QFileDialog::getOpenFileName(this,
                                                     "Open A file ",
                                                     QDir::homePath(),
                                                     "All files (*.*) ;; Document files (*.doc *.rtf);; PNG files (*.png)",0,QFileDialog::DontUseNativeDialog);
    string file=filename.toStdString();

    img = cv::imread(file, 1);
    cvtColor(img, img, CV_BGR2RGB);
    tam_imagen = img.size();
    histograma();

}

void MainWindow::on_Guardar_clicked()
{

    imageObject = new QImage();
    imageObject->load(filename);
    QString FS = QFileDialog::getSaveFileName(this, tr("Save Image File"),
                                                    QString(),
                                                    tr("Images (*.png)"),0,QFileDialog::DontUseNativeDialog);
    string ffile=FS.toStdString();
    cout<<ffile<<endl;
    imwrite(ffile+".png",img);

    }
}
