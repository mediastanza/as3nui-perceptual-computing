#include "util_pipeline.h"
#include "boost/thread.hpp"

class PcPipleline:public UtilPipeline
{
public:
	enum Mode {
		GESTURE       		=	0x00000002,
		FACE_LOCATION 		=	0x00000004,
		FACE_LANDMARK 		=	0x00000008,
		COLOR_VGA			=	0x00000001,
		COLOR_WXGA			=	0x00000010,
		DEPTH_QVGA			=	0x00000020,
		VOICE_RECOGNITION	=	0x00000040,
	};

	PcPipleline();

	virtual bool PcPipleline::OnNewFrame(void);
	virtual void  PXCAPI OnGesture(PXCGesture::Gesture *data);

	bool Init(Mode mode);
	void Close();
	bool GetGeoNode(PXCGesture::GeoNode::Label body, PXCGesture::GeoNode* data);
	pxcU32 GetFaceData();
	void GetHandData();

private:
	
	boost::thread _thread;

	static void* dataThread(void* ptr);
};