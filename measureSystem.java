
package POO_projecto;

import com.googlecode.javacpp.Loader;
import com.googlecode.javacv.*;
import com.googlecode.javacv.cpp.*;
import com.googlecode.javacv.cpp.opencv_core.CvPoint;
import com.googlecode.javacv.cpp.opencv_core.CvScalar;
import com.googlecode.javacv.cpp.opencv_core.CvSeq;
import com.googlecode.javacv.cpp.opencv_core.IplImage;
import com.googlecode.javacv.cpp.opencv_highgui.CvCapture;
import com.googlecode.javacv.cpp.opencv_imgproc.CvMoments;
import static com.googlecode.javacv.cpp.opencv_core.*;
import static com.googlecode.javacv.cpp.opencv_imgproc.*;
import static com.googlecode.javacv.cpp.opencv_calib3d.*;
import static com.googlecode.javacv.cpp.opencv_objdetect.*;
import static com.googlecode.javacv.cpp.opencv_highgui.*;

//import org.javaopencvbook.utils.ImageProcessor;
import org.opencv.core.Core; 
import org.opencv.core.Mat; 
import org.opencv.videoio.Videoio;
import org.opencv.videoio.VideoCapture;

/**
 * @author simaoppcastro
 */
public class measureSystem 
{
    //public static void main(String[] args) 
    {
        IplImage img1;
        IplImage imghsv;
        IplImage imgbin;
        
        //*testes
        IplImage gray;          //não utilizado, até ao momento
        IplImage frame;         //não utilizado, até ao momento
        IplImage canny; //= null;   //iniciado, mas não utilizado até ao momento
        
        //--------------------para apresentar na imagem as medições (ainda nao esta a funcionar)-------------------------------------------
        CvScalar red = CV_RGB(255,0,0);
        CvFont font = new CvFont();
        cvInitFont(font, CV_FONT_HERSHEY_SIMPLEX, 0.75, 0.75, 0, 2,0);
        //------------------------------------------------------------
        //---------------cores da filtragem--------------------------------
        /*
        CvScalar minc = cvScalar(95, 150, 75, 0);
        CvScalar maxc = cvScalar(145, 255, 255, 0);
         */
        //azul
        CvScalar Bminc = cvScalar(95, 150, 75, 0);
        CvScalar Bmaxc = cvScalar(145, 255, 255, 0);
        //vermelho
        CvScalar Rminc = cvScalar(150, 150, 75, 0);
        CvScalar Rmaxc = cvScalar(190, 255, 255, 0);
        //preto
        CvScalar Pminc = cvScalar(0, 0, 0, 0);
        CvScalar Pmaxc = cvScalar(30, 30, 30, 0);
        //------------------------------cores------------------------------
        
        //**teste CvSize
        CvSize size = new CvSize();
        //------------------------------------------    
        CvSeq contorno1 = new CvSeq();      //contorno 1
        CvSeq contorno2 = new CvSeq();
        CvMemStorage storage = CvMemStorage.create();
        CvMoments moments = new CvMoments(Loader.sizeof(CvMoments.class));

        double areaMax;
        double areaC = 0;
        double m10;
        double m01;
        double m_area;

        int posX = 0;
        int posY = 0;

        CvCapture capturaImagem = cvCreateCameraCapture(0);                          //(CV_CAP_ANY);     //qual camera escolher
        imghsv = cvCreateImage(cvSize(640, 480), 8, 3);                         //iniciar os objectos, que neste caso são das imagens correspondentes a cada frame
        imgbin = cvCreateImage(cvSize(640, 480), 8, 1);
        
        //teste
        canny = cvCreateImage(cvSize(640, 480), 8, 1);
        img1 = cvCreateImage(cvSize(640, 480), 8, 1);
        
        


        int i = 1;
        while (i == 1)                                                          //LOOP PRINCIPAL
        {

            img1 = cvQueryFrame(capturaImagem);                                 //img1 começa por ser a primeira instancia e ligação com a captura de video

            if (img1 == null)                                                   //pequeno tratamento de dados/erros
            {   
                System.err.println("Erro! Sem Imagem/Problemas com a ligação");
                break;
            }

            cvCvtColor(img1, imghsv, CV_BGR2HSV);               //tratamento de imagem -> procura pelas diferentes cores
            cvInRangeS(imghsv, Rminc, Rmaxc, imgbin);           //(imagem fonte, cor minima, cor maxima, imagem onde aparece só a parte colorida retida)  -> restringe para a imagem/objecto pretendido, a cor, dentro do limite desejado, para colocar no novo objecto 

            contorno1 = new CvSeq();                            //iniciar o primeiro contorno (CvSeq())
            areaMax = 1000;                                     //iniciar a variavel area 

            cvFindContours(imgbin, storage, contorno1, Loader.sizeof(CvContour.class),CV_RETR_LIST, CV_LINK_RUNS, cvPoint(0, 0));           //função metodo para procurar pelos contornos
            
            contorno2 = contorno1;                              //atribuição do 1º contorno ao 2º, para que posteriormente seja utilizado
            
            //contorno 1
            while (contorno1 != null && !contorno1.isNull())                //tratamento do 1º contorno
            {
                areaC = cvContourArea(contorno1, CV_WHOLE_SEQ, 1);          //atribuir a variavel areaC, o que a função/metodo atribui de area calculada

                if (areaC > areaMax) 
                {
                    areaMax = areaC;
                }

                contorno1 = contorno1.h_next();

            }
            
            //contorno 2
            while (contorno2 != null && !contorno2.isNull())            //tratamento do 2º contorno
            {
                areaC = cvContourArea(contorno2, CV_WHOLE_SEQ, 1);      //mesma atribuição refeita em cima -> ver area atribuida a partir do contorno 2

                if (areaC < areaMax) 
                {
                    //agora sim, é desenhado o contorno na imgbin (imgbin -> frame onde só aparece o objecto que anteriormente foi identificado pela cor desejada)
                    cvDrawContours(imgbin, contorno2, CV_RGB(0, 0, 0), CV_RGB(0, 0, 0),0, CV_FILLED, 8, cvPoint(0, 0));         //CV_RGB(0, 0, 0), CV_RGB(0, 0, 0)
                    //função que desenha a partir do contorno definido anteriormente que neste caso é o contorno 2 (outras defenições correspondem ao tipo de contorno
                }

                contorno2 = contorno2.h_next();
            }

            cvMoments(imgbin, moments, 1);

            m10 = cvGetSpatialMoment(moments, 1, 0);
            m01 = cvGetSpatialMoment(moments, 0, 1);
            m_area = cvGetCentralMoment(moments, 0, 0);

            posX = (int) (m10 / m_area);
            posY = (int) (m01 / m_area);

            if (posX > 0 && posY > 0) 
            {
                System.out.println("x = " + posX + ", y= " + posY);
            }

            cvCircle(img1, cvPoint(posX, posY), 5, cvScalar(0, 0, 255, 0), 9, 0, 0);                //circulo do centro da imagem/objecto em vermelho
            //cvCircle(imgbin, cvPoint(posX, posY), 5, cvScalar(0, 0, 255, 0), 9, 0, 0);                //circulo do centro da imagem/objecto em vermelho
            
            double largura = posX / 2 + 100; 
            double comprimento = posY + 30;

            cvShowImage("Imagem/Video Original", img1);         ///(texto que aparece na frame, qual imagem)
            cvShowImage("Imagem/Video Tratado", imgbin);
            //cvShowImage("CannyTeste", canny);
            
             
            //**testes
            
            
            cvDrawContours(canny, contorno1, CV_RGB(255, 255, 255), CV_RGB(255, 255, 255),0, CV_FILLED, 8, cvPoint(0, 0));
            
            System.out.println("teste:" + size.height(posX).width(posY));
            System.out.println("m10: " + m10);
            System.out.println("m01: " + m01);
            System.out.println("m_area: " + m_area);
            
            System.out.println("*****");
            System.out.println("Largura: " + largura + " mm");
            System.out.println("Comprimento: " + comprimento + " mm");
            System.out.println("*****");

            //**teste
            
            //tecla para sair do pograma 
            char c = (char) cvWaitKey(15);
            if (c == 'q' || c == 27) 
            {
                break;
            }
            
            

        }

        cvReleaseImage(imghsv);             //original
        cvReleaseImage(imgbin);             //contornos
        //cvReleaseImage(canny);
        //cvReleaseImage(img1);
        cvReleaseMemStorage(storage);
        cvReleaseCapture(capturaImagem);

    }

}

