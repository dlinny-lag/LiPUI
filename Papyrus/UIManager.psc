Scriptname LiP:UIManager extends Quest

LiP:API Property LiPAPI Auto Const
LiP:UpdateManager Property UpdateManager Auto Const
LiP:OrgasmsHandler Property OrgasmsHandler Auto Const
LiP:SentientFilter Property SentientFilter Auto Const
LiP:UIColorCoding Property UIColorCoding Auto Const
Keyword Property TrackedActors Auto Const Mandatory

string mcmModName = "LivingIsPain~UI" const

int FontSize = 10
bool AllowPlayerWidget = true
int PlayerWidgetX = 10
int PlayerWidgetY = 300
bool AllowNPCWidgets = true
int MaxDistance = 500

Actor player

int Trauma = 0 const
int AlcoholIntox = 1 const
int DrugIntox = 2 const
int Willpower = 3 const
int SelfEsteem = 4 const
int Spirit = 5 const
int Tenacity = 6 const
int GenitalDissatisfaction = 7 const
int AnalDissatisfaction = 8 const
int OralDissatisfaction = 9 const
int NippleDissatisfaction = 10 const
int GenitalLust = 11 const
int AnalLust = 12 const
int OralLust = 13 const
int NippleLust = 14 const
int GenitalHealth = 15 const
int AnalHealth = 16 const
int OralHealth = 17 const
int Sadism = 18 const
int Masochism = 19 const
int FailedOrgasms = 20 const
ActorValue[] MyAttributes
function InitAttributes()
    MyAttributes = new ActorValue[21]
    MyAttributes[Trauma] = LiPAPI.Trauma
    MyAttributes[AlcoholIntox] = LiPAPI.AlcoholIntox
    MyAttributes[DrugIntox] = LiPAPI.DrugIntox
    MyAttributes[Willpower] = LiPAPI.Willpower
    MyAttributes[SelfEsteem] = LiPAPI.SelfEsteem
    MyAttributes[Spirit] = LiPAPI.Spirit
    MyAttributes[Tenacity] = LiPAPI.Tenacity
    MyAttributes[GenitalDissatisfaction] = LiPAPI.GenitalDissatisfaction
    MyAttributes[AnalDissatisfaction] = LiPAPI.AnalDissatisfaction
    MyAttributes[OralDissatisfaction] = LiPAPI.OralDissatisfaction
    MyAttributes[NippleDissatisfaction] = LiPAPI.NippleDissatisfaction
    MyAttributes[GenitalLust] = LiPAPI.GenitalLust
    MyAttributes[AnalLust] = LiPAPI.AnalLust
    MyAttributes[OralLust] = LiPAPI.OralLust
    MyAttributes[NippleLust] = LiPAPI.NippleLust
    MyAttributes[GenitalHealth] = LiPAPI.GenitalHealth
    MyAttributes[AnalHealth] = LiPAPI.AnalHealth
    MyAttributes[OralHealth] = LiPAPI.OralHealth
    MyAttributes[Sadism] = LiPAPI.Sadism
    MyAttributes[Masochism] = LiPAPI.Masochism

    MyAttributes[FailedOrgasms] = OrgasmsHandler.FailedOrgasms
endfunction

Event OnInit()
    RegisterForRemoteEvent(Game.GetPlayer(), "OnPlayerLoadGame")
    RegisterForCustomEvent(UpdateManager, "OnUpdate")
    player = Game.GetPlayer()
    DS:FormSet.Create(TrackedActors)
    InitAttributes()
    EnsureSubscribed()
Endevent

Event LiP:UpdateManager.OnUpdate(LiP:UpdateManager sender, var[] args)

EndEvent

Event Actor.OnPlayerLoadGame(Actor a)
    EnsureSubscribed()
Endevent

int showPlayerWidgetTimer = 10 const
int showWarningTimer = 11 const


event OnTimer(int timerId)
    if showPlayerWidgetTimer == timerId
        if AllowPlayerWidget
            ShowPlayerWidget()
        endif
    elseif showWarningTimer == timerId
        if !CheckOldVersion()
            StartTimer(1, showWarningTimer)
        endif
    endif
endevent

bool function CheckOldVersion()
    if Game.IsPluginInstalled("LiPHUDAddon.esl")
        Quest MQ101 = Game.GetFormFromFile(0x0001ED86, "Fallout4.esm") as Quest
        if MQ101.GetStage() < 15
            return false
        endif
        Debug.MessageBox("LiP UI\nIt is highly recommended to uninstall 'LiP HUD Addon'")
    endif
    int aa = AA:Plugin.GetVersionInt()
    if aa < 0x0081
        Quest MQ101 = Game.GetFormFromFile(0x0001ED86, "Fallout4.esm") as Quest
        if MQ101.GetStage() < 15
            return false
        endif
        Debug.MessageBox("LiP UI\nAdditional Attributes 0.8.1 or higher is required")
    endif
    return true
endfunction

function ShowPlayerWidget()
    LiP:UI.EnableWidget(player, player.GetDisplayName())
    LiP:UI.SetPlayerWidgetPosition(PlayerWidgetX, PlayerWidgetY)
endfunction
Function HidePlayerWidget()
    LiP:UI.DisableWidget(player)
EndFunction

function SetColorSchemes()
    LiP:UI:Interval[] full = UIColorCoding.GetFullRange()
    LiP:UI:Interval[] health = UIColorCoding.GetHealthRange()
    LiP:UI:Interval[] negative = UIColorCoding.GetNegativeRange()

    ;Debug.Trace("Ranges:")
    ;Debug.Trace(full)
    ;Debug.Trace(negative)

    LiP:UI.SetColorScheme(MyAttributes[Trauma], negative)
    LiP:UI.SetColorScheme(MyAttributes[AlcoholIntox], negative)
    LiP:UI.SetColorScheme(MyAttributes[DrugIntox], negative)
    LiP:UI.SetColorScheme(MyAttributes[Willpower], full)
    LiP:UI.SetColorScheme(MyAttributes[SelfEsteem], full)
    LiP:UI.SetColorScheme(MyAttributes[Spirit], full)
    LiP:UI.SetColorScheme(MyAttributes[Tenacity], full)
    LiP:UI.SetColorScheme(MyAttributes[GenitalDissatisfaction], negative)
    LiP:UI.SetColorScheme(MyAttributes[AnalDissatisfaction], negative)
    LiP:UI.SetColorScheme(MyAttributes[OralDissatisfaction], negative)
    LiP:UI.SetColorScheme(MyAttributes[NippleDissatisfaction], negative)
    LiP:UI.SetColorScheme(MyAttributes[GenitalLust], negative)
    LiP:UI.SetColorScheme(MyAttributes[AnalLust], negative)
    LiP:UI.SetColorScheme(MyAttributes[OralLust], negative)
    LiP:UI.SetColorScheme(MyAttributes[NippleLust], negative)
    LiP:UI.SetColorScheme(MyAttributes[GenitalHealth], health)
    LiP:UI.SetColorScheme(MyAttributes[AnalHealth], health)
    LiP:UI.SetColorScheme(MyAttributes[OralHealth], health)
    LiP:UI.SetColorScheme(MyAttributes[Sadism], negative)
    LiP:UI.SetColorScheme(MyAttributes[Masochism], negative)

    LiP:UI.SetColorScheme(MyAttributes[FailedOrgasms], UIColorCoding.GetFailedRange())
endfunction


function EnsureSubscribed()
    DS:FormSet.Clear(TrackedActors)
    AA:AVChanges.UnregisterForAttributesChanges(self) ; could be registered by previous version

    ; init UI
    if LoadSettingFromMCM()
        UnregisterForExternalEvent("OnMCMSettingChange|" + mcmModName)
        RegisterForExternalEvent("OnMCMSettingChange|"+mcmModName, "OnMCMSettingChange")
    endif
    bool[] flags = new bool[FailedOrgasms+1]
    flags[FailedOrgasms] = true
    LiP:UI.__RegisterAttributes(MyAttributes, flags)
    SetColorSchemes()
    LiP:UI.SetFontSize(FontSize)
    LiP:UI.SetMaxDistance(MaxDistance)

    Actor[] actors = new Actor[1]
    actors[0] = player
    DS:FormSet.Add(TrackedActors, player)

    StartTimer(0.1, showPlayerWidgetTimer)
    StartTimer(1, showWarningTimer)
    
    CheckAAF()

endfunction

Function OnMCMSettingChange(string modName, string id)
    ;Debug.Trace(modname + ": " + id + " changed")
    if id == "iFontSize:Default"
        FontSize = MCM.GetModSettingInt(mcmModName, "iFontSize:Default")
        LiP:UI.SetFontSize(FontSize)
        return
    endif
    bool xy = false
    if id == "iPositionX:Default"
        PlayerWidgetX = MCM.GetModSettingInt(mcmModName, "iPositionX:Default")
        xy = true
    elseif id == "iPositionY:Default"
        PlayerWidgetY = MCM.GetModSettingInt(mcmModName, "iPositionY:Default")
        xy = true
    elseif id == "iMaxDistance:Default"
        MaxDistance = MCM.GetModSettingInt(mcmModName, "iMaxDistance:Default")
        LiP:UI.SetMaxDistance(MaxDistance)
    elseif id == "bShowPlayerWidget:Default"
        AllowPlayerWidget = MCM.GetModSettingBool(mcmModName, "bShowPlayerWidget:Default")
        if AllowPlayerWidget
            ShowPlayerWidget()
        else
            HidePlayerWidget()
        endif
        return
    elseif id == "bShowNPCWidgets:Default"
        AllowNPCWidgets = MCM.GetModSettingBool(mcmModName, "bShowNPCWidgets:Default")
        if !AllowNPCWidgets
            Form[] actors = DS:FormSet.ToArray(TrackedActors)
            UntrackAllNPCs(actors)
        endif
        return
    endif
    if xy
        if AllowPlayerWidget
            ShowPlayerWidget()
        endif
        return
    endif

    if id == "sNormalColor:Default"
        int c = UIColorCoding.Normal
        UIColorCoding.NormalColor = MCM.GetModSettingString(mcmModName, "sNormalColor:Default")
        if c == UIColorCoding.Normal
            MCM.SetModSettingString(mcmModName, "sNormalColor:Default", UIColorCoding.NormalColor)
            MCM.RefreshMenu()
        endif
    elseif id == "sModeratePositiveColor:Default"
        int c = UIColorCoding.ModeratePositive
        UIColorCoding.ModeratePositiveColor = MCM.GetModSettingString(mcmModName, "sModeratePositiveColor:Default")
        if c == UIColorCoding.ModeratePositive
            MCM.SetModSettingString(mcmModName, "sModeratePositiveColor:Default", UIColorCoding.ModeratePositiveColor)
            MCM.RefreshMenu()
        endif
    elseif id == "sHighPositiveColor:Default"
        int c = UIColorCoding.HighPositive
        UIColorCoding.HighPositiveColor = MCM.GetModSettingString(mcmModName, "sHighPositiveColor:Default")
        if c == UIColorCoding.HighPositive
            MCM.SetModSettingString(mcmModName, "sHighPositiveColor:Default", UIColorCoding.HighPositiveColor)
            MCM.RefreshMenu()
        endif
    elseif id == "sMaxPositiveColor:Default"
        int c = UIColorCoding.MaxPositive
        UIColorCoding.MaxPositiveColor = MCM.GetModSettingString(mcmModName, "sMaxPositiveColor:Default")
        if c == UIColorCoding.MaxPositive
            MCM.SetModSettingString(mcmModName, "sMaxPositiveColor:Default", UIColorCoding.MaxPositiveColor)
            MCM.RefreshMenu()
        endif
    elseif id == "sModerateNegativeColor:Default"
        int c = UIColorCoding.ModerateNegative
        UIColorCoding.ModerateNegativeColor = MCM.GetModSettingString(mcmModName, "sModerateNegativeColor:Default")
        if c == UIColorCoding.ModerateNegative
            MCM.SetModSettingString(mcmModName, "sModerateNegativeColor:Default", UIColorCoding.ModerateNegativeColor)
            MCM.RefreshMenu()
        endif
    elseif id == "sHighNegativeColor:Default"
        int c = UIColorCoding.HighNegative
        UIColorCoding.HighNegativeColor = MCM.GetModSettingString(mcmModName, "sHighNegativeColor:Default")
        if c == UIColorCoding.HighNegative
            MCM.SetModSettingString(mcmModName, "sHighNegativeColor:Default", UIColorCoding.HighNegativeColor)
            MCM.RefreshMenu()
        endif
    elseif id == "sMaxNegativeColor:Default"
        int c = UIColorCoding.MaxNegative
        UIColorCoding.MaxNegativeColor = MCM.GetModSettingString(mcmModName, "sMaxNegativeColor:Default")
        if c == UIColorCoding.MaxNegative
            MCM.SetModSettingString(mcmModName, "sMaxNegativeColor:Default", UIColorCoding.MaxNegativeColor)
            MCM.RefreshMenu()
        endif
    endif

    SetColorSchemes()
endfunction

 ; returns MCM installation status
bool Function LoadSettingFromMCM()
    if !MCM.IsInstalled()
        return false
    endif
    FontSize = MCM.GetModSettingInt(mcmModName, "iFontSize:Default")
    AllowPlayerWidget = MCM.GetModSettingInt(mcmModName, "bShowPlayerWidget:Default")
    PlayerWidgetX = MCM.GetModSettingInt(mcmModName, "iPositionX:Default")
    PlayerWidgetY = MCM.GetModSettingInt(mcmModName, "iPositionY:Default")
    AllowNPCWidgets = MCM.GetModSettingBool(mcmModName, "bShowNPCWidgets:Default")
    MaxDistance = MCM.GetModSettingInt(mcmModName, "iMaxDistance:Default")
    UIColorCoding.NormalColor = MCM.GetModSettingString(mcmModName, "sNormalColor:Default")
    UIColorCoding.ModeratePositiveColor = MCM.GetModSettingString(mcmModName, "sModeratePositiveColor:Default")
    UIColorCoding.HighPositiveColor = MCM.GetModSettingString(mcmModName, "sHighPositiveColor:Default")
    UIColorCoding.MaxPositiveColor = MCM.GetModSettingString(mcmModName, "sMaxPositiveColor:Default")
    UIColorCoding.ModerateNegativeColor = MCM.GetModSettingString(mcmModName, "sModerateNegativeColor:Default")
    UIColorCoding.HighNegativeColor = MCM.GetModSettingString(mcmModName, "sHighNegativeColor:Default")
    UIColorCoding.MaxNegativeColor = MCM.GetModSettingString(mcmModName, "sMaxNegativeColor:Default")

    return true
endfunction

AAF:AAF_API aafApi
bool aafSubscribed
function CheckAAF()
    if aafApi && aafSubscribed
        return
    endif
    aafSubscribed = false
    if !aafApi 
        aafApi = Game.GetFormFromFile(0x00000F99, "AAF.esm") as AAF:AAF_API
    endif
    aafSubscribed = aafApi
    RegisterForCustomEvent(aafApi, "OnAnimationStart")
    RegisterForCustomEvent(aafApi, "OnAnimationStop")
endfunction

; for debug purposes only
function TrackNPC(Actor a)
    if a && a != player
        DS:FormSet.Add(TrackedActors, a)
        ShowNPCWidget(a)
    endif
endfunction

function ShowNPCWidget(Actor a)
    if a && a != player
        LiP:UI.EnableWidget(a, a.GetDisplayName())
    endif
endfunction

Event AAF:AAF_API.OnAnimationStart(AAF:AAF_API sender, var[] args)
    if AllowNPCWidgets && args[0] as int == 0
        Form[] actors = Utility.VarToVarArray(args[1]) as Form[]
        DS:FormSet.AddRange(TrackedActors, actors)
        int i = 0
        while i < actors.Length
            Actor a = actors[i] as Actor
            if SentientFilter.IsActorValid(a)
                ShowNPCWidget(a)
            endif
            i += 1
        endwhile
    endif
Endevent

function UntrackAllNPCs(Form[] actors)
    DS:FormSet.RemoveRange(TrackedActors, actors)
    DS:FormSet.Add(TrackedActors, player) ; don't remove player. yes, overhead, but simplification
    int i = 0
    while i < actors.Length
        Actor a = actors[i] as Actor
        if a != player
            LiP:UI.DisableWidget(a)
        endif
        i += 1
    endwhile
endfunction

Event AAF:AAF_API.OnAnimationStop(AAF:AAF_API sender, var[] args)
    Form[] actors = Utility.VarToVarArray(args[1]) as Form[]
    UntrackAllNPCs(actors)
Endevent