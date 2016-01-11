def Dialog_NPC1_QuestSolved_Condition():
	Player = getNPC("Player01")
	if (wasHeard("Dialog_NPC1_HaveYouQuest") and Player.getItemCount("UsableItem") >= 3):
		return True

	return False