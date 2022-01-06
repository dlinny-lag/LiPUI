package  
{
	import flash.display.MovieClip;
	import flash.text.*;
	import flash.filters.BitmapFilterQuality;
	import LiPUI;
	public class OutlinedTextField extends  MovieClip
	{
		private var main:TextField;
		private var back:TextField;
		public function OutlinedTextField(title:Boolean) : void
		{
			var format:TextFormat = new TextFormat(LiPUI.FontName, LiPUI.FontSize);
			main = new TextField();
			main.embedFonts = true;
			main.defaultTextFormat = format;
			main.type = TextFieldType.DYNAMIC;
			main.autoSize = TextFieldAutoSize.LEFT;
			main.multiline = false;
			if (title)
				main.textColor = LiPUI.FontColor;
			else
				main.textColor = 0x00FFFFFF;
			main.antiAliasType = flash.text.AntiAliasType.ADVANCED;

			back = new TextField();
			back.embedFonts = true;
			back.defaultTextFormat = format;
			back.type = TextFieldType.DYNAMIC;
			back.autoSize = TextFieldAutoSize.LEFT;
			back.multiline = false;
			back.textColor = 0;
			back.antiAliasType = flash.text.AntiAliasType.ADVANCED;
		
			back.x = 1;
			back.y = 1;
			
			addChild(back);
			addChild(main);
		}

		public function SetMainColor(mainColor:uint) : void
		{
			var format:TextFormat = main.getTextFormat();
			format.color = mainColor;
			main.setTextFormat(format);
			main.textColor = mainColor
		}
		public function SetWidth(width:uint) : void
		{
			main.width = width;
			back.width = width;
		}
		public function SetBold(bold:Boolean) : void
		{
			var fontName:String;
			if (bold)
				fontName = LiPUI.BoldFontName;
			else
				fontName = LiPUI.FontName;
				
			var format:TextFormat = main.getTextFormat();
			format.font = fontName
			main.defaultTextFormat = format;
			main.setTextFormat(format);
			
			format = back.getTextFormat();
			format.font = fontName;
			back.defaultTextFormat = format;
			back.setTextFormat(format);
		}
		public function SetText(val:String) : void
		{
			main.text = val;
			back.text = val;
		}
		
		public function UpdateFormat() : void
		{
			main.defaultTextFormat.size = LiPUI.FontSize;
			var format:TextFormat = main.getTextFormat();
			format.size = LiPUI.FontSize;
			main.setTextFormat(format);
			
			back.defaultTextFormat.size = LiPUI.FontSize;
			format = back.getTextFormat();
			format.size = LiPUI.FontSize;
			back.setTextFormat(format);
		}
	}

}