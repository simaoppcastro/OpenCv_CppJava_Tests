//------------------------------------------------------------------------------
#include <opencv/cv.h>
#include <opencv/highgui.h> 
#include <stdio.h>
#include <vector>
#include <algorithm>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
//------------------------------------------------------------------------------
CvScalar red = CV_RGB(255,0,0);
CvScalar black = CV_RGB(0,0,0);
//------------------------------------------------------------------------------

/*------------------------------------------------------------------------*//**
	Real time object measurement.
	
	The algorithm is based on the detection of the contour of the object we want
	to measure and the calculation of the minimal rectangle that encloses it.
	
	Prerequisites:
	   * the object should be placed on an uniform background
	   * the object should have enough contrast to be detected by the webcam
	   * the camera should positioned on a fixed distance from the object
	   * just a single object can be measured; if you put two or more objects
	     you will get the size of the minimal area containing all them.
	
	The program is C++ but uses the OpenCV C interface which for a beginner I think
	is much easier to learn.
	
	To compile and run the program under Linux, or under Windows with GCC just
	type 'make run'. To compile and run under Visual Studio follow the instructions
	here:
	
	http://docs.opencv.org/doc/tutorials/introduction/windows_visual_studio_Opencv/windows_visual_studio_Opencv.html
	
	The documentation of all the functions used in this program can be found here:
	
	http://opencv.willowgarage.com/documentation/c/imgproc__image_processing.html
	
	
	To terminate the program press <ESC>.
	
	
**//*------------------------------------------------------------------------*/

int main(int argc, char **argv)
	{
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		Structure containing the image captured by the camera. It will be initialized
	   automatically by the function that retrieves the frame from the camera.
	~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	
	IplImage *frame = NULL;
	
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		The image captured by the camera is colored, but OpenCV needs a grayscale
	   image to calculate the borders and the contours. After image acquisition
	   the frame is converted to grayscale and stored here.
	   
	   This image must have the same width and the same height as the captured
	   frame, but in this part of the code we do not yet know the image size as
	   it depends by the camera characteristics. 
	   
	   We keep this variable uninitialized till we do not capture the first frame.
	~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	
	IplImage *grey  = NULL;
	
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		To calculate the contours we need first to extract the borders which can
	   be calculated with different algorithms; the most common used are Sobel,
	   Prewit and Canny. I prefer to use Sobel because it tends to give better
	   results and the threshold is not critical.
	   
	   For an explanation of the Sobel operator look here:
	   http://en.wikipedia.org/wiki/Sobel_operator
	   
	   The variable below is where the borders of the image are stored after the
	   Sobel operator has been calculated.
	   
	   The result is another image with the borders highlighted (here is an
	   example: http://en.wikipedia.org/wiki/File:Bikesgray_sobel.JPG)
	   
	   This image must have the same width and the same height as the captured
	   frame, but in this part of the code we do not yet know the image size as
	   it depends by the camera characteristics. 
	   
	   We keep this variable uninitialized till we do not capture the first frame.
	~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	
	IplImage *canny = NULL;
	
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		The variable below represents the interface OpenCV gives to us to capture
		the image from the camera. The very nice thing that OpenCV offers to us is
		that this 'CvCapture' hides to us all the details of the thousands of cams
		present in the market and gives to us a simple method to get a frame.
		
		The variable 'capture' has been initialized to retrieve the image from the
		first available camera (-1). If you have more than one camera connected
		and you want to capture from a specific one you just have to substitute
		-1 with the number assigned to that camera.
		
		What is this number? On Linux is the value you find in /dev/video*.
		For example if you have three cameras you will have:
		
		/dev/video0
		/dev/video1
		/dev/video2
		
		On Windows I do not know, but since the cameras are numbered progressively
		it will be not difficult to find what code is associated to what camera;
		just try and see...
	~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

	//CvCapture *capture = cvCaptureFromCAM(CV_CAP_ANY);
	CvCapture *capture = cvCaptureFromCAM(1);
	
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		The constant below is the conversion factor between pixel and millimeters
		and is the value used to pass from the size of the object espressed in
		pixel to the real size.
		
		There are two ways to get this value: one is to know exactly the focal
		distance of the lens, the distance of the object and the size of the CCD
		or CMOS sensor of the camera and apply a formula which I do not remember.
		I think this method not much practic and for this I suggest you to use
		this one:
		
		* put the camera in place, at a reasonable distance from the plane where
		  you want to put the objects to measure

		* set conversion_factor=1
		
		* put an object to the plane and run the program. Since the conversion
		  factor is set to 1 you will get the measurement of the object in pixels,
		  let's say pixel_width, pixel_height.
		
		* measure the object, let's call this measurement real_width, real_height
		
		* the conversion_factor is real_width/pixel_width

		Of course this is not a very precise measurement system as it relies on
		cheap webcams with cheap lenses, but if the camera is well positioned and
		fixed the measurement will be accurate enough.
	~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	
	const float conversion_factor = 1;
	
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		The two instruction that follows are not really necessary, but I put them
		just for didactical reasons.
		
		The first instruction defines how the output window will be called (frame)
		and that its size will be defined automatically by the size of the frame
		shown.
		
		The second instruction uses the window name defined above and move to the
		top left corner of the monitor.
		
		The two instructions do not open a window or move it, but just tell to
		OpenCV what to do when later we will show the image. At the end of the
		code you will see this function:
		
		cvShowImage("frame", frame);
		
		which prints the content of the frame in the window called "frame" which
		has the caracteristics we defined here.
	~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

	cvNamedWindow("frame", CV_WINDOW_AUTOSIZE);
	cvMoveWindow("frame", 0, 0);
	
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		Below I declare and initialize the font that I use to print on screen the
		size of the object.
	~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	
	CvFont font;
	cvInitFont(&font, CV_FONT_HERSHEY_SIMPLEX, 0.75, 0.75, 0, 2);
	
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		The structures below are the data containers used to store the result by
		the function cvFindContours which calculates the contour of the object
		given as input the Sobel image.
	~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	
	CvSeq *contours = 0;
	CvMemStorage *g_storage = cvCreateMemStorage(0);

	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		These variables will be filled with the width and the height of the object
		we are measuring.
	~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

	int obj_width  = 0;
	int obj_height = 0;
	
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		Since the webcams are often characterized by a background noise which can
		cause the acquired image to be a bit instable I perform a temporal smoothing
		on the coordinates of the vertexes of the minimum enclosing rect.
		
		By temporal smoothing I mean that the position of the vertexes of the minimum
		rect that encloses the object are calculated by the algorithm but also are
		averged with their previous positions. This determines a more stable
		measurement.
		
		The temporal smoothing is calculated using the formula below:
		
		smoothed_val = alpha*last_smoothed + (1.0-alpha)*measured_val
		
		once the smoothing has been calculated we assign last_smoothed = smoothed_val
		and we are ready to smooth again.
		
		Doing in this way, and choosing an appropriate value of alpha (which can
		range between 0.0 and 1.0), we can take into consideration the past values.
		
		The higher the alpha the smoother will be the result which translates into
		a slower reactivity of the system to the changes.
		
		Try to experiment changing the value below and see the difference.
	~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	
	// Smooth factor
	float alpha = 0.6;
	
	// Last vertexes coordinates used for temporal smooth
	CvPoint2D32f last_vertici[4];
	
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		Processing loop: here starts the heart of the processing. The loop is
		infinite and will be terminated by pressing the <ESC> button or in case
		of error.
	~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	
	while(true)
		{
		/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
			Object width and height are set to zero since we do not yet calculate it.
		~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
		
		obj_width = obj_height = 0;
		
		/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
			Acquire image from camera. If for any reason the frame could not be
			captured the function cvQueryFrame returns NULL. Since there is nothing
			to do without frame we exit with an error message.
			
			Notice how simple and elegant is to acquire a frame with the functions
			of OpenCV.
		~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

		frame = cvQueryFrame(capture);
		
		if(!frame)
			{
			fprintf( stderr, "ERROR: frame is null...\n" );
			exit(0);
			}
		
		/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
			If the images 'grey' and 'canny' have not yet been defined we can do it
			now, since now we know the size of the frame.
			
			This initialization will be done only once.
		~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

		if(grey==NULL)
			grey = cvCreateImage(cvSize(frame->width, frame->height), IPL_DEPTH_8U, 1);
			
		if(canny==NULL)
			canny = cvCreateImage(cvSize(frame->width, frame->height), IPL_DEPTH_8U, 1);

		/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
			Since the Canny operator can be calculated only on grayscale images and
			the image captured is certainly colored we transform the input image
			into a grayscale one.
		~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

		cvCvtColor(frame, grey, CV_BGR2GRAY);
		
		/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
			To avoid that the background noise given by the camera makes the system
			too sensitive to small variations we blur a bit the image. This does not
			substitute the temporal smoothing about which I talk above.
			
			Blurring the image is one of the 'must do' steps performed in image
			processing prior any other image transformation because this tend to
			eliminate the details that are too small to be meaningful. Blur is in
			fact a low pass filter.
			
			The parameters of this function are:
			
			   * source
			   * destination
			   * type
			   * aperture
			   
			aperture is how strong will be the filtering. The bigger aperture is the
			blurred the image will be. We do not need to blurr too much as we do not
			want to loose too many details.
		~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

		cvSmooth(grey, grey, CV_BLUR, 3);
		
		/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
			If you want to see how the smoothed image looks like uncomment the line
			below.
		~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
		
		//cvShowImage("grey", grey);

		/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
			Calculates the border using the Canny algorithm.
			
			The parameters of this function are:
				* source
				* destination
			   * threshold1
			   * threshold2
			   * aperture
			
			You can find the meaning of these two thresholds here:
			http://en.wikipedia.org/wiki/Sobel_operator
			
			aperture is the size of the extended Sobel kernel, must be 1, 3, 5 or 7
			
			Feel free to experiment changing the thresholds to see how it works.
			The range of the thresholds is 0-255.
		~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

		cvCanny(grey, canny, 180, 210, 3);

		/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
			If you want to see how the canny image looks like uncomment the line
			below.
		~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
		
		//cvShowImage("canny", canny);

		/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
			Finally calculate the contours. For a detailed description of this
			function look here:
			
			http://opencv.willowgarage.com/documentation/c/imgproc_structural_analysis_and_shape_descriptors.html?highlight=cvfindcontours#cvFindContours
			
		~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

		cvFindContours(canny, g_storage, &contours, sizeof(CvContour), CV_RETR_EXTERNAL);
		
		/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
			Process the contours in order to find the minimal enclosing rectangle
			containing our object.
		~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
		
		// The variable totale contains now the first contour sequence
		CvSeq *totale = contours;

		// If there is at least one contour I can continue processing
		if(totale)
			{
			/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
				The contours come on different sequences, one sequence for each
				connected component. Since we assume that there will be only an object
				in view if we have more than one connected component this will be
				considered as part of the details of the object.
				
				For this we put all the contours together in a single contour sequence
				(totale).
			~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
		
			// Move to the next sequence (the first has already been assigned to total)
			CvSeq *ptr = totale->h_next;
			
			// Scan all the sequences to add each element to 'totale'
			while(ptr)
				{
				// Add each element of the current sequence to 'totale'
				for (int i=0; i < ptr->total; ++i)
					{
					// Retrieve the i-th element
					CvPoint *point = (CvPoint*)cvGetSeqElem(ptr, i);
					
					// Add to 'total'
					cvSeqPush(totale, point);
					}
				
				// Move to next sequence and continue loop till there are more sequences
				ptr = ptr->h_next;
				}
		
			/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
				The function that follows calculates the minimal rectangle that
				encloses the contours passed as argument.
			~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

			CvBox2D box = cvMinAreaRect2(totale);

			/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
				The structure returned by cvMinAreaRect2 does ot have practical use
				for our purposes. We have to get the vertex of the rectangle. We get
				it via the function cvBoxPoints.
			~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

			CvPoint2D32f vertici[4];
			cvBoxPoints(box, vertici);

			/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
				Now that we have the vertexes we can perform the temporal smoothing
				we talked above.
			~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

			for(int i=0; i<4; i++)
				{
				// Smooth coordinate x of the vertex
				vertici[i].x = alpha*last_vertici[i].x + (1.0-alpha)*vertici[i].x;
				
				// Smooth coordinate y of the vertex
				vertici[i].y = alpha*last_vertici[i].y + (1.0-alpha)*vertici[i].y;
				
				// Assign the present smoothed values as last_vertici for the next smooth
				last_vertici[i] = vertici[i];
				}

			/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
				With the vertexes we can now calculate the size of the object.
				
				The size of the object is calculated by applying the formula that
				gives the distance between two points in a bidimensional space; this
				formula is the theorem of Pitagoras.
				
				Since we deal with a rectangle, considering any vertex V we can
				calculate its two sizes (width and height) by calculating the distance
				of V from the previous vertex and the distance of V from the next
				vertex. This is the reason why I calculate the distance between 
				vertici[0]/vertici[3] and vertici[0]/vertici[1].
				
				The attribution width and height here is arbitrary since we do not
				know any information about the object to name correctly the size.
			~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

			obj_width  = conversion_factor*sqrt((vertici[0].x-vertici[3].x)*(vertici[0].x-vertici[3].x) + (vertici[0].y-vertici[3].y)*(vertici[0].y-vertici[3].y));
			obj_height = conversion_factor*sqrt((vertici[0].x-vertici[1].x)*(vertici[0].x-vertici[1].x) + (vertici[0].y-vertici[1].y)*(vertici[0].y-vertici[1].y));;

			/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
				Draw on screen the minimal rectangle that encloses the contours.
				
				The cvLine function draws a line from one point to another and
				accepts only integer coordinates; this is the reason why I create
				two temporary CvPoint and why I use the cvRound function.
			~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

			CvPoint pt0, pt1;
			for(int i = 0; i < 4; i++ )
				{
				pt0.x = cvRound(vertici[i].x);
				pt0.y = cvRound(vertici[i].y);
				pt1.x = cvRound(vertici[(i+1)%4].x);
				pt1.y = cvRound(vertici[(i+1)%4].y);
				cvLine(frame, pt0, pt1, red, 1, CV_AA, 0);
				}

			/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
				If both width and height is different than zero we print on screen
				the size of the object.
			~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

			if(obj_width && obj_height)
				{
				char text[100];
				sprintf(text, "Dimension = %d x %d mm", obj_width, obj_height);
				cvPutText(frame, text, cvPoint(20, 450), &font, black);
				}
			
			// Here ends the processing of the contours, if any
			}
		
		/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
			Show the image acquired with eventually the detected rectangle and the
			size of the object.
		~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
		
		cvShowImage("frame", frame);

		/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
			Check for keypress and terminate if <ESC> is pressed, otherwise continue
			with the processing loop.
		~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
		
		int key = cvWaitKey(10) & 255;		
		if(key == 27) break;

		// Here ends the processing loop (you can terminate it by pressing <ESC>
		}

	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		Cleanup and terminate
		
		Since this is the end of the program all the resources allocated will be
		automatically released, but since it is a good programming practice to
		release the resources and in this case also has a didactical purpose
		here is...
	~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

	cvReleaseImage(&frame);
	cvReleaseImage(&canny);
	cvReleaseImage(&grey);
	cvDestroyWindow("frame");
	return 0;
	}



