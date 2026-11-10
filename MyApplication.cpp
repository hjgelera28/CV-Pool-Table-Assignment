
#include "Utilities.h"
#include <array>

#define TOP_LEFT	0
#define BOT_LEFT	1
#define TOP_RIGHT	2
#define BOT_RIGHT	3

void show_image_array(Mat* image_array, char** image_files, int number_of_images);
void myApplication();

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
	pool_corners[BOT_LEFT] = Point2f(38, 472);
	pool_corners[TOP_RIGHT] = Point2f(1570, 21);
	pool_corners[BOT_RIGHT] = Point2f(1888, 451);

	// Define pool centre
	Point2f pool_centre = (
		(pool_corners[0].x + pool_corners[1].x + pool_corners[2].x + pool_corners[3].x) / 4,
		(pool_corners[0].y + pool_corners[1].y + pool_corners[2].y + pool_corners[3].y) / 4
		);

	// Load images
	const int number_of_images = sizeof(pool_files) / sizeof(pool_files[0]);
	Mat* pool_image = new Mat[number_of_images];
	Mat* new_pool_image = new Mat[number_of_images];
	Mat* temp_pool_image = new Mat[number_of_images];	// Temporary images for transforms

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
	}

	show_image_array(pool_image, pool_files, number_of_images);

	// Convert image to grayscale
	// ==================================================
	Mat* gray_pool_image = new Mat[number_of_images];

	for (int file_no = 0; (file_no < number_of_images); file_no++)
	{
		cvtColor(pool_image[file_no], gray_pool_image[file_no], COLOR_BGR2GRAY);
	}

	show_image_array(gray_pool_image, pool_files, number_of_images);

	// Canny Edge Detection
	// ==================================================
	Mat* canny_pool_image = new Mat[number_of_images];
	for (int file_no = 0; (file_no < number_of_images); file_no++)
	{
		Canny(gray_pool_image[file_no], canny_pool_image[file_no], 75, 150);
	}

	show_image_array(canny_pool_image, pool_files, number_of_images);

	// Perspective Transformation
	// ==================================================
	Point2f p_transform_points[4];
	p_transform_points[BOT_LEFT] = Point2f(0, 500);
	p_transform_points[TOP_LEFT] = Point2f(0, 0);
	p_transform_points[BOT_RIGHT] = Point2f(1000, 500);
	p_transform_points[TOP_RIGHT] = Point2f(1000, 0);

	Mat perspective_matrix = getPerspectiveTransform(pool_corners, p_transform_points);
	
	for (int file_no = 0; (file_no < number_of_images); file_no++)
	{
		cout << "Perpective Tranforming" << pool_files[file_no] << "..." << endl;
		warpPerspective(pool_image[file_no], temp_pool_image[file_no], perspective_matrix, pool_image[file_no].size());
	}

	memcpy(new_pool_image, temp_pool_image, sizeof(Mat) * number_of_images);

	// Cropping images
	// ==================================================
	int new_width = 1000;
	int new_height = 500;

	for (int file_no = 0; (file_no < number_of_images); file_no++)
	{
		temp_pool_image[file_no] = new_pool_image[file_no](Rect(0, 0, new_width, new_height));
	}

	memcpy(new_pool_image, temp_pool_image, sizeof(Mat) * number_of_images);

	// Show Transformed Images
	show_image_array(new_pool_image, pool_files, number_of_images);
}

void show_image_array(Mat* image_array, char** image_files, int number_of_images)
{
	for (int file_no = 0; (file_no < number_of_images); file_no++)
	{
		if (image_array[file_no].empty())
		{
			cout << "Could not open " << image_array[file_no] << endl;
		}
		imshow(image_files[file_no], image_array[file_no]);
		char pause = cv::waitKey();  // This introduces a delay between frame.  The length of the delay/number can be reduced.
	}
	destroyAllWindows();
}