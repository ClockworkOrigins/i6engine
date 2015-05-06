import ScriptingMathPython
from ScriptingMathPython import *
import ScriptingObjectPython
from ScriptingObjectPython import *
import ScriptingGUIPython
from ScriptingGUIPython import *

def tick(id):
	go = getObjectPtr(getObject(id))
	if go == None:
		return;
	c = go.getGOC(ComponentTypes.PhysicalStateComponent)
	if c == None:
		return;
	psc = getPhysicalStateComponent(c)

	psc.applyCentralForce(i6eVector(0.0, 0.0, -50.0), True)
	return

def shatter(selfID, otherID):
	go = getObjectPtr(getObject(selfID))
	if go == None:
		return;
	c = go.getGOC(ComponentTypes.PhysicalStateComponent)
	if c == None:
		return;
	psc = getPhysicalStateComponent(c)

	psc.applyCentralForce(i6eVector(0.0, 1000.0, 5000.0), True)
	addPrint("ScriptCollision_" + str(selfID), "RPG/Blanko", 0.5, 0.48, "A crash? Registered in a python script? Nice!", Alignment.Center, 2000000)
	setSize("ScriptCollision_" + str(selfID), 1.0, 0.3)
	return