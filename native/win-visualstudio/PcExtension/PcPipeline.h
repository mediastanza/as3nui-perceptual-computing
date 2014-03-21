#include "util_pipeline.h"
#include "boost/thread.hpp"
#include "FlashRuntimeExtensions.h"

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

	bool Init(Mode mode,FREContext ctx);
	void Close();
	PXCGesture::GeoNode GetLeftHand();
	PXCGesture::GeoNode GetRightHand();
	PXCPoint3DF32 GetHead();

private:
	FREContext _ctx;
	boost::thread _thread;
	PXCGesture::GeoNode _leftHand;
	PXCGesture::GeoNode _rightHand;
	PXCPoint3DF32 _headPos;

	static void* dataThread(void* ptr);
	bool UpdateGeoNode();
	bool UpdateHeadPos();
};