// PcExtension.cpp : Defines the exported functions for the DLL application.
//

#include <stdlib.h>
#include <stdio.h>
#include "PcExtension.h"
#include "PcPipeline.h"

extern "C"
{
	PcPipleline pp;

	FREObject Pc_start(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[])
	{
		pp.Init((PcPipleline::Mode)(PcPipleline::FACE_LOCATION|PcPipleline::GESTURE),ctx);
		return NULL;
	}

	FREObject Pc_stop(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[])
	{	
		pp.Close();

		return NULL;
	}

	// Utility functions

	FREObject createFREObjectForVector3(double x, double y, double z) 
	{
		FREObject obj;
		FREObject freX, freY, freZ;
		FRENewObjectFromDouble(x, &freX);
		FRENewObjectFromDouble(y, &freY);
		FRENewObjectFromDouble(z, &freZ);
		FREObject params[] = {freX, freY, freZ};
		FRENewObject( (const uint8_t*) "flash.geom.Vector3D", 3, params, &obj, NULL);
		return obj;
	}

	FREObject createFREObjectForUnsignedInt(unsigned int i)
	{
		FREObject fre;
		FRENewObjectFromUint32(i, &fre);
		return fre;
	}

	FREObject Pc_getLeftHand(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[])
	{
		PXCGesture::GeoNode node = pp.GetLeftHand();

		//Construct FREObject

		FREObject freGeoNode;
		FRENewObject((const uint8_t*)"com.as3nui.data.GeoNode", 0, NULL, &freGeoNode, NULL);

		FRESetObjectProperty(freGeoNode, (const uint8_t*) "confidence",    createFREObjectForUnsignedInt(node.confidence), NULL);
		FRESetObjectProperty(freGeoNode, (const uint8_t*) "positionImage", createFREObjectForVector3(node.positionImage.x, node.positionImage.y, node.positionImage.z), NULL);
		FRESetObjectProperty(freGeoNode, (const uint8_t*) "positionWorld", createFREObjectForVector3(node.positionWorld.x, node.positionWorld.y, node.positionWorld.z), NULL);

		return freGeoNode;
	}

	FREObject Pc_getRightHand(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[])
	{
		PXCGesture::GeoNode node = pp.GetRightHand();

		//Construct FREObject

		FREObject freGeoNode;
		FRENewObject((const uint8_t*)"com.as3nui.data.GeoNode", 0, NULL, &freGeoNode, NULL);

		FRESetObjectProperty(freGeoNode, (const uint8_t*) "confidence",    createFREObjectForUnsignedInt(node.confidence), NULL);
		FRESetObjectProperty(freGeoNode, (const uint8_t*) "positionImage", createFREObjectForVector3(node.positionImage.x, node.positionImage.y, node.positionImage.z), NULL);
		FRESetObjectProperty(freGeoNode, (const uint8_t*) "positionWorld", createFREObjectForVector3(node.positionWorld.x, node.positionWorld.y, node.positionWorld.z), NULL);

		return freGeoNode;
	}

	FREObject Pc_getHead(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[])
	{
		PXCPoint3DF32 point = pp.GetHead();
		FREObject freFaceData = createFREObjectForVector3(point.x,point.y,point.z);

		return freFaceData;
	}

	/*-----------------------------------------
     Start Native Extension Accessibility
     ------------------------------------------*/
    
	FRENamedFunction _Instance_methods[] = {
		{ (const uint8_t*) "start", 0, Pc_start },
		{ (const uint8_t*) "stop", 0, Pc_stop },
		{ (const uint8_t*) "getLeftHand", 0, Pc_getLeftHand },
		{ (const uint8_t*) "getRightHand", 0, Pc_getRightHand },
		{ (const uint8_t*) "getHead", 0, Pc_getHead },
	};

	void contextInitializer(void* extData, const uint8_t* ctxType, FREContext ctx, uint32_t* numFunctions, const FRENamedFunction** functions)
	{
		*numFunctions = sizeof( _Instance_methods ) / sizeof( FRENamedFunction );
		*functions = _Instance_methods;
	}

	void contextFinalizer(FREContext ctx)
	{
		return;
	}

	void AirPcInitializer(void** extData, FREContextInitializer* ctxInitializer, FREContextFinalizer* ctxFinalizer)
	{
		*ctxInitializer = &contextInitializer;
		*ctxFinalizer = &contextFinalizer;
	}

	void AirPcFinalizer(void* extData)
	{
		return;
	}
}