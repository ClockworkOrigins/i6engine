import ScriptingRPGPython
from ScriptingRPGPython import *

# gives xp to an NPC
def giveXP(self, xp):
	self.setCurrentXP(self.getCurrentXP() + xp)
	if (self.getCurrentXP() >= self.getNextXP()):
		self.setLevel(self.getLevel() + 1)
		self.setNextXP(self.getNextXP() + (self.getLevel() + 1) * 500)
	return