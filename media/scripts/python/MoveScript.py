def init(id):
	go = getObject(id)
	if go == None:
		return
	vc = go.getVelocityComponent()
	if vc == None:
		return

	vc.accelerate("")
	return

def tick(id):
	return

def shatter(selfID, otherID):
	go = getObject(selfID)
	if go == None:
		return
	psc = go.getPhysicalStateComponent()
	if psc == None:
		return

	psc.applyCentralForce(Vec3(0.0, 1000.0, 5000.0), True)
	addPrint("ScriptCollision_" + str(selfID), "RPG/Blanko", 0.5, 0.48, "A crash? Registered in a python script? Nice!", Alignment.Center, 3000000)
	setSize("ScriptCollision_" + str(selfID), 1.0, 0.3)
	return
