def Dialog_NPC1_HaveYouQuest_Info():
	NPC01 = getNPC("NPC01")
	Player = getNPC("Player01")
	Player.say("Dialog_NPC1_HaveYouQuest_01_Sound", "Dialog_NPC1_HaveYouQuest_01_Subtitle")
	NPC01.say("Dialog_NPC1_HaveYouQuest_02_Sound", "Dialog_NPC1_HaveYouQuest_02_Subtitle")

	setQuestStatus("CollectUsableItemsQuest", QuestStatus.RUNNING)
	addLogEntryHelper("CollectUsableItemsQuest", "CollectUsableItemsQuest_Entry_1")
	return