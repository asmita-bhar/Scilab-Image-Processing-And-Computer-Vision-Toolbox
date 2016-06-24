/********************************************************
Author: Asmita Bhar
********************************************************/

#include <numeric>
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/opencv.hpp"
#include <iostream>
using namespace cv;
using namespace std;
extern "C"
{
	#include "api_scilab.h"
	#include "Scierror.h" 
	#include "BOOL.h"
	#include <localization.h>
	#include <sciprint.h>
	#include "../common.h"
	int opencv_HoughLines(char *fname, unsigned long fname_len)
	{
		//error management variable		
		SciErr sciErr;	
		
		//variable declaration	
		int *piAddr1 = NULL;
		int *piAddr2 = NULL;
		int inputPara = 0;
		int m1=0, n1=0, m2=0, n2=0;
		int firstRow=0, firstCol=0, lastRow=0, lastCol=0;
		int IRows = 0, ICols = 0, Rows=0, Cols=0; //r=0,c=0;;
		int i=0, j=0, count=0;
		double a=0, b=0, c=0;
		int row=0, col=0; 
		int *piLen = NULL;
		int *Len = NULL;
		char **pstData = NULL;
		char **pstData1 = NULL;
		double *rho = NULL;
		double *theta = NULL;
		double *points = NULL;	

		//check the number of input and output arguments
		CheckInputArgument(pvApiCtx, 3, 3);
    		CheckOutputArgument(pvApiCtx, 1, 1);

		Mat image;
		retrieveImage(image,1);

		//input the second argument 
		sciErr = getVarAddressFromPosition(pvApiCtx,2, &piAddr1);
		if (sciErr.iErr)
    		{
    		    printError(&sciErr, 0);
    		    return 0;
    		}
		//Check that the first input argument is a real matrix (and not complex)
    		if ( !isDoubleType(pvApiCtx, piAddr1) ||  isVarComplex(pvApiCtx, piAddr1) )
    		{
        		Scierror(999, "%s: Wrong type for input argument #%d: A real matrix expected.\n", fname, 2);
        		return 0;
    		}

    		//get matrix of theta values
    		sciErr = getMatrixOfDouble(pvApiCtx, piAddr1, &m1, &n1, &theta);
    		if (sciErr.iErr)
    		{
        		printError(&sciErr, 0);
        		return 0;
    		}
		
		if(m1!=1)
		{		
			Scierror(999, "%s: Wrong size for input arguments: Same size expected.\n", fname, 2);
        		return 0;	
		}

		//input the second argument 
		sciErr = getVarAddressFromPosition(pvApiCtx,3, &piAddr2);
		if (sciErr.iErr)
    		{
    		    printError(&sciErr, 0);
    		    return 0;
    		}
		//Check that the first input argument is a real matrix (and not complex)
    		if ( !isDoubleType(pvApiCtx, piAddr2) ||  isVarComplex(pvApiCtx, piAddr2) )
    		{
        		Scierror(999, "%s: Wrong type for input argument #%d: A real matrix expected.\n", fname, 3);
        		return 0;
    		}

    		//get matrix of rho values
    		sciErr = getMatrixOfDouble(pvApiCtx, piAddr2, &m2, &n2, &rho);
    		if (sciErr.iErr)
    		{
        		printError(&sciErr, 0);
        		return 0;
    		}
		
		if(m2!=1)
		{		
			Scierror(999, "%s: Wrong size for input arguments: Same size expected.\n", fname, 3);
        		return 0;	
		}
		
		//check for consistency in rho and theta matrix		
		if(n1!=n2)
		{		
			Scierror(999, "%s: Wrong size for input arguments: Same size expected.\n", fname, 2);
        		return 0;	
		}

		
		
		//calculating the size of the image matrix
		IRows = image.rows;
		ICols = image.cols;
		
		//declaring the image boundary
		firstRow = 1;
		firstCol = 1;
		lastRow = IRows;
		lastCol = ICols;

		//declaring points matrix
		points = (double *)malloc(sizeof(double)*m1*4);

		for(i=0; i<n1; i++)
		{
			/* The line equation is (x-1)cos(theta) + (y-1)sin(theta) = rho 
			i.e. x.cos(theta) + y.sin(theta) = rho + cos(theta) + sin(theta).
			Comparing with the normal line equation a.x + b.y = c we get
			a = cos(theta), b = sin(theta) and c = rho + cos(theta) + sin(theta). */
			count=0;
			a = cos(theta[i]);
			b = sin(theta[i]);
			c = rho[i]+a+b;

			//checking for intersections with left image border
			row = round((-b*firstCol +c)/a);
			if((row>=firstRow) && (row<=lastRow))
			{
				points[i+n1*count] = row;
				points[i+n1*(count+1)] = firstCol;
				count += 2;
			}

			//checking for intersections with right image border
			row = round((-b*lastCol +c)/a);
			if((row>=firstRow) && (row<=lastRow))
			{
				points[i+n1*count] = row;
				points[i+n1*(count+1)] = lastCol;
				count += 2;
			}
			
			//checking for intersections with top image border if we have not found two intersection points
			if(count<4)
			{
				col = round((-a*firstRow +c)/b);
				if((col>=firstCol) && (col<=lastCol))
				{
					points[i+n1*count] = firstRow;
					points[i+n1*(count+1)] = col;
					count += 2;
				}
			}
			
			//checking for intersections with bottom image border if we have not found two intersection points
			if(count<4)
			{
				col = round((-a*lastRow +c)/b);
				if((col>=firstCol) && (col<=lastCol))
				{
					points[i+n1*count] = lastRow;
					points[i+n1*(count+1)] = col;
					count += 2;
				}
			}
			
			//if the line does not intersect with the image border set the points of intersection to -1
			if(count<4)
			{
				for(j=0; j<4; j++)
				{
					points[i+n1*j] = 0;
				}
			}
			
			
		}

		//creating matrix of points
		sciErr = createMatrixOfDouble(pvApiCtx, nbInputArgument(pvApiCtx) + 1, n1, 4, points);
		if(sciErr.iErr)
		{
			printError(&sciErr, 0);
			return 0;
		}

		//return the output arguments to the scilab engine
		AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
		ReturnArguments(pvApiCtx);

		return 0;
	}
}
	
		

