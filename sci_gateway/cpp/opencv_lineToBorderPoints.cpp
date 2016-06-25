/********************************************************
Author: Asmita Bhar
********************************************************/

#include <numeric>
//#include "opencv2/core/core.hpp"
//#include "opencv2/highgui/highgui.hpp"
//#include "opencv2/opencv.hpp"
#include <iostream>
//using namespace cv;
using namespace std;
extern "C"
{
	#include "api_scilab.h"
	#include "Scierror.h" 
	#include "BOOL.h"
	#include <localization.h>
	#include <sciprint.h>
	//#include "../common.h"
	int opencv_lineToBorderPoints(char *fname, unsigned long fname_len)
	{
		//error management variable		
		SciErr sciErr;		
		
		//Variable declaration
		int *piAddr1 = NULL;
		int *piAddr2 = NULL;
		int m1=0, n1=0, m2=0, n2=0;
		double firstRow=0, firstCol=0, lastRow=0, lastCol=0;
		double row=0, col=0;
		int i=0, j=0, count=0;
		double a=0, b=0, c=0;
		double *lines = NULL;
		double *size = NULL;
		double *points = NULL;

		//check the number of input and output arguments
		CheckInputArgument(pvApiCtx, 2, 2);
    		CheckOutputArgument(pvApiCtx, 1, 1);

		//input the first argument
		sciErr = getVarAddressFromPosition(pvApiCtx,1, &piAddr1);
		if (sciErr.iErr)
    		{
    		    printError(&sciErr, 0);
    		    return 0;
    		}
		//Check that the first input argument is a real matrix (and not complex)
    		if ( !isDoubleType(pvApiCtx, piAddr1) ||  isVarComplex(pvApiCtx, piAddr1) )
    		{
        		Scierror(999, "%s: Wrong type for input argument #%d: A real matrix expected.\n", fname, 1);
        		return 0;
    		}

    		//get matrix of lines
    		sciErr = getMatrixOfDouble(pvApiCtx, piAddr1, &m1, &n1, &lines);
    		if (sciErr.iErr)
    		{
        		printError(&sciErr, 0);
        		return 0;
    		}
		
		if(n1!=3)
		{		
			Scierror(999, "%s: Wrong size for input arguments: Same size expected.\n", fname, 1);
        		return 0;	
		}

		//input the second argument
		sciErr = getVarAddressFromPosition(pvApiCtx,2, &piAddr2);
		if (sciErr.iErr)
    		{
    		    printError(&sciErr, 0);
    		    return 0;
    		}
		
		//get matrix of size
    		sciErr = getMatrixOfDouble(pvApiCtx, piAddr2, &m2, &n2, &size);
    		if (sciErr.iErr)
    		{
        		printError(&sciErr, 0);
        		return 0;
    		}
		
		if((n2<2) || (m2!=1))
		{
			Scierror(999, "%s: Wrong size for input arguments: Same size expected.\n", fname, 2);
        		return 0;	
		}
		
		//declaring border of the image
		firstRow = 0.5;
		firstCol = 0.5;
		lastRow = size[0]+0.5;
		lastCol = size[1]+0.5;

		//declaring points matrix
		points = (double *)malloc(sizeof(double)*m1*4);
		
		for(i=0; i<m1; i++)
		{
			count = 0;
			a = lines[i];
			b = lines[i+m1];
			c = lines[i+2*m1];
			//checking for intersections with top image border
			col = -(a*firstRow +c)/b;
			if((col>=firstCol) && (col<=lastCol))
			{
				points[i+m1*count] = firstRow;
				points[i+m1*(count+1)] = col;
				count += 2;
			}

			//checking for intersections with the left image border
			row = -(b*firstCol +c)/a;
			if((row>=firstRow) && (row<=lastRow))
			{
				points[i+m1*count] = row;
				points[i+m1*(count+1)] = firstCol;
				count += 2;
			}

			//checking for intersections with bottom image border if we have not found two intersection points
			if(count<4)
			{
				col = -(a*lastRow +c)/b;
				if((col>=firstCol) && (col<=lastCol))
				{
					points[i+m1*count] = lastRow;
					points[i+m1*(count+1)] = col;
					count += 2;
				}
			}
			
			//checking for intersections with the right image border if we have not found two intersection points
			if(count<4)
			{
				row = -(b*lastCol +c)/a;
				if((row>=firstRow) && (row<=lastRow))
				{
					points[i+m1*count] = row;
					points[i+m1*(count+1)] = lastCol;
					count += 2;
				}
			}
			
			
			//if the line does not intersect with the image border set the points of intersection to -1
			if(count<4)
			{
				for(j=0; j<4; j++)
				{
					points[i+m1*j] = -1;
				}
			}
		}

		//creating matrix of points
		sciErr = createMatrixOfDouble(pvApiCtx, nbInputArgument(pvApiCtx) + 1, m1, 4, points);
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
	
		
		
