#include "PcPipeline.h"

PcPipleline::PcPipleline()
{

}

bool PcPipleline::OnNewFrame(void)
{
	UpdateHeadPos();
	UpdateGeoNode();
	FREDispatchStatusEventAsync(_ctx,(const uint8_t*)"newframe",(const uint8_t*)"info");

	return true;
}

// For now, left/right hand only
bool PcPipleline::UpdateGeoNode()
{
	bool res = false;

	PXCGesture* detector = QueryGesture();

	if (detector)
	{
		// Query user
		pxcUID user=-1;

		// max 100 users to detect, and pick up the first one
		for (int u=0;u < 100; u++) {

			pxcStatus sts= detector->QueryUser(u,&user);
			if (sts>=PXC_STATUS_NO_ERROR) break;
		}

		// Display node
		if(user >= 0)
		{
			PXCGesture::GeoNode data;
			if (detector->QueryNodeData(user,PXCGesture::GeoNode::LABEL_BODY_HAND_LEFT,&data)>=PXC_STATUS_NO_ERROR)
			{
				_leftHand = data;

				// Normalize
				_leftHand.positionImage.x = 1 - _leftHand.positionImage.x / 320;
				_leftHand.positionImage.y = _leftHand.positionImage.y / 240;

				res = true;
			}

			if (detector->QueryNodeData(user,PXCGesture::GeoNode::LABEL_BODY_HAND_RIGHT,&data)>=PXC_STATUS_NO_ERROR)
			{
				_rightHand = data;

				// Normalize
				_rightHand.positionImage.x = 1 - _rightHand.positionImage.x / 320;
				_rightHand.positionImage.y = _rightHand.positionImage.y / 240;

				res = true;
			}
		}
	}

	return res;
}

void PcPipleline::OnGesture(PXCGesture::Gesture *data)
{
	if ((data->label&PXCGesture::Gesture::LABEL_MASK_SET) < PXCGesture::Gesture::LABEL_SET_CUSTOMIZED)
	{
		switch(data->label)
		{
		case PXCGesture::Gesture::LABEL_HAND_WAVE:
			FREDispatchStatusEventAsync(_ctx,(const uint8_t*)"wave",(const uint8_t*)"info");
			break;
		case PXCGesture::Gesture::LABEL_HAND_CIRCLE:
			FREDispatchStatusEventAsync(_ctx,(const uint8_t*)"circle",(const uint8_t*)"info");
			break;
		case PXCGesture::Gesture::LABEL_NAV_SWIPE_LEFT:
			FREDispatchStatusEventAsync(_ctx,(const uint8_t*)"swipeleft",(const uint8_t*)"info");
			break;
		case PXCGesture::Gesture::LABEL_NAV_SWIPE_RIGHT:
			FREDispatchStatusEventAsync(_ctx,(const uint8_t*)"swiperight",(const uint8_t*)"info");
			break;
		case PXCGesture::Gesture::LABEL_NAV_SWIPE_UP:
			FREDispatchStatusEventAsync(_ctx,(const uint8_t*)"swipeup",(const uint8_t*)"info");
			break;
		case PXCGesture::Gesture::LABEL_NAV_SWIPE_DOWN:
			FREDispatchStatusEventAsync(_ctx,(const uint8_t*)"swipedown",(const uint8_t*)"info");
			break;
		}
	}
	
}

// For now, 2D position only
bool PcPipleline::UpdateHeadPos()
{
	bool faceDetect = false;

	PXCFaceAnalysis::Detection::Data        faceData;

	PXCFaceAnalysis *faceAnalyzer = QueryFace();
	if (faceAnalyzer)
	{
		PXCFaceAnalysis::Detection *pDetectIfc = faceAnalyzer->DynamicCast<PXCFaceAnalysis::Detection>();

		// Request the first face in the frame
		pxcUID faceID = 0;
		pxcStatus sts = faceAnalyzer->QueryFace(0, &faceID);

		if (PXC_STATUS_NO_ERROR == sts) // Found a face
		{
			// Query face data
			pxcStatus retCode = pDetectIfc->QueryData(faceID, &faceData);
			if (PXC_STATUS_NO_ERROR == retCode)
			{
				faceDetect = true;
				pxcF32 x = faceData.rectangle.x + faceData.rectangle.w / 2;
				pxcF32 y = faceData.rectangle.y + faceData.rectangle.h / 2;
				_headPos.x = 1 - x / 640;
				_headPos.y = y / 480;
				_headPos.z = 0;
			}
		}
	}

	return faceDetect;
}

bool PcPipleline::Init(Mode mode,FREContext ctx)
{
	_ctx = ctx;

	// Set mode
	if (mode&COLOR_VGA)           EnableImage(PXCImage::COLOR_FORMAT_RGB32,640,480);
	if (mode&COLOR_WXGA)          EnableImage(PXCImage::COLOR_FORMAT_RGB24,1280,720);
	if (mode&DEPTH_QVGA)          EnableImage(PXCImage::COLOR_FORMAT_DEPTH,320,240);
	if (mode&GESTURE)             EnableGesture();
	if (mode&FACE_LOCATION)       EnableFaceLocation();
	if (mode&FACE_LANDMARK)       EnableFaceLandmark();
	if (mode&VOICE_RECOGNITION)   EnableVoiceRecognition();

	// Create dedicated thread for data processing
	_thread = boost::thread(&PcPipleline::dataThread,this);

	return true;
}

void PcPipleline::Close()
{
	_thread.join();
}

void* PcPipleline::dataThread(void* self)
{
	PcPipleline* adapter = (PcPipleline*) self;
	adapter->LoopFrames();

	return NULL;
}

PXCGesture::GeoNode PcPipleline::GetLeftHand()
{
	return _leftHand;
}

PXCGesture::GeoNode PcPipleline::GetRightHand()
{
	return _rightHand;
}

PXCPoint3DF32 PcPipleline::GetHead()
{
	return _headPos;
}
