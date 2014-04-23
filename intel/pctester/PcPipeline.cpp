#include "PcPipeline.h"

PcPipleline::PcPipleline()
{

}

bool PcPipleline::OnNewFrame(void)
{
	GetFaceData();
	//GetHandData();

	
	return true;
}

void PcPipleline::GetHandData()
{
	// Get head pos from sample "gesture viewer simple"
	PXCGesture* detector = QueryGesture();

	if (detector)
	{
		// Query user
		pxcUID user=-1;
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
				float x=data.positionImage.x;
				float y=data.positionImage.y;

				printf("x: %f  y: %f \n", x, y);
			}
		}
	}
}

void PcPipleline::OnGesture(PXCGesture::Gesture *data)
{
	if ((data->label&PXCGesture::Gesture::LABEL_MASK_SET) < PXCGesture::Gesture::LABEL_SET_CUSTOMIZED)
	{
		switch(data->label)
		{
		case PXCGesture::Gesture::LABEL_HAND_WAVE:
			printf("wave \n");
			break;
		case PXCGesture::Gesture::LABEL_HAND_CIRCLE:
			printf("circle \n");
			break;
		case PXCGesture::Gesture::LABEL_NAV_SWIPE_LEFT:
			printf("swipe left \n");
			break;
		case PXCGesture::Gesture::LABEL_NAV_SWIPE_RIGHT:
			printf("swipe right \n");
			break;
		case PXCGesture::Gesture::LABEL_NAV_SWIPE_UP:
			printf("swipe up \n");
			break;
		case PXCGesture::Gesture::LABEL_NAV_SWIPE_DOWN:
			printf("swipe down \n");
			break;
		}
	}
	
}

bool PcPipleline::Init(Mode mode)
{
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

bool PcPipleline::GetGeoNode(PXCGesture::GeoNode::Label body, PXCGesture::GeoNode* data)
{
	if (IsDisconnected()) return false;
	if (!IsImageFrame()) return false;
	
	PXCGesture* gesture = QueryGesture();
	if (!gesture) return false;

	return gesture->QueryNodeData(0,body,data)>= PXC_STATUS_NO_ERROR;
}

uint32_t PcPipleline::GetFaceData()
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
				printf("%d %d\n", faceData.rectangle.x + faceData.rectangle.w / 2, faceData.rectangle.y + faceData.rectangle.h / 2);
				//printf("x: %d y: %d w: %d h: %d\n", faceData.rectangle.x, faceData.rectangle.y, faceData.rectangle.w, faceData.rectangle.h);
			}
		}
	}

	return 0;


	//bool faceDetect = false;

	//// PC-SDK defined data structs
	//PXCFaceAnalysis::Detection::Data        faceData;
	////PXCFaceAnalysis::Landmark::LandmarkData lmData;

	//// Get face, landmark interfaces
	//PXCFaceAnalysis *faceAnalyzer = QueryFace();
	//PXCFaceAnalysis::Detection *pDetectIfc = faceAnalyzer->DynamicCast<PXCFaceAnalysis::Detection>();
	////PXCFaceAnalysis::Landmark  *pLandmarkIfc = faceAnalyzer->DynamicCast<PXCFaceAnalysis::Landmark>();

	//// clear feedback window, if enabled
	////if (m_pFaceRender) m_pFaceRender->ClearData();

	//// Request the first face in the frame (TODO: track a 'primary' face, if multiple are found)
	//pxcUID faceID = 0;
	//pxcStatus sts = faceAnalyzer->QueryFace(0, &faceID);   // get first face index (increments over time)

	//if (PXC_STATUS_NO_ERROR == sts) // Found a face
	//{
	//	// Query face data
	//	faceDetect = true;
	//	pxcStatus retCode = pDetectIfc->QueryData(faceID, &faceData);
	//	if (PXC_STATUS_NO_ERROR == retCode)
	//	{
	//		printf("x: %d y: %d w: %d h: %d\n", faceData.rectangle.x, faceData.rectangle.y, faceData.rectangle.w, faceData.rectangle.h);
	//	}
	//}
	//else 
	//{
	//	//DBGOUT("No face data - capture time %3.1f ms\n", captureTime * 1000.0f);
	//}

	//return true;

	///* face */
	//PXCFaceAnalysis *faceAnalyzer = QueryFace();
	//PXCFaceAnalysis::Landmark *landmark = faceAnalyzer->DynamicCast<PXCFaceAnalysis::Landmark>();

	//pxcUID fid = 0;
	//pxcU64 timeStamp = 0;
	//pxcStatus sts = faceAnalyzer->QueryFace(0, &fid, &timeStamp);

	////testing the QueryLandmarkData(pxcUID fid, Label landmark, LandmarkData *data) 
	//PXCFaceAnalysis::Landmark::ProfileInfo lInfo={0};
	//landmark->QueryProfile(&lInfo);

	//PXCFaceAnalysis::Landmark::LandmarkData data[7];
	//landmark->QueryLandmarkData(fid,lInfo.labels,&data[0]);

	//printf("fd: %f \n", data[0].position.x);


	//return (uint32_t)data[0].position.x;
}