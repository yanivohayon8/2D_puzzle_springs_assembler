#include <iostream>
#include <stdio.h>
#include <box2d/box2d.h>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/types_c.h>
using namespace cv;

//#include <Eigen/src/Eigenvalues/
#include <Eigen/Dense>

int EigenHelloWorld()
{
	Eigen::MatrixXd m(2, 2);
	m(0, 0) = 3;
	m(1, 0) = 2.5;
	m(0, 1) = -1;
	m(1, 1) = m(1, 0) + m(0, 1);
	std::cout << m << std::endl;

	return 0;
}


int hellowWorld()
{
	// Initialize Box2D world
	b2Vec2 gravity(0.0f, -10.0f);
	b2World world(gravity);

	// Create the ground body
	b2BodyDef groundBodyDef;
	groundBodyDef.position.Set(0.0f, -10.0f);
	b2Body* groundBody = world.CreateBody(&groundBodyDef);

	// Define the ground shape
	b2PolygonShape groundShape;
	groundShape.SetAsBox(50.0f, 10.0f);

	// Create the ground fixture
	b2FixtureDef groundFixtureDef;
	groundFixtureDef.shape = &groundShape;
	groundBody->CreateFixture(&groundFixtureDef);

	// Create the box body
	b2BodyDef boxBodyDef;
	boxBodyDef.type = b2_dynamicBody;
	boxBodyDef.position.Set(0.0f, 20.0f);
	b2Body* boxBody = world.CreateBody(&boxBodyDef);

	// Define the box shape
	b2PolygonShape boxShape;
	boxShape.SetAsBox(1.0f, 1.0f);

	// Create the box fixture
	b2FixtureDef boxFixtureDef;
	boxFixtureDef.shape = &boxShape;
	boxFixtureDef.density = 1.0f;
	boxFixtureDef.friction = 0.3f;
	boxFixtureDef.restitution = 0.5f;
	boxBody->CreateFixture(&boxFixtureDef);

	// Set up OpenCV visualization
	const float k_pixelsPerMeter = 10.0f;
	cv::Mat image(600, 800, CV_8UC3);
	cv::namedWindow("Box2D Simulation", cv::WINDOW_NORMAL);
	cv::resizeWindow("Box2D Simulation", image.cols, image.rows);

	// Step through the simulation
	float timeStep = 1.0f / 60.0f;
	int32 velocityIterations = 6;
	int32 positionIterations = 2;
	for (int32 i = 0; i < 99999999; ++i)
	{
		world.Step(timeStep, velocityIterations, positionIterations);

		// Clear the image
		image.setTo(cv::Scalar(255, 255, 255));

		// Draw the ground
		const b2Transform& groundTransform = groundBody->GetTransform();
		for (b2Fixture* f = groundBody->GetFixtureList(); f; f = f->GetNext())
		{
			b2PolygonShape* polygonShape = dynamic_cast<b2PolygonShape*>(f->GetShape());
			if (polygonShape)
			{
				b2Vec2 vertices[b2_maxPolygonVertices];
				for (int i = 0; i < polygonShape->m_count; ++i)
				{
					vertices[i] = b2Mul(groundTransform, polygonShape->m_vertices[i]);
				}
				std::vector<cv::Point> points(polygonShape->m_count);
				for (int i = 0; i < polygonShape->m_count; ++i)
				{
					points[i] = cv::Point(vertices[i].x * k_pixelsPerMeter, vertices[i].y * k_pixelsPerMeter);
				}
				cv::polylines(image, points, true, cv::Scalar(0, 0, 0), 2);
			}
		}

	}

	return 0;
}

int Box2dHelloWorld(int argc, char** argv)
{
	B2_NOT_USED(argc);
	B2_NOT_USED(argv);

	// Define the gravity vector.
	b2Vec2 gravity(0.0f, -10.0f);

	// Construct a world object, which will hold and simulate the rigid bodies.
	b2World world(gravity);

	// Define the ground body.
	b2BodyDef groundBodyDef;
	groundBodyDef.position.Set(0.0f, -10.0f);

	// Call the body factory which allocates memory for the ground body
	// from a pool and creates the ground box shape (also from a pool).
	// The body is also added to the world.
	b2Body* groundBody = world.CreateBody(&groundBodyDef);

	// Define the ground box shape.
	b2PolygonShape groundBox;

	// The extents are the half-widths of the box.
	groundBox.SetAsBox(50.0f, 10.0f);

	// Add the ground fixture to the ground body.
	groundBody->CreateFixture(&groundBox, 0.0f);

	// Define the dynamic body. We set its position and call the body factory.
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(0.0f, 4.0f);
	b2Body* body = world.CreateBody(&bodyDef);

	// Define another box shape for our dynamic body.
	b2PolygonShape dynamicBox;
	dynamicBox.SetAsBox(1.0f, 1.0f);

	// Define the dynamic body fixture.
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &dynamicBox;

	// Set the box density to be non-zero, so it will be dynamic.
	fixtureDef.density = 1.0f;

	// Override the default friction.
	fixtureDef.friction = 0.3f;

	// Add the shape to the body.
	body->CreateFixture(&fixtureDef);

	// Prepare for simulation. Typically we use a time step of 1/60 of a
	// second (60Hz) and 10 iterations. This provides a high quality simulation
	// in most game scenarios.
	float timeStep = 1.0f / 60.0f;
	int32 velocityIterations = 6;
	int32 positionIterations = 2;

	// This is our little game loop.
	for (int32 i = 0; i < 60; ++i)
	{
		// Instruct the world to perform a single step of simulation.
		// It is generally best to keep the time step and iterations fixed.
		world.Step(timeStep, velocityIterations, positionIterations);

		// Now print the position and angle of the body.
		b2Vec2 position = body->GetPosition();
		float angle = body->GetAngle();

		printf("%4.2f %4.2f %4.2f\n", position.x, position.y, angle);
	}

	// When the world destructor is called, all bodies and joints are freed. This can
	// create orphaned pointers, so be careful about your world management.

	return 0;
}

int opencvHelloWorld()
{


	////create a gui window:
	namedWindow("Output", 1);

	//initialize a 120X350 matrix of black pixels:
	Mat output = Mat::zeros(120, 350, CV_8UC3);

	//write text on the matrix:
	putText(output,
		"Hello World :)",
		cvPoint(15, 70),
		FONT_HERSHEY_PLAIN,
		3,
		cvScalar(0, 255, 0),
		4);

	//display the image:
	imshow("Output", output);

	//wait for the user to press any key:
	cv::waitKey(0);

	return 0;
}

int mainHelloWorld(int argc, char** argv)
{
	std::cout << "Hello World" << std::endl;
	Box2dHelloWorld(argc, argv);
	opencvHelloWorld();
	EigenHelloWorld();
	return 0;
}