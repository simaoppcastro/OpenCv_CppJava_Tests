//------------------------------------------------------------------------------
#include <iostream>
#include <opencv/cv.h>
#include <opencv/highgui.h> 
#include <stdio.h>
#include <vector>
#include <algorithm>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace std;

//--------------------------------------------------------------------------------
/*
  Autor: Simão Pedro Pereira Castro
  Email: simaoppcastro@gmail.com
  Projecto: MachineVision
  Versão: 2.0
  Data: 26/01/2019
*/
/*------------------------------------------------------------------------*//**
	Medição de objetos em tempo real.

	O algoritmo é baseado na detecção do contorno do objeto que queremos
	medir e calcular o retângulo mínimo que o envolve.

	Pré-requisitos:
	* o objeto deve ser colocado em um fundo uniforme
	* o objeto deve ter contraste suficiente para ser detectado pela webcam
	* a câmera deve ser posicionada em uma distância fixa do objeto
	* apenas um único objeto pode ser medido; se você colocar dois ou mais objetos
	você terá o tamanho da área mínima contendo todos eles.

	O programa é em C ++, mas usa a interface nesta versão usa OpenCV C
	Para compilar e executar o programa no Linux, ou no Windows com o GCC apenas
	digite 'make run'. Para compilar e executar no Visual Studio, siga as instruções
	Aqui:
	
	http://docs.opencv.org/doc/tutorials/introduction/windows_visual_studio_Opencv/windows_visual_studio_Opencv.html
	
	The documentation of all the functions used in this program can be found here:
	
	http://opencv.willowgarage.com/documentation/c/imgproc__image_processing.html
	
	
	Para terminar foi predefinido a tecla <ESC> para terminar aplicação.
	
	
**//*------------------------------------------------------------------------*/

int MeasureFunctionSystem()
{
	//------------------------------------------------------------------------------
	CvScalar redColor = CV_RGB(255,0,0);
	CvScalar blackColor = CV_RGB(0,0,0);
	CvScalar greenColor = CV_RGB(0,119,73);
	//------------------------------------------------------------------------------
	
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	Estrutura contendo a imagem capturada pela câmera. Será inicializado
	automaticamente pela função que recupera o quadro da câmera.
	~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	
	IplImage *frame = NULL;
	
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	A imagem capturada pela câmera é colorida, mas o OpenCV precisa de uma escala de cinza
	imagem para calcular as bordas e os contornos. Após aquisição de imagem
	o quadro é convertido em escala de cinza e armazenado aqui.

	Esta imagem deve ter a mesma largura e a mesma altura que a capturada
	quadro, mas nesta parte do código ainda não sabemos o tamanho da imagem como
	isso depende das características da câmera.

	Mantemos essa variável não inicializada até não capturarmos o primeiro quadro.
	~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	
	IplImage *grey  = NULL;
	
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		Para calcular os contornos, precisamos primeiro extrair as bordas que podem
	ser calculado com diferentes algoritmos; os mais comuns usados são Sobel,
	Prewit e Canny. Eu prefiro usar Sobel porque tende a dar melhor
	resultados e o limiar não é crítico.

	Para uma explicação do operador de Sobel, verificar link:
	http://en.wikipedia.org/wiki/Sobel_operator

	A variável abaixo é onde as bordas da imagem são armazenadas após o
	O operador de Sobel foi calculado.

	O resultado é outra imagem com as bordas destacadas (aqui é uma
	exemplo: http://en.wikipedia.org/wiki/File:Bikesgray_sobel.JPG)

	Esta imagem deve ter a mesma largura e a mesma altura que a capturada
	quadro, mas nesta parte do código ainda não sabemos o tamanho da imagem como
	isso depende das características da câmera.

	Mantemos essa variável não inicializada até não capturarmos o primeiro quadro.
	~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	
	IplImage *canny = NULL;
	
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	A variável abaixo representa a interface que o OpenCV nos dá para capturar
	a imagem da câmera. A coisa muito legal que o OpenCV nos oferece é
	que este 'CvCapture' esconde para nós todos os detalhes dos milhares de cames
	presente no mercado e nos dá um método simples para obter um quadro.

	A variável 'capture' foi inicializada para recuperar a imagem do
	primeira câmera disponível (-1). Se você tiver mais de uma câmera conectada
	e você quer capturar de um específico que você só tem que substituir
	-1 com o número atribuído a essa câmera.

	Qual é esse número? No Linux é o valor que você encontra em / dev / video *.
	Por exemplo, se você tem três câmeras, você terá:

	/ dev / video0
	/ dev / video1
	/ dev / video2

	No Windows eu não sei, mas desde que as câmeras são numeradas progressivamente
	não será difícil encontrar qual código está associado a qual câmera;
	apenas tente e veja ...
	~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

	//CvCapture *capture = cvCaptureFromCAM(CV_CAP_ANY);
	CvCapture *capture = cvCaptureFromCAM(1);
	
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	A constante abaixo é o fator de conversão entre pixel e milímetros
	e é o valor usado para passar do tamanho do objeto expresso em
	pixel para o tamanho real.

	Existem duas maneiras de obter este valor: uma é saber exatamente o foco
	distância da lente, a distância do objeto e o tamanho do CCD
	ou sensor CMOS da câmera e aplicar uma fórmula que não me lembro.
	Eu acho que esse método não é muito prático e por isso eu sugiro que você use
	este:

	* coloque a câmera no lugar, a uma distância razoável do avião onde
	você quer colocar os objetos para medir

	* set conversion_factor = 1

	* Coloque um objeto no plano e execute o programa. Desde a conversão
	fator é definido como 1, você obterá a medida do objeto em pixels,
	digamos pixel_width, pixel_height.

	* medir o objeto, vamos chamar essa medida de real_width, real_height

	* o fator de conversão é real_width / pixel_width

	É claro que este não é um sistema de medição muito preciso, pois depende
	webcams baratas com lentes baratas, mas se a câmera estiver bem posicionada e
	fixa a medida será precisa o suficiente.
	~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	
	//const float conversion_factor = 1;
	const float conversion_factor = 1;

	
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	As duas instruções que se seguem não são realmente necessárias, mas eu as coloco
	apenas por motivos didáticos.

	A primeira instrução define como a janela de saída será chamada (quadro)
	e que seu tamanho será definido automaticamente pelo tamanho do quadro
	mostrando.

	A segunda instrução usa o nome da janela definido acima e move para o
	canto superior esquerdo do monitor.

	As duas instruções não abrem uma janela ou a movem, mas apenas
	OpenCV o que fazer quando mais tarde vamos mostrar a imagem. No final de
	código você verá esta função:

	cvShowImage ("frame", frame);

	que imprime o conteúdo do quadro na janela chamada "frame", que
	tem as características que definimos aqui.
	~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	cvNamedWindow("frame", CV_WINDOW_AUTOSIZE);
	//cvNamedWindow("frame", CV_WINDOW_FREERATIO);
	cvMoveWindow("frame", 0, 0);
	
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	Abaixo é declarada e inicializada a fonte usada para imprimir na tela
	tamanho do objeto.
	~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	
	CvFont font;
	//cvInitFont(&font, CV_FONT_HERSHEY_SIMPLEX, 0.75, 0.75, 0, 2);
	cvInitFont(&font, CV_FONT_HERSHEY_SIMPLEX, 0.75, 0.75, 0, 2);

	
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	As estruturas abaixo são os contentores de dados usados para armazenar o resultado
	a função cvFindContours que calcula o contorno do objeto
	dada como entrada a imagem de Sobel.
	~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	
	CvSeq *contours = 0;
	CvMemStorage *g_storage = cvCreateMemStorage(0);

	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	Estas variáveis serão preenchidas com a largura e a altura do objeto
	nós estamos medindo.
	~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

	int obj_width  = 0;		//largura
	int obj_height = 0;		//altura
	
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	Como as webcams são frequentemente caracterizadas por um ruído de fundo que pode
	fazer com que a imagem adquirida seja um pouco instável Eu faço uma suavização temporal
	nas coordenadas dos vértices do recto envolvente mínimo.

	Por suavização temporal quero dizer que a posição dos vértices do mínimo
	rect que envolve o objeto são calculados pelo algoritmo, mas também são
	averged com suas posições anteriores. Isso determina um mais estável
	medição.

	A suavização temporal é calculada usando a fórmula abaixo:

	smoothed_val = alpha * last_smoothed + (1.0-alpha) * measured_val

	uma vez calculada a suavização, atribuímos last_smoothed = smoothed_val
	e estamos prontos para suavizar novamente.

	Fazendo desta forma, e escolhendo um valor apropriado de alfa (que pode
	ser no intervalo entre 0,0 e 1,0), podemos levar em consideração os valores anteriores.

	Quanto maior o alfa, mais suave será o resultado que se traduz em
	uma reatividade mais lenta do sistema para as mudanças.

	Tente experimentar alterar o valor abaixo e veja a diferença.
	~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	
	// Smooth factor -> ajustes+++
	//float alpha = 0.6;
	float alpha = 0.9;

	
	//Últimas coordenadas de vértices usadas para suavizar/suavização temporal
	CvPoint2D32f last_vertice[4];
	
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	Loop de processamento: aqui começa o coração do processamento. O loop é
	infinito e será terminado pressionando o botão <ESC> ou no caso
	de erro.
	~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	
	while(true)
	{
		/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		Largura e altura do objeto são definidas como zero, pois ainda não calculamos.
		~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
		
		obj_width = obj_height = 0;		//inicializar variaveis de altura/largura a zero
		
		/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		Adquira a imagem da câmera. Se por algum motivo o quadro não puder ser
		capturou a função cvQueryFrame retorna NULL. Como não há nada
		para fazer sem moldura, saímos com uma mensagem de erro.

		Observe como é simples e elegante adquirir um quadro com as funções
		do OpenCV.
		~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

		frame = cvQueryFrame(capture);
		
		if(!frame)	//se nao for possivel adquirir imagem
		{
			fprintf( stderr, "ERROR: frame is null...\n" );
			fprintf( stderr, "ERRO: frame é nula (Verificar Camera/CCD!)...\n" );
			exit(0);
		}
		
		/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		Se as imagens 'cinza' e 'canny' ainda não foram definidas, podemos fazê-lo
		agora, desde agora sabemos o tamanho do quadro.

		Esta inicialização será feita apenas uma vez
		~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

		if(grey==NULL)
		{
			grey = cvCreateImage(cvSize(frame->width, frame->height), IPL_DEPTH_8U, 1);
		}			
		if(canny==NULL)
		{
			canny = cvCreateImage(cvSize(frame->width, frame->height), IPL_DEPTH_8U, 1);
		}
		/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		Como o operador Canny pode ser calculado apenas em imagens em escala de cinza e
		a imagem capturada é certamente colorida transformamos a imagem de entrada
		em um grayscale.
		~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

		cvCvtColor(frame, grey, CV_BGR2GRAY);
		
		/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		Para evitar que o ruído de fundo fornecido pela câmera faça com que o sistema
		muito sensível a pequenas variações, borramos um pouco a imagem. Isso não
		substitua o alisamento temporal sobre o qual falo acima.

		Desfocando a imagem é uma das etapas "obrigatórias" executadas na imagem
		processamento antes de qualquer outra transformação de imagem, porque isso tendem a
		elimine os detalhes que são pequenos demais para serem significativos. Borrão está em
		fato um filtro de baixa passagem.

		Os parâmetros desta função são:

		* fonte
		* destino
		* tipo
		* abertura

		a abertura é quão forte será a filtragem. A maior abertura é a
		turva a imagem será. Nós não precisamos blurr muito como nós não
		quero perder muitos detalhes.
		~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

		//cvSmooth(grey, grey, CV_BLUR, 3);
		cvSmooth(grey, grey, CV_BLUR, 3);

		
		/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		Se você quiser ver como a imagem suavizada parece, descomente a linha
		abaixo.
		~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
		
		cvShowImage("grey", grey);

		/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		Calcula a borda usando o algoritmo Canny.
		Os parâmetros desta função são:
				* fonte
				* destino
     			* limiar1
				* limiar2
				* abertura
				* source
	    		* destination
    		    * threshold1
		        * threshold2
		        * aperture
			

		Podemos encontrar o significado desses dois limites aqui:
		http://en.wikipedia.org/wiki/Sobel_operator

		abertura é o tamanho do kernel Sobel estendido, deve ser 1, 3, 5 ou 7

		Sinta-se à vontade para experimentar a alteração dos limites para ver como funciona.
		O intervalo dos limites é 0-255.
		~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

		//cvCanny(grey, canny, 180, 210, 3);
		cvCanny(grey, canny, 250, 255, 3);


		/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		Se você quiser ver como a imagem astuta parece descomentar a linha
		abaixo.
		~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
		
		cvShowImage("canny", canny);

		/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		Finalmente, calcule os contornos. Para uma descrição detalhada desta
		função ver link: http://opencv.willowgarage.com/documentation/c/imgproc_structural_analysis_and_shape_descriptors.html?highlight=cvfindcontours#cvFindContours		
		~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

		cvFindContours(canny, g_storage, &contours, sizeof(CvContour), CV_RETR_EXTERNAL);
		
		/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		Processe os contornos para encontrar o retângulo delimitador mínimo
		contendo nosso objeto.
		~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
		
		// A variável total contém agora a primeira sequência de contorno
		CvSeq *total = contours;

		// Se houver pelo menos um contorno, posso continuar processando
		if(total)
		{
			/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
			Os contornos vêm em seqüências diferentes, uma seqüência para cada
			componente conectado. Já que assumimos que haverá apenas um objeto
			em vista, se temos mais de um componente conectado isso será
			considerado como parte dos detalhes do objeto.

			Para isso, colocamos todos os contornos juntos em uma única sequência de contorno
			(total).
			~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
		
			// Mover para a próxima sequência (a primeira já foi atribuída ao total)
			CvSeq *ptr = total->h_next;
			
			//Analise todas as sequências para adicionar cada elemento ao 'total'
			while(ptr)
			{
				// Adicione cada elemento da sequência atual para 'total'
				for (int i=0; i < ptr->total; ++i)
				{
					// Recupere o elemento com o indice i
					CvPoint *point = (CvPoint*)cvGetSeqElem(ptr, i);
					
					// Adicionar a 'total'
					cvSeqPush(total, point);
				}
				
				// Mover para a próxima sequência e continuar em loop até que haja mais sequências
	    		ptr = ptr->h_next;
			}
		
			/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
			A função que segue calcula o retângulo mínimo que
			encerra os contornos passados como argumento.
			~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

			CvBox2D box = cvMinAreaRect2(total);

			/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
			A estrutura retornada por cvMinAreaRect2 não tem uso prático para nossos propósitos. 
			Temos que pegar o vértice do retângulo. Nós conseguimos através da função cv BoxPoints.
			~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

			CvPoint2D32f vertice[4];
			cvBoxPoints(box, vertice);

			/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
			Agora que temos os vértices podemos realizar a suavização temporal que falamos acima.
			~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

			for(int i=0; i<4; i++)
			{
				// Smooth coordenada x do vertice
				vertice[i].x = alpha*last_vertice[i].x + (1.0-alpha)*vertice[i].x;
				
				// Smooth coordenada y do vertice
				vertice[i].y = alpha*last_vertice[i].y + (1.0-alpha)*vertice[i].y;
				
				// Atribuir os valores suavizados atuais como last_vertice para a próxima suavidade
				last_vertice[i] = vertice[i];
			}

			/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
			Com os vértices podemos agora calcular o tamanho do objeto.

			O tamanho do objeto é calculado aplicando a fórmula que
			dá a distância entre dois pontos em um espaço bidimensional; isto
			fórmula é o teorema de Pitágoras.

			Como lidamos com um retângulo, considerando qualquer vértice V, podemos
			calcular seus dois tamanhos (largura e altura) calculando a distância
			de V do vértice anterior e a distância de V a partir do próximo
			vértice. Esta é a razão pela qual eu calculo a distância entre
			vértice [0] / vértice [3] e vértice [0] / vértice [1].

			A largura e a altura de atribuição aqui são arbitrárias, já que não
			Conhecer qualquer informação sobre o objeto para nomear corretamente o tamanho.
			~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

			obj_width  = conversion_factor*sqrt((vertice[0].x-vertice[3].x)*(vertice[0].x-vertice[3].x) + (vertice[0].y-vertice[3].y)*(vertice[0].y-vertice[3].y));
			obj_height = conversion_factor*sqrt((vertice[0].x-vertice[1].x)*(vertice[0].x-vertice[1].x) + (vertice[0].y-vertice[1].y)*(vertice[0].y-vertice[1].y));;

			/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
			Desenhe na tela o retângulo mínimo que envolve os contornos.

			A função cvLine desenha uma linha de um ponto a outro e
			aceita apenas coordenadas inteiras; esta é a razão pela qual eu crio
			dois temporários CvPoint e porque eu uso a função cvRound.
			~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

			CvPoint pt0, pt1;
			for(int i = 0; i < 4; i++ )
			{
				pt0.x = cvRound(vertice[i].x);
				pt0.y = cvRound(vertice[i].y);
				pt1.x = cvRound(vertice[(i+1)%4].x);
				pt1.y = cvRound(vertice[(i+1)%4].y);
				cvLine(frame, pt0, pt1, redColor, 1, CV_AA, 0);
			}

			/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
			Se a largura e a altura forem diferentes de zero, imprimimos na tela
			o tamanho do objeto.
			~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

			if(obj_width && obj_height)
			{
				char text[100];
				sprintf(text, "Dimensoes -> Largura*Altura = %d x %d mm", obj_width, obj_height);
				cvPutText(frame, text, cvPoint(20, 450), &font, redColor);
			}
			
			// Aqui termina o processamento dos contornos, se houver algum
		}
		
		/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		Mostre a imagem adquirida eventualmente com o retângulo detectado e o tamanho do objeto.
		~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
		
		cvShowImage("frame", frame);

		/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		Verifique as teclas pressionadas e termine se <ESC> for pressionado; caso contrário, continue com o loop de processamento.
		~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
		
		int key = cvWaitKey(10) & 255;		
		if(key == 27) break;

		// Aqui termina o loop de processamento (você pode terminá-lo pressionando <ESC>
	}

	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	Limpar e terminar

	Como este é o fim do programa, todos os recursos alocados serão
	lançado automaticamente, mas como é uma boa prática de programação
	liberar os recursos e, neste caso, também tem um propósito didático
	aqui está...
	~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

	cvReleaseImage(&frame);
	cvReleaseImage(&canny);
	cvReleaseImage(&grey);
	cvDestroyWindow("frame");
	return obj_height;
}

int main(int argc, char **argv)
{
	cout << "A Iniciar MachineVision" << '\n';
	MeasureFunctionSystem();
	int largura = MeasureFunctionSystem();
	cout << "Largura: " + largura << '\n';
	return 0;
}




