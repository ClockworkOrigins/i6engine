# adds a log entry
# i6Function: void addLogEntryHelper(QuestIdentifier,string)
def addLogEntryHelper(quest, entry):
	addLogEntry(quest, entry)
	printMessage("LogEntryMessage", 0.5, 0.55, getTextManager().getText("NewLogEntry") + " (" + getQuestName(quest) + ")", Alignment.Center, 5000000, "DejaVuSans-8")
	return
