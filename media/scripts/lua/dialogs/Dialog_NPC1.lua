function Dialog_NPC1_Hi_Info()
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
end