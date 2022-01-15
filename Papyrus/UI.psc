Scriptname LiP:UI Native Hidden Const

string function GetVersionString() native global
int function GetVersionint() native global

bool function SetFontSize(float size) native global
function SetMaxDistance(int distance) native global
bool function EnableWidget(Actor a, string name) native global
bool function DisableWidget(Actor a) native global
bool function SetPlayerWidgetPosition(int x, int y) native global

struct Interval
    float Border
    int Color
endstruct
bool function SetColorScheme(ActorValue attribute, Interval[] intervals) global native

string function ToHexString(int number) global native
int function FromHexString(string value) global native

; internal. do not call
bool function __RegisterAttributes(ActorValue[] attributes, bool[] flags) native global


