//**************************************************
// Author : Asmita Bhar
//**************************************************

function [out] = cvMedian(image,varargin)
// Finds median values in an input
//
// Calling Sequence
// val = cvMedian(image);
// val = cvMedian(image, name, value, .....);
//
// Parameters
// image : Input image matrix 
// SortMethod (Optional) : The sort method to calculate the median - Quick sort or Insertion Sort. Default : Quick sort
// Dimension (Optional) : Dimension along which the function operates - Row, Column, All or Custom. Default : All
// CustomDimension (Optional) : The integer dimension over which the function calculates the median. This value cannot exceed the number of dimensions in input. It applies only when 'Dimension' property is set to 'Custom'. Default : 1
// val : It contains the median values.
//
// Description 
// The function calculates the median values in a given input image matrix. 
// 
// Examples
// //Load an image
// I = imread('peppers.png');
// val1 = cvMedian(I); // calculates the median value considering dimension as 'All'
// val2 = cvMedian(I,'Dimension','Row'); //calculates the median value in 'Row' dimension
// 
// Authors
// Asmita Bhar
//
	image = mattolist(image);
	[lhs,rhs] = argn(0);
	if rhs<1 then
		error(msprintf("Not enough input arguments"));
	end
	if rhs>7 then
		error(msprintf("Too many input arguments"));
	end
	
	[iRows iCols]=size(image(1))
	iChannels = size(image)
	
	sortMethod = 'Quick sort';
	dimension = 'All';
	customDimension = 1;

	flag=0;
	i=1;
	while(i<rhs-1)
		if strcmpi(varargin(i),'SortMethod')==0 then
			sortMethod = varargin(i+1);
			if strcmpi(sortMethod,"Quick sort") & strcmpi(sortMethod,"Insertion sort") then
                		error(msprintf(" wrong input argument #%d,SortMethod not matched",i))
            		end
		elseif strcmpi(varargin(i),'Dimension')==0 then
			dimension = varargin(i+1)
			if strcmpi(dimension,"Column") & strcmpi(dimension,"Row") &strcmpi(dimension,"All") & strcmpi(dimension,"Custom") then
                		error(msprintf(" wrong input argument #%d, Dimension not matched",i))
            		end
		elseif strcmpi(varargin(i),'CustomDimension')==0 then
			customDimension = varargin(i+1)
			flag=1;
		end
		i=i+2;
	end
	
	if (strcmpi(dimension,'Custom') & (flag==1))
		error(msprintf("The CustomDimension property is not relevant in this configuration"));
	end

	if(iChannels==1) then
		if(dimension=='All') then
			out = median(image(1));
		elseif(dimension=='Row') then
			out = median(image(1),'c');
		elseif(dimension=='Column') then
			out = median(image(1),'r');
		elseif(dimension=='Custom') then
			if(customDimension==1) then
				out = median(image(1),'r');
			elseif(customDimension==2) then
				out = median(image(1),'c');
			end
		end
	elseif(iChannels==3)
		if(dimension=='All') then
			out = median([image(1) image(2) image(3)]); 
		elseif(dimension=='Row') then
			out(:,:,1) = median(image(1),'c');
			out(:,:,2) = median(image(2),'c');
			out(:,:,3) = median(image(3),'c');
		elseif(dimension=='Column') then
			out(:,:,1) = median(image(1),'r');
			out(:,:,2) = median(image(2),'r');
			out(:,:,3) = median(image(3),'r');
		elseif(dimension=='Custom') then
			if(customDimension==1) then
				out(:,:,1) = median(image(1),'r');
				out(:,:,2) = median(image(2),'r');
				out(:,:,3) = median(image(3),'r');
			elseif(customDimension==2) then
				out(:,:,1) = median(image(1),'c');
				out(:,:,2) = median(image(2),'c');
				out(:,:,3) = median(image(3),'c');
			elseif(customDimension==3) then
				a = median(image(1),3);
				b = median(image(2),3);
				c = median(image(3),3);
				for i=1:iRows
					for j=1:iCols
						out(i,j) = median([a(i,j) b(i,j) c(i,j)]);	
					end

				end
			end
		end
	end

endfunction
	
