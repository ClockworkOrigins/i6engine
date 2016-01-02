function Dialog_NPC1_QuestSolved_Condition()
	local Player = getNPC("Player01")
	if (wasHeard("Dialog_NPC1_HaveYouQuest") and Player:getItemCount("UsableItem") >= 3) then
		return true
	end

	return false
end