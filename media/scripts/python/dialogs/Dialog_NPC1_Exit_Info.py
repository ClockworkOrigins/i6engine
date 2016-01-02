def Dialog_NPC1_Exit_Info():
	Player = getNPC("Player01")
	Player.say("Dialog_NPC1_Exit_01_Sound", "Dialog_NPC1_Exit_01_Subtitle")
	exitDialog()
	return
