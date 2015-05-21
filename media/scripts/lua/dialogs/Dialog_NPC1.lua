﻿function Dialog_NPC1_Hi_Info()
	NPC01 = getNPC("NPC01")
	Player = getNPC("Player01")
	NPC01:say("Dialog_NPC1_Hi_01_Sound", "Dialog_NPC1_Hi_01_Subtitle")
	Player:say("Dialog_NPC1_Hi_02_Sound", "Dialog_NPC1_Hi_02_Subtitle")
end

function Dialog_NPC1_HaveYouQuest_Condition()
	if (wasHeard("Dialog_NPC1_Hi")) then
		return true
	end

	return false
end

function Dialog_NPC1_HaveYouQuest_Info()
end

function Dialog_NPC1_QuestSolved_Condition()
	if (wasHeard("Dialog_NPC1_HaveYouQuest")) then
		return true
	end

	return false
end

function Dialog_NPC1_QuestSolved_Info()
end

function Dialog_NPC1_Exit_Info()
	exitDialog()
end