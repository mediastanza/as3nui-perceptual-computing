package
{
	import com.as3nui.RealSense;
	import com.as3nui.data.GeoNode;
	
	import flash.display.Sprite;
	import flash.events.Event;
	import flash.events.MouseEvent;
	import flash.geom.Vector3D;
	import flash.text.TextField;
	import flash.text.TextFormat;
	
	[SWF(width="1024", height="768", backgroundColor="#ffffff", frameRate="30")]
	public class tester extends Sprite
	{
		private var device:RealSense = new RealSense();
		
		// Visual stuff
		private var head:Sprite = new Sprite();
		private var primeHand:Sprite = new Sprite();
		private var secondHand:Sprite = new Sprite();
		
		private var headPos:TextField = new TextField();
		private var primeHandPos:TextField = new TextField();
		private var secondHandPos:TextField = new TextField();
		
		private var gestureInfo:TextField = new TextField();
		
		private var format:TextFormat = new TextFormat();
		
		// Stage Width = 500; Stage Height = 375
		
		public function tester()
		{
			device.Start();
			
			device.addEventListener("newframe",onNewFrame);
			device.addEventListener("wave",onWave);
			device.addEventListener("circle",onCircle);
			device.addEventListener("swipeleft",onSwipeLeft);
			device.addEventListener("swiperight",onSwipeRight);
			device.addEventListener("swipeup",onSwipeUp);
			device.addEventListener("swipedown",onSwipeDown);
			
			stage.addEventListener(MouseEvent.CLICK, onClick);
			stage.addEventListener(Event.CLOSING, onClose);
			
			// Create visual
			
			format.size = 26;
			
			createHeadVisual();
			createPrimeHandVisual();
			createSecondHandVisual();
			
			gestureInfo.defaultTextFormat = format;
			gestureInfo.text = "gesture";
			
			gestureInfo.x = stage.stageWidth / 2;
			addChild(gestureInfo);

			//stage.nativeWindow.maximize();
		}
		
		private function onNewFrame(event:Event)
		{
			// Update hand position
			var leftHand:GeoNode = device.GetLeftHand();
			updatePrimeHandPos(leftHand.positionImage.x * stage.nativeWindow.width, leftHand.positionImage.y * stage.nativeWindow.height);
			
			var rightHand:GeoNode = device.GetRightHand();
			updateSecondHandPos(rightHand.positionImage.x * stage.nativeWindow.width, rightHand.positionImage.y * stage.nativeWindow.height);
			
			
			// Update head position
			var headVect:Vector3D = device.GetHead();
			updateHeadPos(headVect.x * stage.nativeWindow.width, headVect.y * stage.nativeWindow.height);
		}
		
		private function onClick(e:MouseEvent):void
		{
			trace("I was clicked!");
			
			//updatePrimeHandPos(e.stageX, e.stageY);
			//updateSecondHandPos(e.stageX, e.stageY);
			//updateHeadPos(e.stageX, e.stageY);
		}
		
		private function onClose(e:Event):void
		{
			trace("Closing...");
			device.Stop();
		}
		
		private function createHeadVisual():void
		{
			// Initial pos
			head.x = stage.stageWidth / 2;
			head.y = stage.stageHeight / 3;
			
			// Draw a dot
			head.graphics.lineStyle(10,0xff0000);
			head.graphics.beginFill(0xff0000);
			head.graphics.moveTo(0,0);
			head.graphics.lineTo(10,0);
			head.graphics.lineTo(10,10);
			head.graphics.lineTo(0,10);
			head.graphics.endFill();
			addChild(head);
			
			// Show default info
			headPos.defaultTextFormat = format;
			headPos.text = "Head";
			headPos.x = head.x;
			headPos.y = head.y + 20;
			
			addChild(headPos);
		}
		
		private function createPrimeHandVisual():void
		{
			// Initial pos
			primeHand.x = stage.stageWidth / 3;
			primeHand.y = stage.stageHeight / 2;
			// Draw a dot
			primeHand.graphics.lineStyle(10,0x00ff00);
			primeHand.graphics.beginFill(0x00ff00);
			primeHand.graphics.moveTo(0,0);
			primeHand.graphics.lineTo(10,0);
			primeHand.graphics.lineTo(10,10);
			primeHand.graphics.lineTo(0,10);
			primeHand.graphics.endFill();
			addChild(primeHand);
			// Show default info
			primeHandPos.defaultTextFormat = format;
			primeHandPos.width = 200;
			primeHandPos.text = "Left Hand";
			primeHandPos.x = primeHand.x;
			primeHandPos.y = primeHand.y + 20;
			addChild(primeHandPos);
		}
		
		private function createSecondHandVisual():void
		{
			secondHand.x = stage.stageWidth * 2 / 3;
			secondHand.y = stage.stageHeight / 2;
			// Draw a dot
			secondHand.graphics.lineStyle(10,0x0000ff);
			secondHand.graphics.beginFill(0x0000ff);
			secondHand.graphics.moveTo(0,0);
			secondHand.graphics.lineTo(10,0);
			secondHand.graphics.lineTo(10,10);
			secondHand.graphics.lineTo(0,10);
			secondHand.graphics.endFill();
			addChild(secondHand);
			// Show default info
			secondHandPos.defaultTextFormat = format;
			secondHandPos.width = 200;
			secondHandPos.text = "Right Hand";
			secondHandPos.x = secondHand.x;
			secondHandPos.y = secondHand.y + 20;
			addChild(secondHandPos);
		}
		
		private function updateHeadPos(x:Number, y:Number):void
		{
			if(x <= 0 || y <= 0)
				return;
			
			head.x = x;
			head.y = y;
			
			headPos.x = head.x;
			headPos.y = head.y + 20;
			headPos.text = head.x.toFixed(0).toString() + " " + head.y.toFixed(0).toString();
		}
		
		private function updatePrimeHandPos(x:Number, y:Number):void
		{
			if(x <= 0 || y <= 0)
				return;
			
			primeHand.x = x;
			primeHand.y = y;
			
			primeHandPos.x = primeHand.x;
			primeHandPos.y = primeHand.y + 20;
			primeHandPos.text = primeHand.x.toFixed(0).toString() + " " + primeHand.y.toFixed(0).toString();
		}
		
		private function updateSecondHandPos(x:Number, y:Number):void
		{
			if(x <= 0 || y <= 0)
				return;
			
			secondHand.x = x;
			secondHand.y = y;
			
			secondHandPos.x = secondHand.x;
			secondHandPos.y = secondHand.y + 20;
			secondHandPos.text = secondHand.x.toFixed(0).toString() + " " + secondHand.y.toFixed(0).toString();
		}
		
		private function onWave(event:Event)
		{
			gestureInfo.text = "wave";
		}
		
		private function onCircle(event:Event)
		{
			gestureInfo.text = "circle";
		}
		
		private function onSwipeLeft(event:Event)
		{
			gestureInfo.text = "swipe left";
		}
		
		private function onSwipeRight(event:Event)
		{
			gestureInfo.text = "swipe right";
		}
		
		private function onSwipeUp(event:Event)
		{
			gestureInfo.text = "swipe up";
		}
		
		private function onSwipeDown(event:Event)
		{
			gestureInfo.text = "swipe down";
		}
	}
}