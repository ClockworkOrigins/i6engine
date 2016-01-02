def Dialog_NPC1_Trade_Info():
	NPC01 = getNPC("NPC01")
	Player = getNPC("Player01")
	Player.say("Dialog_NPC1_Trade_01_Sound", "Dialog_NPC1_Trade_01_Subtitle")
	NPC01.say("Dialog_NPC1_Trade_02_Sound", "Dialog_NPC1_Trade_02_Subtitle")
	Player.trade(NPC01, 0.5, 1.0)
	return