package  
{
	import flash.display.MovieClip;
	import flash.geom.Point;
	import OutlinedTextField;
	import LiPUI;
	
	public class ActorWidget extends  MovieClip
	{
		private var valueTexts : Vector.<OutlinedTextField>;
		private var titleTexts : Vector.<OutlinedTextField>;
		
		private static var xScale:Number = 0.5;
		
		private static const Trauma:uint = 0;
		private static const Alcohol:uint = 1;
		private static const Drug:uint = 2;
		private static const Willpower:uint = 3;
		private static const SelfEsteem:uint = 4;
		private static const Spirit:uint = 5;
		private static const Tenacity:uint = 6;
		private static const GenitalDissatisfaction:uint = 7;
		private static const AnalDissatisfaction:uint = 8;
		private static const OralDissatisfaction:uint = 9;
		private static const NippleDissatisfaction:uint = 10;
		private static const GenitalLust:uint = 11;
		private static const AnalLust:uint = 12;
		private static const OralLust:uint = 13;
		private static const NippleLust:uint = 14;
		private static const GenitalHealth:uint = 15;
		private static const AnalHealth:uint = 16;
		private static const OralHealth:uint = 17;
		private static const Sadism:uint = 18;
		private static const Masochism:uint = 19;
		private static const FailedOrgasms:uint = 20;
		public static const MaxAttributeIndex:uint = 20;
	
		private static var titleCoords : Vector.<Point>;
		private static var valCoords : Vector.<Point>;
		private static const Name:uint = 0;
		private static const Diss:uint = 1;
		private static const Lust:uint = 2;
		private static const Health:uint = 3;
		private static const G:uint = 4;
		private static const A:uint = 5;
		private static const O:uint = 6;
		private static const N:uint = 7;
		private static const Mental:uint = 8;
		private static const Tr:uint = 9;
		private static const WP:uint = 10;
		private static const SE:uint = 11;
		private static const Sp:uint = 12;
		private static const Tn:uint = 13;
		private static const Other:uint = 14;
		private static const Al:uint = 15;
		private static const Dr:uint = 16;
		private static const S:uint = 17;
		private static const M:uint = 18;
		private static const Fails:uint = 19;
		
		// Name of the character
		//        G   A   O   N
		//Diss   999 999 999 999
		//Lust   999 999 999 999
		//Health 999 999 999 
		//Mental  Tr  WP  SE  Sp  Tn
		//       999 999 999 999 999
		//Other   Al  Dr  S   M
		//       999 999 999 999
		//Fails 99999.999
		public static function InitPositions() : void
		{
			if (!titleCoords || titleCoords.length == 0)
			{
				titleCoords = new Vector.<Point>(20);
			}
			if (!valCoords || valCoords.length == 0)
			{
				valCoords = new Vector.<Point>(21);
			}
			titleCoords[Name] =   new Point( 1, 0);
			titleCoords[Diss] =   new Point( 0, 2);
			titleCoords[Lust] =   new Point( 0, 3);
			titleCoords[Health] = new Point( 0, 4);
			titleCoords[G] =      new Point( 8, 1);
			titleCoords[A] =      new Point(12, 1);
			titleCoords[O] =      new Point(16, 1);
			titleCoords[N] =      new Point(20, 1);
			titleCoords[Mental] = new Point( 0, 5);
			titleCoords[Tr] =     new Point( 8, 5);
			titleCoords[WP] =     new Point(12, 5);
			titleCoords[SE] =     new Point(16, 5);
			titleCoords[Sp] =     new Point(20, 5);
			titleCoords[Tn] =     new Point(24, 5);
			titleCoords[Other] =  new Point( 0, 7);
			titleCoords[Al] =     new Point( 8, 7);
			titleCoords[Dr] =     new Point(12, 7);
			titleCoords[S] =      new Point(16, 7);
			titleCoords[M] =      new Point(20, 7);
			titleCoords[Fails] =  new Point( 0, 9);

			valCoords[GenitalDissatisfaction] = new Point( 7, 2);
			valCoords[AnalDissatisfaction] =    new Point(11, 2);
			valCoords[OralDissatisfaction] =    new Point(15, 2);
			valCoords[NippleDissatisfaction] =  new Point(19, 2);

			valCoords[GenitalLust] = new Point( 7, 3);
			valCoords[AnalLust] =    new Point(11, 3);
			valCoords[OralLust] =    new Point(15, 3);
			valCoords[NippleLust] =  new Point(19, 3);
			
			valCoords[GenitalHealth] = new Point( 7, 4);
			valCoords[AnalHealth] =    new Point(11, 4);
			valCoords[OralHealth] =    new Point(15, 4);
			
			valCoords[Trauma] =     new Point( 7.5, 6);
			valCoords[Willpower] =  new Point(11.5, 6);
			valCoords[SelfEsteem] = new Point(15.5, 6);
			valCoords[Spirit] =     new Point(19.5, 6);
			valCoords[Tenacity] =   new Point(23.5, 6);
			
			valCoords[Alcohol] =   new Point( 7, 8);
			valCoords[Drug] =      new Point(11, 8);
			valCoords[Sadism] =    new Point(15, 8);
			valCoords[Masochism] = new Point(19, 8);
			
			valCoords[FailedOrgasms] = new Point(6, 9);
		}
		
		
		public function ActorWidget(name:String) : void
		{
			super();
			visible = false;
			Build(name);
			scaleX = LiPUI.WidgetScaleX;
		}
		
		private function Build(name:String) : void
		{
			// instantiate elements
			valueTexts = new Vector.<OutlinedTextField>(21, true);
			titleTexts = new Vector.<OutlinedTextField>(20);
			InitAndPosition(name);
		}
		

		private function InitAndPosition(name:String) : void 
		{
			titleTexts[Name] = CreateTitle(Name, name);
			titleTexts[Name].SetBold(true);
			titleTexts[Diss] = CreateTitle(Diss, "Diss");
			titleTexts[Lust] = CreateTitle(Lust, "Lust");
			titleTexts[Health] = CreateTitle(Health, "Health");
			titleTexts[G] = CreateTitle(G, "G");
			titleTexts[A] = CreateTitle(A, "A");
			titleTexts[O] = CreateTitle(O, "O");
			titleTexts[N] = CreateTitle(N, "N");
			titleTexts[Mental] = CreateTitle(Mental, "Mental");
			titleTexts[Tr] = CreateTitle(Tr, "Tr");
			titleTexts[WP] = CreateTitle(WP, "WP");
			titleTexts[SE] = CreateTitle(SE, "SE");
			titleTexts[Sp] = CreateTitle(Sp, "Sp");
			titleTexts[Tn] = CreateTitle(Tn, "Tn");
			titleTexts[Other] = CreateTitle(Other, "Other");
			titleTexts[Al] = CreateTitle(Al, "Al");
			titleTexts[Dr] = CreateTitle(Dr, "Dr");
			titleTexts[S] = CreateTitle(S, "S");
			titleTexts[M] = CreateTitle(M, "M");
			titleTexts[Fails] = CreateTitle(Fails, "Fails");
			
			var i:uint;
			for (i = 0; i <= FailedOrgasms; i++)
			{
				valueTexts[i] = CreateValue(i);
			}
		}
		
		private function SetTitlePosition(index:uint, t:OutlinedTextField) : void
		{
			var p:Point = titleCoords[index];
			t.x = p.x * LiPUI.FontSize * xScale;
			t.y = p.y * LiPUI.FontSize;
		}
		private function SetValuePosition(index:uint, t:OutlinedTextField) : void
		{
			var p:Point = valCoords[index];
			t.x = p.x * LiPUI.FontSize * xScale;
			t.y = p.y * LiPUI.FontSize;
		}
		private function CreateTitle(index:uint, title:String) : OutlinedTextField
		{
			var t:OutlinedTextField = new OutlinedTextField(true);
			SetTitlePosition(index, t);
			t.SetText(title);
			addChild(t);
			return t;
		}
		private function CreateValue(index:uint, value:String = "---") : OutlinedTextField
		{
			var v:OutlinedTextField = new OutlinedTextField(false);
			SetValuePosition(index, v);
			v.SetText(value);
			addChild(v);
			return v;
		}
		
		public function UpdateFont() : void
		{
			var i:uint;
			for (i = 0; i < titleTexts.length; i++)
			{
				var t:OutlinedTextField = titleTexts[i];
				t.UpdateFormat();
				t.SetMainColor(LiPUI.FontColor);
				SetTitlePosition(i, t);
			}
			for (i = 0; i < valueTexts.length; i++)
			{
				var v:OutlinedTextField = valueTexts[i];
				v.UpdateFormat();
				SetValuePosition(i, v);
			}
		}
		
		public function SetValue(index:uint, value:String, color:uint) : void
		{
			valueTexts[index].SetText(value);
			valueTexts[index].SetMainColor(color);
		}
		
		public function SetAnimationState(index:uint, bold:Boolean, alpha:Number) : void
		{
			valueTexts[index].SetBold(bold)
			valueTexts[index].alpha = alpha;
		}
	}

}