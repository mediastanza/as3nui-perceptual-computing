package com.as3nui
{
	import com.as3nui.data.GeoNode;
	
	import flash.events.Event;
	import flash.events.EventDispatcher;
	import flash.events.StatusEvent;
	import flash.external.ExtensionContext;
	import flash.geom.Vector3D;
	
	public class RealSense extends EventDispatcher
	{
		private var _ctx:ExtensionContext;
		
		public function RealSense()
		{
			this._ctx = ExtensionContext.createExtensionContext("com.as3nui.RealSense",null);
			
			this._ctx.addEventListener(StatusEvent.STATUS, onStatus);
		}
		
		private function onStatus(event:StatusEvent):void
		{
			if(event.level == "info")
			{
				if(event.code == "newframe")
				{
					dispatchEvent(new Event("newframe"));	
				}
				if(event.code == "wave")
				{
					dispatchEvent(new Event("wave"));	
				}
				if(event.code == "circle")
				{
					dispatchEvent(new Event("circle"));	
				}
				if(event.code == "swipeleft")
				{
					dispatchEvent(new Event("swipeleft"));	
				}
				if(event.code == "swiperight")
				{
					dispatchEvent(new Event("swiperight"));	
				}
				if(event.code == "swipeup")
				{
					dispatchEvent(new Event("swipeup"));	
				}
				if(event.code == "swipedown")
				{
					dispatchEvent(new Event("swipedown"));	
				}
			}			
		}
		
		public function Start():Object
		{
			// Do it in worker thread
			var result:Object = this._ctx.call("start");
			return result;
		}
		
		public function Stop():Object
		{
			var result:Object = this._ctx.call("stop");
			return result;
		}
		
		public function GetLeftHand():GeoNode
		{
			return this._ctx.call("getLeftHand") as GeoNode;
		}
		
		public function GetRightHand():GeoNode
		{
			return this._ctx.call("getRightHand") as GeoNode;
		}
		
		public function GetHead():Vector3D
		{
			return this._ctx.call("getHead") as Vector3D;
		}
	}
}