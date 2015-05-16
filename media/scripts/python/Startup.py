import ScriptingRPGPython
from ScriptingRPGPython import *

# This method is called during first loading of a level in RPG
def Startup():
	insertPlayerAtWaypoint("Player01", "Start")

	insertNPCAtWaypoint("NPC01", "WP_001")
	return
