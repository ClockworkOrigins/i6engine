def Dialog_NPC1_Hi_Info():
	NPC01 = getNPC("NPC01")
	Player = getNPC("Player01")
	NPC01.say("Dialog_NPC1_Hi_01_Sound", "Dialog_NPC1_Hi_01_Subtitle")
	Player.say("Dialog_NPC1_Hi_02_Sound", "Dialog_NPC1_Hi_02_Subtitle")
	return