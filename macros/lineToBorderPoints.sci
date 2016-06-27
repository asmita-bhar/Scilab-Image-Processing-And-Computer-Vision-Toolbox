function [points] = lineToBorderPoints(lines,isize)
// Points of intersection of line(s) with the image border
//
// Calling Sequence 
// points = lineToBorderPoints(lines,isize)
//	
// Parameters 
// lines : It is a MX3 matrix. If a line is represented by A*x + B*y + C = 0, then each row is of the form [A B C] and M is the number of lines.
// isize : It is the size of the image. It is of the form as that returned by the function size.
// points : It is a MX4 matrix. It returns the points of intersection of the line with the image border. Each row is of the form [x1,y1,x2,y2] where (x1,y1) and (x2,y2) are the two points of intersection. If a given line does not intersect the image border, the function returns [-1 -1 -1 -1].
//
// Description 
// The function calculates the points of intersection of one or more lines with the image border. 
//
// Examples  
// // Load an image
// I = imread('rice.png');
// // Define a line : 2*x + y = 300
// line = [2 1 -300];
// // Calculate the points of intersection
// points = lineToBorderPoints(line,size(I(1)))
//
// Authors
// Asmita Bhar
//

	points = opencv_lineToBorderPoints(lines,isize)
endfunction 
