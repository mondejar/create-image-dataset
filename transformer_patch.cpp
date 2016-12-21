#include "transformer_patch.h"
 

void Transformer_patch::setInputPatch(cv::Mat m)
{
	_patch = m.clone();
	_sizePatch = _patch.size();
}


void Transformer_patch::blur()
{
	float w_value, h_value;
	float randValue;

	randValue =  ((double) rand() / (RAND_MAX));
	w_value = ( (_sizePatch.width * 0.2) * randValue) + 1;
	randValue =  ((double) rand() / (RAND_MAX));
	h_value = ( (_sizePatch.width * 0.2) * randValue) + 1;

   	cv::Size kernel;
   	kernel = cv::Size((int)w_value, (int)h_value);  
	cv::blur(_patch, _patch, kernel);
}

void Transformer_patch::dynamic_range_compression()
{
	float a,b;
	float randValue;

	randValue =  ((double) rand() / (RAND_MAX));
	a = (0.6 * randValue) + 0.4; //[0.4, 1]

	randValue =  ((double) rand() / (RAND_MAX));
	b = 25.0 * randValue;//[0,100]
	int value;
	for(int r = 0; r < _patch.rows; r++)
	{
		for(int c = 0; c < _patch.cols; c++)
		{
				value = (int)_patch.at<uchar>(r,c);
				_patch.at<uchar>(r,c) = (int)(std::min((float)255, a * (float)value + b));
		}
	}
}

void Transformer_patch::non_uniform_light()
{
	float center_x, center_y;//Source of the light
	float randValue;
	//The center is in the interval
	//[-width,0] [width, 2 *width]
	//[-height,0] [height, 2*height]

	randValue =  ((double) rand() / (RAND_MAX));
	center_x = _sizePatch.width * 2 * randValue;
	randValue =  ((double) rand() / (RAND_MAX));
	center_y = _sizePatch.width * 2 * randValue;

	if(center_x <  _sizePatch.width)
		center_x = -center_x;
	if(center_y <  _sizePatch.width)
		center_y = -center_y;


	float add_light;
	float dist;
	float sigma;

	randValue =  ((double) rand() / (RAND_MAX));
	sigma = ((_sizePatch.width/2) - (_sizePatch.width/4) * randValue) + (_sizePatch.width/4) ;
	//std::cout<<"Center ["<< center_x << ", "<< center_y << "] sigma = "<< sigma << std::endl;

	for(int r = 0; r < _patch.rows; r++)
	{
		for(int c = 0; c < _patch.cols; c++)
		{
			dist = pow(c - center_x, 2) + pow(r - center_y, 2);
			add_light = 50 * exp( (-1/(2*pow(sigma,2))) * dist);
			_patch.at<uchar>(r,c) = std::min(255, _patch.at<uchar>(r,c) + (int)add_light);
		}
	}
}

void Transformer_patch::affine_transformation()
{
	cv::Point2f corners[4], corners_t[4];
	float w = 0.025 * _sizePatch.width;
	float h = 0.025 * _sizePatch.height;
	float x,y;
	float randValue;

	corners[0] = cv::Point2f(w, h);
	corners[1] = cv::Point2f(w, _sizePatch.height - h);
	corners[2] = cv::Point2f(_sizePatch.width - w, h);
	corners[3] = cv::Point2f(_sizePatch.width - w, _sizePatch.height - h);


	for(int p = 0; p < 4; p++)
	{			
		randValue =  ((double) rand() / (RAND_MAX));		
	    x = ((w*2 * randValue) -w) + corners[p].x;

		randValue =  ((double) rand() / (RAND_MAX));		
	    y =  ((h*2 * randValue) -h) + corners[p].y;
	    corners_t[p] = cv::Point2f(x, y); 
	}   
	     		
	cv::Mat A = cv::getAffineTransform(corners, corners_t);
	cv::warpAffine( _patch, _patch, A , _patch.size()); 	
}


void Transformer_patch::dilate()
{
	float randValue;
	int dilation_type;
	dilation_type = cv::MORPH_CROSS; 
	//dilation_type = MORPH_RECT; 
	//dilation_type = cv::MORPH_ELLIPSE; 

	int dilation_size;	
	randValue =  ((double) rand() / (RAND_MAX));
	dilation_size = 0.04 * _sizePatch.width * randValue;

	cv::Mat element = cv::getStructuringElement( dilation_type,
                                       cv::Size( 2*dilation_size+1, 2*dilation_size +1),
                                       cv::Point( dilation_size, dilation_size ) );
	cv::dilate(_patch, _patch, element);
}

cv::Mat Transformer_patch::getPatch()
{
	return _patch;
}