function Dialog_NPC1_Hi_Info()
	local NPC01 = getNPC("NPC01")
	local Player = getNPC("Player01")
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
	local NPC01 = getNPC("NPC01")
	local Player = getNPC("Player01")
	Player:say("Dialog_NPC1_HaveYouQuest_01_Sound", "Dialog_NPC1_HaveYouQuest_01_Subtitle")
	NPC01:say("Dialog_NPC1_HaveYouQuest_02_Sound", "Dialog_NPC1_HaveYouQuest_02_Subtitle")

	setQuestStatus("CollectUsableItemsQuest", QuestStatus.RUNNING)
	addLogEntryHelper("CollectUsableItemsQuest", "CollectUsableItemsQuest_Entry_1")
end

function Dialog_NPC1_QuestSolved_Condition()
	local Player = getNPC("Player01")
	if (wasHeard("Dialog_NPC1_HaveYouQuest") and Player:getItemCount("UsableItem") >= 3) then
		return true
	end

	return false
end

function Dialog_NPC1_QuestSolved_Info()
	local NPC01 = getNPC("NPC01")
	local Player = getNPC("Player01")
	NPC01:say("Dialog_NPC1_QuestSolved_01_Sound", "Dialog_NPC1_QuestSolved_01_Subtitle")
	Player:say("Dialog_NPC1_QuestSolved_02_Sound", "Dialog_NPC1_QuestSolved_02_Subtitle")

	giveInvItems(Player, NPC01, "UsableItem", 3)
	giveXP(Player, 500)
	addLogEntryHelper("CollectUsableItemsQuest", "CollectUsableItemsQuest_Entry_2")
	setQuestStatus("CollectUsableItemsQuest", QuestStatus.COMPLETED)
end

function Dialog_NPC1_Exit_Info()
	local Player = getNPC("Player01")
	Player:say("Dialog_NPC1_Exit_01_Sound", "Dialog_NPC1_Exit_01_Subtitle")
	exitDialog()
end