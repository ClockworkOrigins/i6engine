def Dialog_NPC1_HaveYouQuest_Condition():
	if (wasHeard("Dialog_NPC1_Hi")):
		return True

	return False