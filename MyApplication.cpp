
#include "Utilities.h"
#include <array>

#define TOP_LEFT	0
#define BOT_LEFT	1
#define TOP_RIGHT	2
#define BOT_RIGHT	3

void show_image_array(Mat* image_array, char** image_files, int number_of_images);
void printBallLocations(vector<Vec3f>* circles, char** image_files, int number_of_images);
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

	// Perspective Transformation
	// ==================================================
	Point2f p_transform_points[4];
	p_transform_points[BOT_LEFT] = Point2f(0, 500);
	p_transform_points[TOP_LEFT] = Point2f(0, 0);
	p_transform_points[BOT_RIGHT] = Point2f(1500, 500);
	p_transform_points[TOP_RIGHT] = Point2f(1500, 0);

	Mat pshift_pool_image[number_of_images];
	Mat perspective_matrix = getPerspectiveTransform(pool_corners, p_transform_points);
	
	for (int file_no = 0; (file_no < number_of_images); file_no++)
	{
		cout << "Perpective Tranforming" << pool_files[file_no] << "..." << endl;
		warpPerspective(pool_image[file_no], pshift_pool_image[file_no], perspective_matrix, pool_image[file_no].size());
	}

	// Cropping images
	// ==================================================
	int new_width = 1500;
	int new_height = 500;

	for (int file_no = 0; (file_no < number_of_images); file_no++)
	{
		pshift_pool_image[file_no] = pshift_pool_image[file_no](Rect(0, 0, new_width, new_height));
	}

	// Show Transformed Images
	show_image_array(pshift_pool_image, pool_files, number_of_images);

	// Convert image to grayscale
	// ==================================================
	Mat* gray_pool_image = new Mat[number_of_images];

	for (int file_no = 0; (file_no < number_of_images); file_no++)
	{
		cvtColor(pshift_pool_image[file_no], gray_pool_image[file_no], COLOR_BGR2GRAY);
	}
	show_image_array(gray_pool_image, pool_files, number_of_images);

	/* Removed due to redundancy from hough
	// Canny Edge Detection
	// ==================================================
	Mat* canny_pool_image = new Mat[number_of_images];
	for (int file_no = 0; (file_no < number_of_images); file_no++)
	{
		Canny(gray_pool_image[file_no], canny_pool_image[file_no], 150, 250);
	}
	show_image_array(canny_pool_image, pool_files, number_of_images);
	*/ 

	// Hough Circle Detection
	// ==================================================
	vector<Vec3f>* circles = new vector<Vec3f>[number_of_images];

	for (int file_no = 0; (file_no < number_of_images); file_no++)
	{
		medianBlur(gray_pool_image[file_no], gray_pool_image[file_no], 5);
	}

	for (int file_no = 0; (file_no < number_of_images); file_no++)
	{
		HoughCircles(gray_pool_image[file_no], circles[file_no], HOUGH_GRADIENT, 1.75, 30, 120, 20, 20, 30);
	}

	for (int file_no = 0; (file_no < number_of_images); file_no++)
	{
		for (size_t i = 0; i < circles[file_no].size(); i++)
		{
			Vec3i c = circles[file_no][i];
			Point center = Point(c[0], c[1]);
			int radius = c[2];
			circle(pshift_pool_image[file_no], center, radius, Scalar(0, 0, 255), 2, LINE_AA);
		}
	}

	// Final Results
	show_image_array(pshift_pool_image, pool_files, number_of_images);	
	printBallLocations(circles, pool_files, number_of_images);
}

// Function to create a window and display the images in image arrays
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

// Function to print the ball locations for each image
void printBallLocations(vector<Vec3f>* circles, char** image_files, int number_of_images)
{
	for (int file_no = 0; (file_no < number_of_images); file_no++)
	{
		cout << "Ball Locations for " << image_files[file_no] << endl;
		for (size_t i = 0; i < circles[file_no].size(); i++)
		{
			Vec3i c = circles[file_no][i];
			Point center = Point(c[0], c[1]);
			int radius = c[2];
			cout << "Ball " << i + 1 << ": (" << center.x << ", " << center.y - radius << ")" << endl;
		}
	}
}