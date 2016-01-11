function Dialog_NPC1_Exit_Info()
	local Player = getNPC("Player01")
	Player:say("Dialog_NPC1_Exit_01_Sound", "Dialog_NPC1_Exit_01_Subtitle")
	exitDialog()
end