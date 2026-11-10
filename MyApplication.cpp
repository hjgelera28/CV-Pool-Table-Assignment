
#include "Utilities.h"
#include <array>

#define TOP_LEFT	0
#define TOP_RIGHT	1
#define BOT_LEFT	2
#define BOT_RIGHT	3

void MyApplication()
{
	// Image file locations
	char* file_location = "Media/Pool/";
	char* pool_files[] = {
		"AllBalls.jpeg",
		"Table1.jpeg",
		"Table2.jpeg",
		"Table3.jpeg",
		"Table4.jpeg",
		"Table5.jpeg",
		"Table6.jpeg"
	};

	// Pool table coordinates
	Point2f pool_corners[4];
	pool_corners[TOP_LEFT] = Point2f(337, 35);
	pool_corners[BOT_LEFT] = Point2f(471, 38);
	pool_corners[TOP_RIGHT] = Point2f(1570, 21);
	pool_corners[BOT_RIGHT] = Point2f(1888, 451);

	// Define pool centre
	Point2f pool_centre = (
		(pool_corners[0].x + pool_corners[1].x + pool_corners[2].x + pool_corners[3].x) / 4,
		(pool_corners[0].y + pool_corners[1].y + pool_corners[2].y + pool_corners[3].y) / 4
		);

	// Load and show images
	const int number_of_images = sizeof(pool_files) / sizeof(pool_files[0]);
	Mat* pool_image = new Mat[number_of_images];
	Mat* new_pool_image = new Mat[number_of_images];

	for (int file_no = 0; (file_no < number_of_images); file_no++)
	{
		cout << pool_files[file_no] << endl;
		string filename(file_location);
		filename.append(pool_files[file_no]);
		pool_image[file_no] = imread(filename, -1);

		if (pool_image[file_no].empty())
		{
			cout << "Could not open " << pool_image[file_no] << endl;
		}
		imshow(pool_files[file_no], pool_image[file_no]);

		char pause = cv::waitKey();  // This introduces a delay between frame.  The length of the delay/number can be reduced.
	}

	// Rotate / Affine Transformation
	double angle = -0.65;
	double scale = 1;
	for (int file_no = 0; (file_no < number_of_images); file_no++)
	{
		Mat rotation_matrix = getRotationMatrix2D(pool_centre, angle, scale);
		cout << "rotating " << pool_files[file_no] << "..." << endl;
		warpAffine(pool_image[file_no], new_pool_image[file_no], rotation_matrix, new_pool_image[file_no].size());
	}

	// Perspective Transformation

	
	// Show Transformed Images
	for (int file_no = 0; (file_no < number_of_images); file_no++)
	{
		if (new_pool_image[file_no].empty())
		{
			cout << "Could not open " << new_pool_image[file_no] << endl;
		}
		imshow(pool_files[file_no], new_pool_image[file_no]);

		char pause = cv::waitKey();  // This introduces a delay between frame.  The length of the delay/number can be reduced.
	}
}


