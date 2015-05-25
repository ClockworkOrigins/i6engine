﻿import ScriptingRPGPython
from ScriptingRPGPython import *

def Dialog_NPC1_Hi_Info():
	NPC01 = getNPC("NPC01")
	Player = getNPC("Player01")
	NPC01.say("Dialog_NPC1_Hi_01_Sound", "Dialog_NPC1_Hi_01_Subtitle")
	Player.say("Dialog_NPC1_Hi_02_Sound", "Dialog_NPC1_Hi_02_Subtitle")
	return

def Dialog_NPC1_HaveYouQuest_Condition():
	if (wasHeard("Dialog_NPC1_Hi")):
		return True

	return False

def Dialog_NPC1_HaveYouQuest_Info():
	NPC01 = getNPC("NPC01")
	Player = getNPC("Player01")
	Player.say("Dialog_NPC1_HaveYouQuest_01_Sound", "Dialog_NPC1_HaveYouQuest_01_Subtitle")
	NPC01.say("Dialog_NPC1_HaveYouQuest_02_Sound", "Dialog_NPC1_HaveYouQuest_02_Subtitle")
	return

def Dialog_NPC1_QuestSolved_Condition():
	if (wasHeard("Dialog_NPC1_HaveYouQuest")):
		return True

	return False

def Dialog_NPC1_QuestSolved_Info():
	NPC01 = getNPC("NPC01")
	Player = getNPC("Player01")
	NPC01.say("Dialog_NPC1_QuestSolved_01_Sound", "Dialog_NPC1_QuestSolved_01_Subtitle")
	Player.say("Dialog_NPC1_QuestSolved_02_Sound", "Dialog_NPC1_QuestSolved_02_Subtitle")
	return

def Dialog_NPC1_Exit_Info():
	Player = getNPC("Player01")
	Player.say("Dialog_NPC1_Exit_01_Sound", "Dialog_NPC1_Exit_01_Subtitle")
	exitDialog()
	return
