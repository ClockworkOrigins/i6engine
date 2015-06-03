import ScriptingAPIPython
from ScriptingAPIPython import *
import ScriptingGUIPython
from ScriptingGUIPython import *
import ScriptingRPGPython
from ScriptingRPGPython import *

# adds a log entry
def addLogEntryHelper(quest, entry):
	addLogEntry(quest, entry)
	printMessage("LogEntryMessage", 0.5, 0.55, getTextManager().getText("NewLogEntry") + " (" + getQuestName(quest) + ")", Alignment.Center, 5000000, "DejaVuSans-8")
	return
