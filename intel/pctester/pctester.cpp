// pctester.cpp : Defines the entry point for the console application.
//

#include "PcPipeline.h"


int main()
{
	PcPipleline pp;
	pp.Init((PcPipleline::Mode)(PcPipleline::GESTURE|PcPipleline::FACE_LOCATION));

	getchar();

	return 0;
}

