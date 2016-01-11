function Dialog_NPC1_HaveYouQuest_Condition()
	if (wasHeard("Dialog_NPC1_Hi")) then
		return true
	end

	return false
end