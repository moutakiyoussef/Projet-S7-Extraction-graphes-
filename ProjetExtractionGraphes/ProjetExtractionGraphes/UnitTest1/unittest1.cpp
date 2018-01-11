#include "stdafx.h"
#include "CppUnitTest.h"
#include "../Slic.h";
#include "../Rag.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest1
{		
	TEST_CLASS(UnitTest1)
	{
	public:
		
		TEST_METHOD(Calculer)
		{
			Slic slic;
			Assert::AreEqual(2, slic.calculer(1, 1));
		}

		TEST_METHOD(existVertex)
		{
		}
		TEST_METHOD(addVertex)
		{
		}
		TEST_METHOD(addEdge)
		{
		}
		TEST_METHOD(listVertex)
		{
		}

		TEST_METHOD(find_mean_rgb)
		{
		}
		TEST_METHOD(compute_distance)
		{
		}
		TEST_METHOD(find_local_minimum)
		{
		}
		TEST_METHOD(clear_data)
		{
		}
		TEST_METHOD(initialize_data)
		{
		}
		TEST_METHOD(generate_superpixels)
		{
		}
		TEST_METHOD(create_connectivity)
		{
		}
		TEST_METHOD(display_contours)
		{
		}
		TEST_METHOD(display_center_grid)
		{
		}
		


	};
}