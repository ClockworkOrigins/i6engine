def Dialog_NPC1_QuestSolved_Info():
	NPC01 = getNPC("NPC01")
	Player = getNPC("Player01")
	NPC01.say("Dialog_NPC1_QuestSolved_01_Sound", "Dialog_NPC1_QuestSolved_01_Subtitle")
	Player.say("Dialog_NPC1_QuestSolved_02_Sound", "Dialog_NPC1_QuestSolved_02_Subtitle")

	giveInvItems(Player, NPC01, "UsableItem", 3)
	giveXP(Player, 500)
	addLogEntryHelper("CollectUsableItemsQuest", "CollectUsableItemsQuest_Entry_2")
	setQuestStatus("CollectUsableItemsQuest", QuestStatus.COMPLETED)
	return