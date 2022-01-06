Scriptname LiP:UIColorCoding extends Quest


int Property Normal = 0x00FFFFFF  Auto ; white 16777215
int Property ModeratePositive = 0x002E86C1 Auto ; light blue 3049153
int Property HighPositive = 0x0017A589 Auto; green 1549705
int Property MaxPositive = 0x0028B463 Auto; deep green 2667619
int Property ModerateNegative = 0x00F4FA58 Auto; yellow 16054872
int Property HighNegative = 0x00FFBF00 Auto; orange 16760576
int Property MaxNegative = 0x00FE2E2E Auto; red 16657966

LiP:UI:Interval[] function GetFullRange()
    ; [-50, -40) max negative
    ; [-40, -30) high negative
    ; [-10, -30) moderate negative
    ; [-10,  10) normal
    ; [ 10,  30) moderate positive
    ; [ 30,  40) high positive
    ; [ 40, +++ max positive

    LiP:UI:Interval[] ranges = new LiP:UI:Interval[7]
    
    ranges[0] = new LiP:UI:Interval
    ranges[0].Color = MaxNegative
    ranges[0].Border = -40

    ranges[1] = new LiP:UI:Interval
    ranges[1].Color = HighNegative
    ranges[1].Border = -30

    ranges[2] = new LiP:UI:Interval
    ranges[2].Color = ModerateNegative
    ranges[2].Border = -10

    ranges[3] = new LiP:UI:Interval
    ranges[3].Color = Normal
    ranges[3].Border = 10

    ranges[4] = new LiP:UI:Interval
    ranges[4].Color = ModeratePositive
    ranges[4].Border = 30

    ranges[5] = new LiP:UI:Interval
    ranges[5].Color = HighPositive
    ranges[5].Border = 40

    ranges[6] = new LiP:UI:Interval
    ranges[6].Color = MaxPositive
    ;ranges[6].Border = ; doesn't matter, ignored actually
    return ranges
endfunction

LiP:UI:Interval[] function GetHealthRange()
    ; [ 0 - 20) normal
    ; [20 - 60) moderate
    ; [60 - 90) high 
    ; [90 - +++ max
    LiP:UI:Interval[] ranges = new LiP:UI:Interval[4]

    ranges[0] = new LiP:UI:Interval
    ranges[0].Color = MaxNegative
    ranges[0].Border = 20

    ranges[1] = new LiP:UI:Interval
    ranges[1].Color = HighNegative
    ranges[1].Border = 60

    ranges[2] = new LiP:UI:Interval
    ranges[2].Color = ModerateNegative
    ranges[2].Border = 90

    ranges[3] = new LiP:UI:Interval
    ranges[3].Color = Normal
    ;ranges[3].Border =  ; doesn't matter, ignored actually
    return ranges
endfunction

LiP:UI:Interval[] function GetNegativeRange()
    ; [ 0 - 20) normal
    ; [20 - 60) moderate
    ; [60 - 90) high 
    ; [90 - +++ max
    LiP:UI:Interval[] ranges = new LiP:UI:Interval[4]

    ranges[0] = new LiP:UI:Interval
    ranges[0].Color = Normal
    ranges[0].Border = 20

    ranges[1] = new LiP:UI:Interval
    ranges[1].Color = ModerateNegative
    ranges[1].Border = 60

    ranges[2] = new LiP:UI:Interval
    ranges[2].Color = HighNegative
    ranges[2].Border = 90

    ranges[3] = new LiP:UI:Interval
    ranges[3].Color = MaxNegative
    ;ranges[3].Border =  ; doesn't matter, ignored actually
    return ranges
endfunction

LiP:UI:Interval[] function GetFailedRange()
    ; [0-10] normal
    ; [10-+++] negative
    LiP:UI:Interval[] ranges = new LiP:UI:Interval[2]

    ranges[0] = new LiP:UI:Interval
    ranges[0].Color = Normal
    ranges[0].Border = 10

    ranges[1] = new LiP:UI:Interval
    ranges[1].Color = ModerateNegative
    ;ranges[1].Border =  ; doesn't matter, ignored actually
    return ranges
endfunction

string Property NormalColor 
    string function get()
        return LiP:UI.ToHexString(Normal)
    endfunction
    function set(string value)
        int val = LiP:UI.FromHexString(value)
        if val
            Normal = val
        endif
    endfunction
endproperty
string Property ModeratePositiveColor 
    string function get()
        return LiP:UI.ToHexString(ModeratePositive)
    endfunction
    function set(string value)
        int val = LiP:UI.FromHexString(value)
        if val
            ModeratePositive = val
        endif
    endfunction
endproperty
string Property HighPositiveColor 
    string function get()
        return LiP:UI.ToHexString(HighPositive)
    endfunction
    function set(string value)
        int val = LiP:UI.FromHexString(value)
        if val
            HighPositive = val
        endif
    endfunction
endproperty
string Property MaxPositiveColor 
    string function get()
        return LiP:UI.ToHexString(MaxPositive)
    endfunction
    function set(string value)
        int val = LiP:UI.FromHexString(value)
        if val
            MaxPositive = val
        endif
    endfunction
endproperty
string Property ModerateNegativeColor 
    string function get()
        return LiP:UI.ToHexString(ModerateNegative)
    endfunction
    function set(string value)
        int val = LiP:UI.FromHexString(value)
        if val
            ModerateNegative = val
        endif
    endfunction
endproperty
string Property HighNegativeColor 
    string function get()
        return LiP:UI.ToHexString(HighNegative)
    endfunction
    function set(string value)
        int val = LiP:UI.FromHexString(value)
        if val
            HighNegative = val
        endif
    endfunction
endproperty
string Property MaxNegativeColor 
    string function get()
        return LiP:UI.ToHexString(MaxNegative)
    endfunction
    function set(string value)
        int val = LiP:UI.FromHexString(value)
        if val
            MaxNegative = val
        endif
    endfunction
endproperty
