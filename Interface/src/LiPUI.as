package 
{
	import flash.utils.Dictionary;
    import flash.display.*;
    import flash.text.*;
	import flash.net.URLLoader;
    import flash.net.URLRequest;
    import flash.events.Event;
	import flash.filters.GlowFilter;
	import flash.filters.BitmapFilterQuality;
    import flash.display.Loader;
	import ActorWidget;
	
	public class LiPUI extends MovieClip
	{
		public static const FontName:String = "$MAIN_Font"
		public static const BoldFontName:String = "$MAIN_Font_Bold"
		public static var FontSize:Number = 10;
		public static var FontColor:uint = 0x00FFFFFF;
		public static var WidgetScaleX:Number = 0.5;
		
		private var allWidgets: Dictionary;
		private var ready:Boolean;
		
		[Embed(source="../fonts/Murecho-Medium.ttf", fontName ="$MAIN_Font", mimeType = "application/x-font", advancedAntiAliasing="true", embedAsCFF="false")]
		private static var MAIN_Font:Class;
		
		[Embed(source="../fonts/Murecho-Black.ttf", fontName = "$MAIN_Font_Bold", mimeType = "application/x-font", advancedAntiAliasing="true", embedAsCFF="false")]
		private static var MAIN_Font_Bold:Class;
		
		public function LiPUI()
		{
			Font.registerFont(MAIN_Font);
			Font.registerFont(MAIN_Font_Bold);
			allWidgets = new Dictionary();
			ready = false;
			super();
			addEventListener(Event.ADDED_TO_STAGE, onStageReady);
		}
		private function Startup() : void
		{
			ActorWidget.InitPositions();
			ready = true;
		}
		private function onStageReady(e:Event):void
		{
			stage.displayState = StageDisplayState.FULL_SCREEN;
			Startup()
		}
		
		public function SetScaleX(scale:Number):void
		{
			WidgetScaleX = scale;
		}
		
		public function IsReady():Boolean
		{
			return ready;
		}
		
		public function GetStageHeight() : int
		{
			return stage.stageHeight;
		}
		
		public function GetStageWidth() : int
		{
			return stage.stageWidth;
		}
		
		public function AddWidget(actorId:uint, actorName:String) : Boolean
		{
			var existing:ActorWidget = allWidgets[actorId];
			if (existing)
				return false;
			
			var widget:ActorWidget = new ActorWidget(actorName);
			allWidgets[actorId] = widget;
			addChild(widget);
			return true;
		}
		public function RemoveWidget(actorId:uint) : Boolean
		{
			var existing:ActorWidget = allWidgets[actorId];
			if (existing)
			{
				removeChild(existing);
				delete allWidgets[actorId];
				return true;
			}
			return false;
		}
		public function SetWidgetPosition(actorId:uint, x:int, y:int):Boolean
		{
			var existing:ActorWidget = allWidgets[actorId];
			if (existing)
			{
				existing.x = x;
				existing.y = y;
				return true;
			}
			return false;
		}
		
		public function SetActorValue(actorId:uint, valueIndex:uint, value:String, color:uint) : Boolean
		{
			if (actorId == 0)
				return false;
				
			if (valueIndex < 0 || valueIndex > ActorWidget.MaxAttributeIndex)
				return false;
				
			var existing:ActorWidget = allWidgets[actorId];
			if (existing)
			{
				existing.visible = true; // debatable idea
				existing.SetValue(valueIndex, value, color);
				return true;
			}
			return false;
		}
		public function SetActorValueAnimationState(actorId:uint, valueIndex:uint, bold:Boolean, alpha:Number): Boolean
		{
			var existing:ActorWidget = allWidgets[actorId];
			if (existing)
			{
				existing.SetAnimationState(valueIndex, bold, alpha);
				return true;
			}
			return false;
		}
		public function SetFontSize(size:Number) : void
		{
			FontSize = size;
			for each (var widget:ActorWidget in allWidgets)
			{
				widget.UpdateFont();
			}
		}
		
		public function SetFontColor(color:uint) : void
		{
			FontColor = color;
			for each (var widget:ActorWidget in allWidgets)
			{
				widget.UpdateFont();
			}
		}
		public function ClearWidgets() : void
		{
			for each(var widget:ActorWidget in allWidgets)
			{
				removeChild(widget);
			}
			allWidgets = new Dictionary();
		}
		
		public function SetVisibility(actorId:uint, visibility:Boolean) : Boolean
		{
			var existing:ActorWidget = allWidgets[actorId];
			if (existing)
			{
				existing.visible = visibility;
				return true;
			}
			return false;
		}
		
		public function GetWidgetWidth(actorId:uint) : int
		{
			var existing:ActorWidget = allWidgets[actorId];
			if (existing)
			{
				return existing.width;
			}
			return -1;
		}
		
		public function GetWidgetHeight(actorId:uint) : int
		{
			var existing:ActorWidget = allWidgets[actorId];
			if (existing)
			{
				return existing.height;
			}
			return -1;
		}
	}
}