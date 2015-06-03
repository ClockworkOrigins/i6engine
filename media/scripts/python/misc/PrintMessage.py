import ScriptingGUIPython
from ScriptingGUIPython import *

# prints a message
def printMessage(name, x, y, message, alignment, lifetime, font):
	addPrint(name, "RPG/Blanko", x, y, message, alignment, lifetime)
	setSize(name, 1.0, 0.05)
	setFont(name, font)
	return